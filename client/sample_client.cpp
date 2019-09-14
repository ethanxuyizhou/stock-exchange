#include "../protocol/messages.grpc.pb.h"
#include "../protocol/messages.pb.h"
#include <grpc/grpc.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/security/credentials.h>

using namespace grpc;
using namespace exchange;

class Client {
public:
  Client(std::shared_ptr<Channel> channel) : stub_(Exchange::NewStub(channel)) {

  }

  void trade() {
    ClientContext context;
    std::shared_ptr<
        ClientReaderWriter<exchange::ClientMessage, exchange::ServerMessage>>
        stream(stub_->Message(&context));
    ClientMessage add_order_message;
    add_order_message.set_t(ClientMessage_MessageType_ADD_ORDER);
    ClientMessage::Transaction *transaction = new ClientMessage::Transaction();
    transaction->set_symbol(StockType::BOND);
    transaction->set_price(1000);
    transaction->set_size(1000);
    transaction->set_dir(Dir::BUY);
    add_order_message.set_allocated_add_order(transaction);
    add_order_message.set_name("Ethan");
    stream->Write(add_order_message);

    ClientMessage hello_message;
    hello_message.set_t(ClientMessage_MessageType_HELLO);
    hello_message.set_name("Ethan");
    stream->Write(hello_message);
    ServerMessage response;
    while (stream->Read(&response)) {
      std::cout << "Read something" << std::endl;
    }
  }

private:
  std::unique_ptr<Exchange::Stub> stub_;
};

int main() { 
  Client client(grpc::CreateChannel("localhost:50051",
                                    grpc::InsecureChannelCredentials()));
  client.trade();
}
