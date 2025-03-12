#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <regex>

using namespace std;


void ex(int s) {
  close(s);
  cout << "exiting server.." << endl;
  exit(0);
}
pair<int,string>* clean(char _buffer[2048]) {
  string buffer = string(_buffer);
  regex rgx(R"(^<(\d+):([^:<>]+)>$)");
  smatch match;

  if(!regex_search(buffer, match, rgx)) {
    return NULL;
  }
  int t1 = stoi(match[1].str());
  string t2 = match[2].str();

  return new pair<int,string>(t1, t2);
}
int main() {
  // server name
  string sname = "Lucky";

  cout << "Welcome to " << sname << "'s server." << endl;
    int s = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(8080);
  addr.sin_addr.s_addr = INADDR_ANY;

  bind(s, (struct sockaddr*) &addr, sizeof(addr));

  listen(s, 5);

  signal(SIGINT, ex);

  int s_num, cs;
  pair<int, string> *cleaned;

  while(true) {
   s_num = rand() % 100 + 1;
   cs = accept(s, nullptr, nullptr);
   char buffer[2048] = {0};
   recv(cs, buffer, sizeof(buffer), 0);
   cleaned = clean(buffer);
   if(cleaned == NULL) {
     cout << "Invalid request!" << endl;
   continue;
     }

   int sum = s_num + cleaned->first;

   cout << "Client name: " << cleaned->second << endl;
   cout << "Client's number : " << cleaned->first << endl;

   if(cleaned->first > 100) {
     const char* error = "Out of range";
     send(cs, error, strlen(error), 0);
     cout << "Out of range!" << endl;
     break;
   }

   cout << "Server number : " << s_num << endl;
   cout << "Sum = " << sum << endl;

   string tmp = to_string(sum);
   const char* msg = tmp.c_str();

   send(cs, msg, strlen(msg), 0);
   cout << "Sent sum to client."<< endl;
   cout << "---------" << endl;
  }
  cout << "Closing server !" << endl;

  close(s);
  return 0;
}