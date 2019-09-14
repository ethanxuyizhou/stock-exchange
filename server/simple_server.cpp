#include <iostream>
#include <map>

#include "../protocol/messages.grpc.pb.h"
#include "../protocol/messages.pb.h"
#include <grpc/grpc.h>
#include <grpcpp/grpcpp.h>

using namespace grpc;
using namespace exchange;

struct Offer {
  int size;
  std::string name;
  Offer(int s, std::string n) {
    size = s;
    name = n;
  }

  static int total_size(const std::vector<Offer> &x) {
    int total = 0;
    for (Offer offer : x)
      total += offer.size;
    return total;
  }
};

class Market_data {
  std::map<int, std::vector<Offer>> buy;
  std::map<int, std::vector<Offer>> sell;

  std::map<int, int>
  get_accumulated_data(const std::map<int, std::vector<Offer>> &x) {
    std::map<int, int> result;
    for (auto it = x.begin(); it != x.end(); ++it)
      result[it->first] = Offer::total_size(it->second);
    return result;
  }

public:
  void add_buy(int price, int size, std::string name) {
    if (buy.find(price) == buy.end())
      buy[price] = {};
    buy[price].push_back(Offer(size, name));
  }

  void add_sell(int price, int size, std::string name) {
    if (sell.find(price) == sell.end())
      sell[price] = {};
    sell[price].push_back(Offer(size, name));
  }

  void set_buy(int price, std::vector<Offer> b) { buy[price] = b; }

  void set_sell(int price, std::vector<Offer> s) { sell[price] = s; }

  std::map<int, int> get_accumulated_buy() { return get_accumulated_data(buy); }

  std::map<int, int> get_accumulated_sell() {
    return get_accumulated_data(sell);
  }

  std::map<int, std::vector<Offer>> get_buy() { return buy; }

  std::map<int, std::vector<Offer>> get_sell() { return sell; }
};

std::map<StockType, Market_data> empty_record() {
  std::map<StockType, Market_data> record;
  Market_data bond, valbz, vale;
  record[StockType::BOND] = bond;
  record[StockType::VALBZ] = valbz;
  record[StockType::VALE] = vale;
  return record;
}

std::map<StockType, Market_data> record = empty_record();

std::map<string, grpc::ServerReaderWriter<ServerMessage, ClientMessage> *>
    writers;

std::mutex record_mutex;

class ServerImpl final : public Exchange::Service {
  void Hello(const exchange::ClientMessage query,
             grpc::ServerReaderWriter<ServerMessage, ClientMessage> *stream) {
    record_mutex.lock();
    writers.insert(make_pair(query.name(), stream));
    for (auto it = record.begin(); it != record.end(); ++it) {
      ServerMessage book_response;
      ServerMessage::Position *position = new ServerMessage::Position();
      position->set_symbol(it->first);

      std::map<int, int> buy_info = it->second.get_accumulated_buy();
      std::map<int, int> sell_info = it->second.get_accumulated_sell();
      for (auto it = buy_info.begin(); it != buy_info.end(); ++it) {
        int price = it->first;
        int size = it->second;
        if (size > 0) {
          PriceAndSize *x = position->add_buy();
          x->set_price(price);
          x->set_size(size);
        }
      }
      for (auto it = sell_info.begin(); it != sell_info.end(); ++it) {
        int price = it->first;
        int size = it->second;
        if (size > 0) {
          PriceAndSize *x = position->add_sell();
          x->set_price(price);
          x->set_size(size);
        }
      }
      book_response.set_t(ServerMessage_MessageType_BOOK);
      book_response.set_allocated_book(position);
      stream->Write(book_response);
    }
    record_mutex.unlock();
  }

  void sendFillOrders(const StockType &symbol, int price, Dir dir, int size,
                      const std::string &name) {
    ServerMessage *response;
    response->set_t(ServerMessage::FILL);
    ServerMessage::Fill *fill_response;
    fill_response->set_symbol(symbol);
    fill_response->set_size(size);
    fill_response->set_dir(dir);
    response->set_allocated_fill(fill_response);
    writers[name]->Write(*response);
  }

