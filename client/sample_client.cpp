#include "../protocol/messages.grpc.pb.h"
#include "../protocol/messages.pb.h"
#include <grpc/grpc.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/security/credentials.h>

using namespace grpc;
using exchange::Exchange;

class Client {
public:
  Client(std::shared_ptr<Channel> channel) { Exchange::NewStub(channel); }

  void trade() {
    ClientContext context;
    std::shared_ptr<
        ClientReaderWriter<exchange::ClientMessage, exchange::ServerMessage>>
        stream(stub_->Message(&context));
	
  }

private:
  std::unique_ptr<Exchange::Stub> stub_;
};

int main() {
  Client client(grpc::CreateChannel("localhost:50051",
                                    grpc::InsecureChannelCredentials()));
  client.trade();
}