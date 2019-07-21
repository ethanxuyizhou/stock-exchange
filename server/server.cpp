#include <iostream>
#include <thread>

#include <grpcpp/grpcpp.h>

class ServerImpl final {
  public:
    ~ServerImpl() {
      server_->Shutdown();
      cq_->Shutdown();
    }

    void Run() {
      std::string server_address("0.0.0.0:50051");

      grpc::ServerBuilder builder;
      builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
      builder.RegisterService(&service_);
      // Get hold of the completion queue used for the asynchronous communication
      // with the gRPC runtime.
      cq_ = builder.AddCompletionQueue();
      // Finally assemble the server.
      server_ = builder.BuildAndStart();
      std::cout << "Server listening on " << server_address << std::endl;

      HandleExchanges();
    }

  private:
    void HandleExchanges() {

    }

    std::unique_ptr<ServerCompletionQueue> cq_;
    Greeter:AsyncService service_;
    std::unique_ptr<Server> server_;
}

int main (int argc, char** argv) {
  GOOGLE_PROTOBUF_VERIFY_VERSION;
  ServerImpl server;
  server.Run();
  return 0;
}
