#pragma once
// specification file

#include <string>
#include <exception>
#include <netinet/in.h>
#include <sys/socket.h>
#include <vector>

class SocketCreationError : public std::exception {
  std::string msg;

 public:
  SocketCreationError(const char* _msg);
  const char* what() const throw();
};

class CacheError : public std::exception {
  std::string msg;

 public:
  CacheError(const char* _msg);
  const char* what() const throw();
};


class socketp {
public:
  int check(int errorC, const char* errorM, const char* passM);
  sockaddr_in get_address(int port, uint32_t addr);
  sockaddr_in get_address(int port, const char* addr);
  std::string get_data(int s, int buffer_s);
  std::string get_GET_data(std::string response);
};

class urls {
  std::string url, protocol, hostname, path, regex_p, port;
  bool flag;
public:
  urls(std::string _url);
  const char* get_host();
  const char* get_path();
  const char* get_port();
};

class cache {
  std::string path;

  std::string sanitize(std::string i);
  std::vector<std::string> adv_tokenizer(std::string s, char del);
  std::string recur_dir(std::vector<std::string> paths);
public:
  bool check_cache(std::string file);
  void create_cache(std::string hostname, std::string contents);
  std::vector<std::string> get_cache(std::string file);
};
