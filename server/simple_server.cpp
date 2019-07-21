#include <iostream>
#include <map>

#include <grpcpp/grpcpp.h>

#include "messages.grpc.pb.h"
#include "messages.pb.h"

using namespace grpc;
using namespace exchange;

#define STOCKTYPE_BOND StockType::BOND
#define STOCKTYPE_VALBZ StockType::VALBZ
#define STOCKTYPE_VALE StockType::VALE

typedef int price;

struct Offer {
    int size;
    std::string name;
    Offer(int size, std::string name) {
        size = size;
        name = name;
    }
};

typedef std::map<price, std::vector<Offer>> orders;

struct Market_data {
  StockType symbol;
  orders buy;
  orders sell;
  Market_data(exchange::StockType symbol) {
    symbol = symbol;
    buy = {};
    sell = {};
  }
};

std::map<string, Market_data*> empty_record() {
    std::map<string, Market_data*> record;
    record["BOND"] = new Market_data(STOCKTYPE_BOND);
    record["VALBZ"] = new Market_data(STOCKTYPE_VALBZ);
    record["VALE"] = new Market_data(STOCKTYPE_VALE);
    return record;
}

std::map<string, Market_data*> record = empty_record();

std::mutex record_mutex;

std::string to_string_symbol(StockType symbol) {
    return StockType_Name(symbol);
}

class ServerImpl final : public Exchange::Service {

    int accumulate_offers(std::vector<Offer>& offers) {
        int total = 0;
        for (auto &offer : offers)
            total += offer.size;
        return total;
    }

    void Hello(grpc::ServerReaderWriter<ServerMessage, ClientMessage>* stream) {
        ServerMessage* hello_response;
        hello_response->set_t(ServerMessage::HELLO);
        stream->Write(*hello_response);
        
        record_mutex.lock();
        for (auto it = record.begin(); it != record.end(); ++it) {
            ServerMessage* book_response;
            ServerMessage::Position* position;
            position->set_symbol(it->second->symbol);
            for (auto order = it->second->buy.begin(); order != it->second->buy.end(); ++order) {
                int price = order->first;
                int size = accumulate_offers(order->second);
                if (size > 0) {
                    PriceAndSize* x = position->add_buy();
                    x->set_price(price);
                    x->set_size(size);
                }
            }
            for (auto order = it->second->sell.begin(); order != it->second->sell.end(); ++order) {
                int price = order->first;
                int size = accumulate_offers(order->second);
                if (size > 0) {
                    PriceAndSize* x = position->add_sell();
                    x->set_price(price);
                    x->set_size(size);
                }
            }
            book_response->set_t(ServerMessage_MessageType_BOOK);
            book_response->set_allocated_book(position);
            stream->Write(*book_response);
        }  
        record_mutex.unlock(); 
    }

    void AddOrder(const exchange::ClientMessage* query) {
        const exchange::ClientMessage_Transaction transaction = query->add_order();
        std::string symbol = to_string_symbol(transaction.symbol());
        int price = transaction.price();
        int size = transaction.size();
        record_mutex.lock();

        std::string name = query->name();

        std::vector<Offer> empty_vec;
        Offer offer = Offer(size, name);

        if (transaction.dir() == Dir::BUY) {
            if (record[symbol]->buy.find(price) == record[symbol]->buy.end())
                record[symbol]->buy[price] = empty_vec;
            record[symbol]->buy[price].push_back(offer);
        } 
        else if (transaction.dir() == Dir::SELL) {
            if (record[symbol]->sell.find(price) == record[symbol]->sell.end())
                record[symbol]->sell[price] = empty_vec;
            record[symbol]->sell[price].push_back(offer);
        }

        record_mutex.unlock();
    }

    Status Message(ServerContext* context,
        grpc::ServerReaderWriter<ServerMessage, ClientMessage>* stream) {
        ClientMessage* query;
        while (stream->Read(query)) {
            if (query->t() == ClientMessage_MessageType_HELLO)
                Hello(stream);
            else if (query->t() == ClientMessage_MessageType_ADD_ORDER)
                AddOrder(query);
        }
        return Status::OK;
    }
};

void RunServer() {
    std::string server_address = "0.0.0.0:50051";
    ServerImpl service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;
    server->Wait();
}

int main() {
    RunServer();
    return 0;
}
