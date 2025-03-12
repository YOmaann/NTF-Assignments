#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/socket.h>
#include <cstring>

using namespace std;

int main(int argc, char *argv[]) {
  if(argc == 1) return 0;

  // Inputting IP address of the server as an argument
  // ./client 172.12.7.1
  
  int client_s = socket(AF_INET, SOCK_STREAM, 0);
  
  sockaddr_in server;
  server.sin_family = AF_INET;
  server.sin_port = htons(8080);
  server.sin_addr.s_addr = inet_addr(argv[1]);

  string username;
  int in;

  do {
    cout << "Input username>>";
    cin >> username;
    cout << "Input number to send>>";
    // cin.ignore(256, '\n');
    //  getline(cin, in, '\n');
    cin >> in;
    cout <<  "Message to send : " << in << endl;
  } while(username.length() > 1024);

  // a char is precisely 1 bit.  
  connect(client_s, (struct sockaddr*) &server, sizeof(server));

  string to_send = "<" + to_string(in) + ":" + username + ">";

  // since send takes char* as argument so converting string to char*
  const char* msg = to_send.c_str();

  send(client_s, msg, strlen(msg), 0);
  //    close(client_s);
    //return 0;

  // receive message from server
  // int cs = accept(client_s, 0, 0);
  
  char buffer[1024] = {0};
  recv(client_s, buffer, sizeof(buffer), 0);

  cout << "Received sum = " << buffer << endl;
  
  close(client_s);

  return 0;
}
