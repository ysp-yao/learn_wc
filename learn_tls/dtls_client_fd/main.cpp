#include "openssl/ssl.h"
//#include <openssl/bio.h>
#include "openssl/err.h"

#include <stdio.h>

#include <winsock2.h>

class WinSocket {
public:
  WinSocket() {
    WSAStartup(MAKEWORD(2, 0), &wsa_);	//≥ı ºªØWS2_32.DLL
  }
  ~WinSocket() {
    WSACleanup();     // Õ∑≈WS2_32.DLL
  }
private:
  WSADATA wsa_;
};


int main() {
  WinSocket win_socket;

  SOCKET my_fd, client_fd;
  struct sockaddr_in server, client;

  my_fd = socket(PF_INET, SOCK_DGRAM, 0);


  server.sin_family = AF_INET;
  server.sin_port = htons(9102);
  server.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");


  char sendbuf[] = "hello server!";
  char recvbuf[1024];
  memset(recvbuf, 0, 1024);

  int len = sizeof(server);
  sendto(my_fd, sendbuf, strlen(sendbuf), 0, (SOCKADDR*)&server, sizeof(server));
  recvfrom(my_fd, recvbuf, 1024, 0, (SOCKADDR*)&server, &len);
  printf("recv:%s", recvbuf);


  return 0;
}