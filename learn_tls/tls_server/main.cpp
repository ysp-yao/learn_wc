

// SSL/TLS 握手过程详解
// https://www.jianshu.com/p/7158568e4867

// chapter 10, The Definitive Guide To Linux Network Programming


#include "openssl/ssl.h"
#include "openssl/err.h"

#include <stdio.h>

#include <winsock2.h>

// my_ssl_method->my_ssl_ctx->my_fd->client_fd->my_ssl

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

  SOCKET my_fd, client_fd;
  struct sockaddr_in server, client;
  int client_size;
  int error = 0, wrote = 0;
  const SSL_METHOD *my_ssl_method;
  SSL_CTX *my_ssl_ctx;
  SSL *my_ssl;

  OpenSSL_add_all_algorithms();
  SSL_load_error_strings();

  my_ssl_method = TLSv1_2_server_method();

  if ((my_ssl_ctx = SSL_CTX_new(my_ssl_method)) == NULL) {
    ERR_print_errors_fp(stderr);
    exit(-1);
  }

  SSL_CTX_use_certificate_file(my_ssl_ctx, "../../certs/server/server.pem", SSL_FILETYPE_PEM);
  SSL_CTX_use_PrivateKey_file(my_ssl_ctx, "../../certs/server/server.pem", SSL_FILETYPE_PEM);
  if (SSL_CTX_check_private_key(my_ssl_ctx)) {
    // The key works
  }
  else {
    // The key is wrong
    fprintf(stderr, "Private key does not match certificate\n");
  }


  //创建套接字
  if ((my_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) <= 0)
  {
    printf("套接字socket创建失败!\n");
    return -1;
  }

  //命名协议，IP，端口
  server.sin_family = AF_INET;
  server.sin_port = htons(9102);
  server.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

  //绑定套接字
  if (bind(my_fd, (SOCKADDR *)&server, sizeof(server)) != 0)
  {
    printf("套接字绑定失败!\n");
    return -1;
  }

  if (listen(my_fd, 1) != 0)
  {
    printf("监听失败!\n");
    return -1;
  }

  int len = sizeof(SOCKADDR_IN);
  if ((client_fd = accept(my_fd, (SOCKADDR *)&client, &len)) <= 0)
  {
    printf("接受连接失败!\n");
    return -1;
  }
  printf("Connection: %s:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
  
  if ((my_ssl = SSL_new(my_ssl_ctx)) == nullptr) {              /* get new SSL state with context */

  }

  SSL_set_fd(my_ssl, client_fd);      /* set connection socket to SSL state */
  
  if (SSL_accept(my_ssl) <= 0)     /* do SSL-protocol accept */
    ERR_print_errors_fp(stderr);
  printf("[%s,%s]\n", SSL_get_version(my_ssl), SSL_get_cipher(my_ssl));

  char buf[1024];
  char sendbuf[] = "hello client!\n";
  int bytes = SSL_read(my_ssl, buf, sizeof(buf)); /* get request */
  if (bytes > 0)
  {
    buf[bytes] = 0;
    printf("Client msg: \"%s\"\n", buf);
    SSL_write(my_ssl, sendbuf, strlen(sendbuf)); /* send reply */
  }


  // my_ssl_method->my_ssl_ctx->my_fd->client_fd->my_ssl
  SSL_shutdown(my_ssl);
  SSL_free(my_ssl);
  closesocket(client_fd);
  SSL_CTX_free(my_ssl_ctx);

  return 0;
}