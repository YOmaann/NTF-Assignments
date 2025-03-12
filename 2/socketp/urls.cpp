#include <iostream>
#include <regex>
#include "socketp.h"

urls::urls(std::string _url) {
  url = _url;
  regex_p = R"(^(https?:\/\/)?([^\/\?:]+)(:(\d+))?([\/\?][^\s]*)?$)";

  std::regex rgx(regex_p);
  std::smatch match;

  

  if(!std::regex_search(url, match, rgx)) {
    flag = false;
    return;
  }
  protocol = match[1].str();
  hostname = match[2].str();
  path = match[5].str();
  port = match[4].str();
  flag = true;
  // std::cout << match[0].str() << std::endl;
  // std::cout << match[1].str() << std::endl;
  // std::cout << match[2].str() << std::endl;
  // std::cout << match[3].str() << std::endl;
}

const char* urls::get_host() {
    if(!flag) return NULL;
    return hostname.c_str();
}

const char* urls::get_path() {
    if(!flag) return NULL;
    return path.c_str();
}
const char* urls::get_port() {
    if(!flag) return NULL;
    return port.c_str();
}
