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

std::string BOND = "BOND";
std::string VALBZ = "VALBZ";
std::string VALE = "VALE";

typedef int price;
typedef int size;
typedef std::map<price, size> orders;

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

std::string to_string_symbol(StockType symbol) {
    return StockType_Name(symbol);
}

class ServerImpl final : public Exchange::Service {
    void Hello(ServerContext* context, grpc::ServerWriter<ServerMessage>* writer) {
        ServerMessage* hello_response;
        hello_response->set_t(ServerMessage::HELLO);
        writer->Write(*hello_response);
        
        for (auto it = record.begin(); it != record.end(); ++it) {
            ServerMessage* book_response;
            ServerMessage::Position* position;
            position->set_symbol(it->second->symbol);
            for (auto order = it->second->buy.begin(); order != it->second->buy.end(); ++order) {
                PriceAndSize* x = position->add_buy();
                x->set_price(order->first);
                x->set_size(order->second);
            }
            for (auto order = it->second->sell.begin(); order != it->second->sell.end(); ++order) {
                PriceAndSize* x = position->add_sell();
                x->set_price(order->first);
                x->set_size(order->second);
            }
            book_response->set_t(ServerMessage_MessageType_BOOK);
            book_response->set_allocated_book(position);
            writer->Write(*book_response);
        }   
    }

    void AddOrder(ServerContext* context, const exchange::ClientMessage_Transaction transaction,
        grpc::ServerWriter<ServerMessage>* writer) {
        std::string symbol = to_string_symbol(transaction.symbol());
        int price = transaction.price();
        int size = transaction.size();
        if (transaction.dir() == Dir::BUY) {
            if (record[symbol]->buy.find(price) == record[symbol]->buy.end())
                record[symbol]->buy[price] = size;
            else
                record[symbol]->buy[price] += size;
        } else {
            if (record[symbol]->sell.find(price) == record[symbol]->sell.end())
                record[symbol]->sell[price] = size;
            else
                record[symbol]->sell[price] += size;
        }
    }

    Status Serve(ServerContext* context, const ClientMessage* query, 
        grpc::ServerWriter<ServerMessage>* writer) {
        if (query->t() == ClientMessage_MessageType_HELLO)
            Hello(context, writer);
        else if (query->t() == ClientMessage_MessageType_ADD_ORDER)
            AddOrder(context, query->add_order(), writer);
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
