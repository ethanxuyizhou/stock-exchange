
exchange::Server hello () {
  exchange::Server hello;
  hello->set_t(exchange::Server::MessageType::HELLO);
  return hello;
}

exchange::Server book (exchange::StockType symbol, exchange::Server::Position position) {
  exchange::Server book;
  book->set_t(exchange::Server::MessageType::BOOK);
  position->set_book(position);
  return book;
}


int main () {
  GOOGLE_PROTOBUF_VERIFY_VERSION;
  

  return 0;
}
