
// SSL/TLS 握手过程详解
// https://www.jianshu.com/p/7158568e4867

// chapter 10, The Definitive Guide To Linux Network Programming

#include "openssl/ssl.h"
#include "openssl/err.h"
#include <winsock2.h>
#include <stdio.h>

class WinSocket {
public:
  WinSocket() {
    WSAStartup(MAKEWORD(2, 0), &wsa_);	//初始化WS2_32.DLL
  }
  ~WinSocket() {
    WSACleanup();     //释放WS2_32.DLL
  }
private:
  WSADATA wsa_;
};


int main() {

  WinSocket win_socket;

  // my_ssl_method->my_ssl_ctx->my_ssl->my_fd

  const SSL_METHOD *my_ssl_method;
  SSL_CTX *my_ssl_ctx;
  SSL *my_ssl;
  SOCKET my_fd;
  struct sockaddr_in server;
  int error = 0, read_in = 0;
  char buffer[512];

  OpenSSL_add_all_algorithms();
  SSL_load_error_strings();

  my_ssl_method = TLSv1_2_client_method();

  if ((my_ssl_ctx = SSL_CTX_new(my_ssl_method)) == NULL) {
    ERR_print_errors_fp(stderr);
    exit(-1);
  }

  if ((my_ssl = SSL_new(my_ssl_ctx)) == NULL) {
    ERR_print_errors_fp(stderr);
    exit(-1);
  }

  //创建套接字
  if ((my_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) <= 0)
  {
    printf("套接字socket创建失败!\n");
    return -1;
  }

  server.sin_family = AF_INET;
  server.sin_port = htons(9102);
  server.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

  //请求连接
  printf("尝试连接中...\n");
  if (connect(my_fd, (SOCKADDR *)&server, sizeof(server)) != 0)
  {
    printf("连接失败!\n");
    return -1;
  }
  printf("连接成功!\n");

  SSL_set_fd(my_ssl, my_fd);
  if (SSL_connect(my_ssl) <= 0) {
    ERR_print_errors_fp(stderr);
    exit(-1);
  }
  printf("[%s,%s]\n", SSL_get_version(my_ssl), SSL_get_cipher(my_ssl));

  char sendbuf[] = "hello server!";
  char recvbuf[1024];
  SSL_write(my_ssl, sendbuf, strlen(sendbuf));   /* encrypt & send message */
  int bytes = SSL_read(my_ssl, recvbuf, sizeof(recvbuf)); /* get reply & decrypt */
  recvbuf[bytes] = 0;
  printf("Received: \"%s\"\n", recvbuf);


  SSL_shutdown(my_ssl);
  SSL_free(my_ssl);
  SSL_CTX_free(my_ssl_ctx);
  closesocket(my_fd);

  return 0;
}