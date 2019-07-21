#include <iostream>
#include <map>

#include <grpcpp/grpcpp.h>

#include "messages.grpc.pb.h"
#include "messages.pb.h"

using namespace grpc;
using namespace exchange;
 
#define BOND "BOND"
#define VALBZ "VALBZ"
#define VALE "VALE"

typedef int price;
typedef int size;
typedef std::map<price, size> orders;

struct Market_data {
  orders buy;
  orders sell;
};

std::map<string, Market_data*> empty_record() {
    std::map<string, Market_data*> record;
    record[BOND] = new Market_data();
    record[VALBZ] = new Market_data();
    record[VALE] = new Market_data();
    return record;
}

std::map<string, Market_data*> record = empty_record();

std::string to_string_symbol(StockType symbol) {
    return StockType_Name(symbol);
}

class ServerImpl final : public Exchange::Service {
    Status Hello(ServerContext* context, grpc::ServerWriter<ServerMessage>* writer) {
        ServerMessage* hello_response;
        hello_response->set_t(ServerMessage::HELLO);
        writer->Write(*hello_response);
        return Status::OK;
    }

    Status AddOrder(ServerContext* context, const exchange::ClientMessage_Transaction transaction,
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
        return Status::OK;
    }

    Status Serve(ServerContext* context, const ClientMessage* query, 
        grpc::ServerWriter<ServerMessage>* writer) {
        if (query->t() == ClientMessage_MessageType_HELLO)
            return Hello(context, writer);
        else if (query->t() == ClientMessage_MessageType_ADD_ORDER)
            return AddOrder(context, query->add_order(), writer);
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