  void combineOrders(const StockType &symbol, int price) {
    Market_data market_data = record[symbol];
    std::map<int, std::vector<Offer>> buy = market_data.get_buy();
    std::map<int, std::vector<Offer>> sell = market_data.get_sell();
    if (buy.find(price) == buy.end() || sell.find(price) == sell.end())
      return;
    std::vector<Offer> buy_offers = buy[price];
    std::vector<Offer> sell_offers = sell[price];
    std::vector<Offer> new_buy_offers, new_sell_offers;

    int buy_size = Offer::total_size(buy_offers);
    int sell_size = Offer::total_size(sell_offers);
    // TODO: move duplicated code into another function.
    if (buy_size == sell_size) {
      for (Offer offer : buy_offers)
        sendFillOrders(symbol, price, Dir::BUY, offer.size, offer.name);
      for (Offer offer : sell_offers)
        sendFillOrders(symbol, price, Dir::SELL, offer.size, offer.name);
    } else if (buy_size > sell_size) {
      for (Offer offer : sell_offers)
        sendFillOrders(symbol, price, Dir::SELL, offer.size, offer.name);
      int accum = 0;
      int index;
      for (int i = 0; i < buy_offers.size(); ++i) {
        Offer offer = buy_offers[i];
        if (accum + offer.size > sell_size) {
          sendFillOrders(symbol, price, Dir::BUY, sell_size - accum,
                         offer.name);
          index = i;
          break;
        } else if (accum + offer.size == sell_size) {
          sendFillOrders(symbol, price, Dir::BUY, offer.size, offer.name);
          index = i + 1;
          break;
        } else {
          sendFillOrders(symbol, price, Dir::BUY, offer.size, offer.name);
          accum += offer.size;
        }
      }
      new_buy_offers.assign(buy_offers.begin() + index, buy_offers.end());
    } else {
      for (Offer offer : buy_offers)
        sendFillOrders(symbol, price, Dir::BUY, offer.size, offer.name);
      int accum = 0;
      int index;
      for (int i = 0; i < sell_offers.size(); ++i) {
        Offer offer = sell_offers[i];
        if (accum + offer.size > buy_size) {
          sendFillOrders(symbol, price, Dir::SELL, buy_size - accum,
                         offer.name);
          index = i;
          break;
        } else if (accum + offer.size == buy_size) {
          sendFillOrders(symbol, price, Dir::SELL, offer.size, offer.name);
          index = i + 1;
          break;
        } else {
          sendFillOrders(symbol, price, Dir::SELL, offer.size, offer.name);
          accum += offer.size;
        }
      }
      new_sell_offers.assign(sell_offers.begin() + index, sell_offers.end());
    }
    record[symbol].set_buy(price, new_buy_offers);
    record[symbol].set_sell(price, new_sell_offers);
  }

  void
  AddOrder(const exchange::ClientMessage query,
           grpc::ServerReaderWriter<ServerMessage, ClientMessage> *stream) {
    const exchange::ClientMessage_Transaction transaction = query.add_order();
    StockType symbol = transaction.symbol();
    int price = transaction.price();
    int size = transaction.size();
    std::string name = query.name();
    record_mutex.lock();
    writers.insert(make_pair(query.name(), stream));
    if (transaction.dir() == Dir::BUY)
      record[symbol].add_buy(price, size, name);
    else if (transaction.dir() == Dir::SELL)
      record[symbol].add_sell(price, size, name);
    combineOrders(symbol, price);
    record_mutex.unlock();
  }

  Status
  Message(ServerContext *context,
          grpc::ServerReaderWriter<ServerMessage, ClientMessage> *stream) {
    ClientMessage query;
    while (stream->Read(&query)) {
      if (query.t() == ClientMessage_MessageType_HELLO) {
        Hello(query, stream);
      } else if (query.t() == ClientMessage_MessageType_ADD_ORDER)
        AddOrder(query, stream);
    }
    return Status::OK;
  }
};

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Supply only one host and port address for the server\n");
    return -1;
  }
  std::string server_address(argv[1]);
  ServerImpl service;
  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
}
