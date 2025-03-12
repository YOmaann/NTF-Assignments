#define PORT 2002

#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include "./socketp/socketp.h"
#include <strings.h>
#include <netdb.h>
//#include <signal>

using namespace std;

void exit_c(int s) {
  close(s);
  cout << "Closing Server.." << endl;
}

void handler(socketp sp, int con) {
  string out = sp.get_data(con, 1024);

  // cout << out << endl;
  // expect something (url to fetch) in GET data
  string get_d = sp.get_GET_data(out);
  
  string url_to_fetch = get_d.substr(1);

  cout << "Fetching : " << url_to_fetch << endl;
  // clean url
  urls u(url_to_fetch);

  // get IP address from hostname
  sockaddr_in to_fetch_addr;
  if(string(u.get_port()) == "")
    to_fetch_addr = sp.get_address(80, "127.0.0.1");
  else
    to_fetch_addr = sp.get_address(stoi(string(u.get_port())), "127.0.0.1");

  if(string(u.get_host()) != "localhost") {
    hostent* to_fetch_ip = gethostbyname(u.get_host());
    bcopy((char *)to_fetch_ip->h_addr, (char *)&to_fetch_addr.sin_addr.s_addr, to_fetch_ip->h_length);
  }
 
  // Check cache
  cache ch;
  if(ch.check_cache(url_to_fetch))
    {
    cout << "Cached!" << endl;
    vector<string> response = ch.get_cache(url_to_fetch);

    // close connections, return cache and exit
    string out = "";
    bool firstS = false;
    for(string res : response) {
      if(!firstS && res == "\r")
	{
	  firstS = true;
	  out = out + "\n\r\n";
	 
	}
      out += res + "\n";
    }

    send(con, out.c_str(),out.length(), 0);    
    sp.check(close(con), "Error while closing connecting", "Connection closed successfully");
    return;    
    }
  else
    cout << "Not cached!" << endl;

  // fetch the website in the GET data using socket programming only
  int clientS = socket(AF_INET, SOCK_STREAM, 0);

  // start a connection
  connect(clientS, (struct sockaddr*) &to_fetch_addr, sizeof(to_fetch_addr));

  // make request
  string request = "GET /" + string(u.get_path()) + " HTTP/1.1\r\nHost: " + string(u.get_host()) + "\r\nConnection: close\r\n\r\n";
  if(string(u.get_port()) != "")
    request = "GET /" + string(u.get_path()) + " HTTP/1.1\r\nHost: " + string(u.get_host()) + ":" + string(u.get_port()) + "\r\nConnection: close\r\n\r\n";

  if(send(clientS, request.c_str(), request.length(), 0) < 0) {
    cout << "Connection rejected by server!" << endl;
    
    close(clientS);
    close(con);
    return;
  }

  string response = sp.get_data(clientS, 1024);
  cout << "Response : " <<  response << endl;

  //  creating cache
  ch.create_cache(url_to_fetch, response);
  close(clientS);
  // there will be an error here since it might not send the entire file.
  send(con, response.c_str(),response.length(), 0);
  sp.check(close(con), "Error while closing connecting", "Connection closed successfully");
  return;
}

int main(int argc, char **argv) {
  //  signal(SIGNIT, exit_c)
  int port = PORT;
  if(argc > 1)
    port = stoi(argv[1]);
  socketp sp;
  int serverSoc;
  serverSoc = sp.check(socket(AF_INET, SOCK_STREAM, 0), "Cannot create socket!", "Socket opened successfully.");
  sockaddr_in client_addr = sp.get_address(port, INADDR_ANY);

  // Binding socket
  sp.check(bind(serverSoc, (struct sockaddr*) &client_addr, sizeof(client_addr)), "Binding Failed!", "Binding successful");
  
  // start listening at PORT
  sp.check(listen(serverSoc, 5), "Cannot listen at port " , "Listening at port ");

  // listening to connections
  while(true) {
    // event loop
    int con = accept(serverSoc, NULL, NULL);

    // create new process using FORK

    pid_t pid = fork();

    if(pid > 0) {
      close(con);
    }
    else if(pid == 0) {
      close(serverSoc);
      handler(sp, con);
      return 0;
    }
    else {
      return -1;
    }
  }
  sp.check(close(serverSoc), "Error while closing servers.", "Closed socket successfully.");
  
  return 0;
}
