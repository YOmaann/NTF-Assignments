// Implementation File - helper functions for socket programming assignment

#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "socketp.h"
#include <regex>
#include <strings.h>

int socketp::check(int errorC, const char* errorM, const char* passM) {
    if(errorC == -1) {
      throw SocketCreationError(errorM);
    }
    else
      std::cout << passM << std::endl;
    return errorC;
}

sockaddr_in socketp::get_address(int port = 8080, uint32_t addr = INADDR_ANY){
  sockaddr_in a;
  a.sin_family = AF_INET;
  a.sin_port = htons(port);
  a.sin_addr.s_addr = addr;

  return a;
}

sockaddr_in socketp::get_address(int port = 8080, const char* addr = "127.0.0.0") {
  return get_address(port, inet_addr(addr));
}

std::string socketp::get_data(int s, int buffer_s = 1024) {
    char buffer[buffer_s];
    std::string response;

    while (true) {
        int bytesReceived = recv(s, buffer, sizeof(buffer), 0);
        if (bytesReceived == -1) {
            return "";
        } else if (bytesReceived == 0) {
            // Server closed the connection
            break;
        }

        // Append the received data to the response
        response.append(buffer, bytesReceived);

        // Check if the end of headers is reached (double CRLF: \r\n\r\n)
        size_t headerEnd = response.find("\r\n\r\n");
        if (headerEnd != std::string::npos) {
            // Headers are complete
            break;
        }
    }

    // read until content length
    size_t contentLengthPos = response.find("Content-Length: ");
    
    //    std::cout << response << std::endl;
    if (contentLengthPos == std::string::npos) {
      std::cout << "Read only headers!\n" << response;
      return response;
    }

    size_t contentLengthEnd = response.find("\r\n", contentLengthPos);
    std::string contentLengthStr = response.substr(
        contentLengthPos + 16, contentLengthEnd - (contentLengthPos + 16));
    int contentLength = std::stoi(contentLengthStr);

    std::cout << contentLength << std::endl;

    size_t bodyStart = response.find("\r\n\r\n") + 4; // Start of the body
    while (response.size() - bodyStart < contentLength) {
        int bytesReceived = recv(s, buffer, sizeof(buffer), 0);
        if (bytesReceived == -1) {
            return "";
        } else if (bytesReceived == 0) {
            break;
        }
        response.append(buffer, bytesReceived);
    }

    return response;
}

std::string socketp::get_GET_data(std::string response){
  std::string regex_p = R"(GET\s+([^\s?]+(?:\?[^\s]*)?))";
  std::smatch match;
  std::regex rgx(regex_p);

  if(!std::regex_search(response, match, rgx))
    return "";

  return match[1].str();
}
// std::string socketp::get_Referer_data(std::string response){
//   std::string regex_p = R"(GET\s+([^\s?]+(?:\?[^\s]*)?))";
//   std::smatch match;
//   std::regex rgx(regex_p);

//   if(!std::regex_search(response, match, rgx))
//     return "";

//   return match[1].str();
// }
