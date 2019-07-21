// Generated by the gRPC C++ plugin.
// If you make any local change, they will be lost.
// source: messages.proto
#ifndef GRPC_messages_2eproto__INCLUDED
#define GRPC_messages_2eproto__INCLUDED

#include "messages.pb.h"

#include <functional>
#include <grpcpp/impl/codegen/async_generic_service.h>
#include <grpcpp/impl/codegen/async_stream.h>
#include <grpcpp/impl/codegen/async_unary_call.h>
#include <grpcpp/impl/codegen/client_callback.h>
#include <grpcpp/impl/codegen/method_handler_impl.h>
#include <grpcpp/impl/codegen/proto_utils.h>
#include <grpcpp/impl/codegen/rpc_method.h>
#include <grpcpp/impl/codegen/server_callback.h>
#include <grpcpp/impl/codegen/service_type.h>
#include <grpcpp/impl/codegen/status.h>
#include <grpcpp/impl/codegen/stub_options.h>
#include <grpcpp/impl/codegen/sync_stream.h>

namespace grpc {
class CompletionQueue;
class Channel;
class ServerCompletionQueue;
class ServerContext;
}  // namespace grpc

namespace exchange {

class Exchange final {
 public:
  static constexpr char const* service_full_name() {
    return "exchange.Exchange";
  }
  class StubInterface {
   public:
    virtual ~StubInterface() {}
    std::unique_ptr< ::grpc::ClientReaderWriterInterface< ::exchange::ClientMessage, ::exchange::ServerMessage>> Message(::grpc::ClientContext* context) {
      return std::unique_ptr< ::grpc::ClientReaderWriterInterface< ::exchange::ClientMessage, ::exchange::ServerMessage>>(MessageRaw(context));
    }
    std::unique_ptr< ::grpc::ClientAsyncReaderWriterInterface< ::exchange::ClientMessage, ::exchange::ServerMessage>> AsyncMessage(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderWriterInterface< ::exchange::ClientMessage, ::exchange::ServerMessage>>(AsyncMessageRaw(context, cq, tag));
    }
    std::unique_ptr< ::grpc::ClientAsyncReaderWriterInterface< ::exchange::ClientMessage, ::exchange::ServerMessage>> PrepareAsyncMessage(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderWriterInterface< ::exchange::ClientMessage, ::exchange::ServerMessage>>(PrepareAsyncMessageRaw(context, cq));
    }
    class experimental_async_interface {
     public:
      virtual ~experimental_async_interface() {}
      virtual void Message(::grpc::ClientContext* context, ::grpc::experimental::ClientBidiReactor< ::exchange::ClientMessage,::exchange::ServerMessage>* reactor) = 0;
    };
    virtual class experimental_async_interface* experimental_async() { return nullptr; }
  private:
    virtual ::grpc::ClientReaderWriterInterface< ::exchange::ClientMessage, ::exchange::ServerMessage>* MessageRaw(::grpc::ClientContext* context) = 0;
    virtual ::grpc::ClientAsyncReaderWriterInterface< ::exchange::ClientMessage, ::exchange::ServerMessage>* AsyncMessageRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) = 0;
    virtual ::grpc::ClientAsyncReaderWriterInterface< ::exchange::ClientMessage, ::exchange::ServerMessage>* PrepareAsyncMessageRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) = 0;
  };
  class Stub final : public StubInterface {
   public:
    Stub(const std::shared_ptr< ::grpc::ChannelInterface>& channel);
    std::unique_ptr< ::grpc::ClientReaderWriter< ::exchange::ClientMessage, ::exchange::ServerMessage>> Message(::grpc::ClientContext* context) {
      return std::unique_ptr< ::grpc::ClientReaderWriter< ::exchange::ClientMessage, ::exchange::ServerMessage>>(MessageRaw(context));
    }
    std::unique_ptr<  ::grpc::ClientAsyncReaderWriter< ::exchange::ClientMessage, ::exchange::ServerMessage>> AsyncMessage(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderWriter< ::exchange::ClientMessage, ::exchange::ServerMessage>>(AsyncMessageRaw(context, cq, tag));
    }
    std::unique_ptr<  ::grpc::ClientAsyncReaderWriter< ::exchange::ClientMessage, ::exchange::ServerMessage>> PrepareAsyncMessage(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncReaderWriter< ::exchange::ClientMessage, ::exchange::ServerMessage>>(PrepareAsyncMessageRaw(context, cq));
    }
    class experimental_async final :
      public StubInterface::experimental_async_interface {
     public:
      void Message(::grpc::ClientContext* context, ::grpc::experimental::ClientBidiReactor< ::exchange::ClientMessage,::exchange::ServerMessage>* reactor) override;
     private:
      friend class Stub;
      explicit experimental_async(Stub* stub): stub_(stub) { }
      Stub* stub() { return stub_; }
      Stub* stub_;
    };
    class experimental_async_interface* experimental_async() override { return &async_stub_; }

   private:
    std::shared_ptr< ::grpc::ChannelInterface> channel_;
    class experimental_async async_stub_{this};
    ::grpc::ClientReaderWriter< ::exchange::ClientMessage, ::exchange::ServerMessage>* MessageRaw(::grpc::ClientContext* context) override;
    ::grpc::ClientAsyncReaderWriter< ::exchange::ClientMessage, ::exchange::ServerMessage>* AsyncMessageRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq, void* tag) override;
    ::grpc::ClientAsyncReaderWriter< ::exchange::ClientMessage, ::exchange::ServerMessage>* PrepareAsyncMessageRaw(::grpc::ClientContext* context, ::grpc::CompletionQueue* cq) override;
    const ::grpc::internal::RpcMethod rpcmethod_Message_;
  };
  static std::unique_ptr<Stub> NewStub(const std::shared_ptr< ::grpc::ChannelInterface>& channel, const ::grpc::StubOptions& options = ::grpc::StubOptions());

  class Service : public ::grpc::Service {
   public:
    Service();
    virtual ~Service();
    virtual ::grpc::Status Message(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::exchange::ServerMessage, ::exchange::ClientMessage>* stream);
  };
  template <class BaseClass>
  class WithAsyncMethod_Message : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithAsyncMethod_Message() {
      ::grpc::Service::MarkMethodAsync(0);
    }
    ~WithAsyncMethod_Message() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Message(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::exchange::ServerMessage, ::exchange::ClientMessage>* stream)  override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestMessage(::grpc::ServerContext* context, ::grpc::ServerAsyncReaderWriter< ::exchange::ServerMessage, ::exchange::ClientMessage>* stream, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncBidiStreaming(0, context, stream, new_call_cq, notification_cq, tag);
    }
  };
  typedef WithAsyncMethod_Message<Service > AsyncService;
  template <class BaseClass>
  class ExperimentalWithCallbackMethod_Message : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    ExperimentalWithCallbackMethod_Message() {
      ::grpc::Service::experimental().MarkMethodCallback(0,
        new ::grpc::internal::CallbackBidiHandler< ::exchange::ClientMessage, ::exchange::ServerMessage>(
          [this] { return this->Message(); }));
    }
    ~ExperimentalWithCallbackMethod_Message() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Message(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::exchange::ServerMessage, ::exchange::ClientMessage>* stream)  override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::experimental::ServerBidiReactor< ::exchange::ClientMessage, ::exchange::ServerMessage>* Message() {
      return new ::grpc::internal::UnimplementedBidiReactor<
        ::exchange::ClientMessage, ::exchange::ServerMessage>;}
  };
  typedef ExperimentalWithCallbackMethod_Message<Service > ExperimentalCallbackService;
  template <class BaseClass>
  class WithGenericMethod_Message : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithGenericMethod_Message() {
      ::grpc::Service::MarkMethodGeneric(0);
    }
    ~WithGenericMethod_Message() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Message(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::exchange::ServerMessage, ::exchange::ClientMessage>* stream)  override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
  };
  template <class BaseClass>
  class WithRawMethod_Message : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    WithRawMethod_Message() {
      ::grpc::Service::MarkMethodRaw(0);
    }
    ~WithRawMethod_Message() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Message(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::exchange::ServerMessage, ::exchange::ClientMessage>* stream)  override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    void RequestMessage(::grpc::ServerContext* context, ::grpc::ServerAsyncReaderWriter< ::grpc::ByteBuffer, ::grpc::ByteBuffer>* stream, ::grpc::CompletionQueue* new_call_cq, ::grpc::ServerCompletionQueue* notification_cq, void *tag) {
      ::grpc::Service::RequestAsyncBidiStreaming(0, context, stream, new_call_cq, notification_cq, tag);
    }
  };
  template <class BaseClass>
  class ExperimentalWithRawCallbackMethod_Message : public BaseClass {
   private:
    void BaseClassMustBeDerivedFromService(const Service *service) {}
   public:
    ExperimentalWithRawCallbackMethod_Message() {
      ::grpc::Service::experimental().MarkMethodRawCallback(0,
        new ::grpc::internal::CallbackBidiHandler< ::grpc::ByteBuffer, ::grpc::ByteBuffer>(
          [this] { return this->Message(); }));
    }
    ~ExperimentalWithRawCallbackMethod_Message() override {
      BaseClassMustBeDerivedFromService(this);
    }
    // disable synchronous version of this method
    ::grpc::Status Message(::grpc::ServerContext* context, ::grpc::ServerReaderWriter< ::exchange::ServerMessage, ::exchange::ClientMessage>* stream)  override {
      abort();
      return ::grpc::Status(::grpc::StatusCode::UNIMPLEMENTED, "");
    }
    virtual ::grpc::experimental::ServerBidiReactor< ::grpc::ByteBuffer, ::grpc::ByteBuffer>* Message() {
      return new ::grpc::internal::UnimplementedBidiReactor<
        ::grpc::ByteBuffer, ::grpc::ByteBuffer>;}
  };
  typedef Service StreamedUnaryService;
  typedef Service SplitStreamedService;
  typedef Service StreamedService;
};

}  // namespace exchange


#endif  // GRPC_messages_2eproto__INCLUDED
