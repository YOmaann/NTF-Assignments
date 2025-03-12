#include  "socketp.h"

SocketCreationError::SocketCreationError(const char* _msg) : msg(_msg) {}
const char* SocketCreationError::what() const throw() {
  return msg.c_str();
}

CacheError::CacheError(const char* _msg) : msg(_msg) {}
const char* CacheError::what() const throw() {
  return msg.c_str();
}
