
#include "openssl/ssl.h"
//#include <openssl/bio.h>
#include "openssl/err.h"

#include <stdio.h>

#include <winsock2.h>

class WinSocket {
public:
  WinSocket() {
    WSAStartup(MAKEWORD(2, 0), &wsa_);	//��ʼ��WS2_32.DLL
  }
  ~WinSocket() {
    WSACleanup();     //�ͷ�WS2_32.DLL
  }
private:
  WSADATA wsa_;
};


int main() {
  WinSocket win_socket;

  SOCKET my_fd, client_fd;
  struct sockaddr_in server, client;
  //const SSL_METHOD *my_ssl_method;
  //SSL_CTX *my_ssl_ctx;
  //SSL *my_ssl;

  //OpenSSL_add_all_algorithms();
  //SSL_load_error_strings();

  //my_ssl_method = DTLS_server_method();

  //my_ssl_ctx = SSL_CTX_new(my_ssl_method);

  //SSL_CTX_use_certificate_file(my_ssl_ctx, "../../certs/server/server.pem", SSL_FILETYPE_PEM);
  //SSL_CTX_use_PrivateKey_file(my_ssl_ctx, "../../certs/server/server.pem", SSL_FILETYPE_PEM);
  //if (SSL_CTX_check_private_key(my_ssl_ctx)) {
  //  // The key works
  //}
  //else {
  //  // The key is wrong
  //  fprintf(stderr, "Private key does not match certificate\n");
  //}

  //�����׽���
  if ((my_fd = socket(AF_INET, SOCK_DGRAM, 0)) <= 0)
  {
    printf("�׽���socket����ʧ��!\n");
    return -1;
  }

  //����Э�飬IP���˿�
  server.sin_family = AF_INET;
  server.sin_port = htons(9102);
  server.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

  //���׽���
  if (bind(my_fd, (SOCKADDR *)&server, sizeof(server)) != 0)
  {
    printf("�׽��ְ�ʧ��!\n");
    return -1;
  }

  int len = sizeof(server);
  char recvbuf[1024];
  memset(recvbuf, 0, 1024);
  char sendbuf[] = "hello client!";
  int n = recvfrom(my_fd, recvbuf, 1024, 0, (SOCKADDR *)&client, &len);
  printf("recv: %s", recvbuf);
  sendto(my_fd, sendbuf, n, 0, (SOCKADDR *)&client, len);

  return 0;
}