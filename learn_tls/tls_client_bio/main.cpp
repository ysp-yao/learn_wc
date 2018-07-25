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

  const SSL_METHOD *my_ssl_method;
  SSL_CTX *my_ssl_ctx;
  SSL *my_ssl;
  BIO *my_bio;
  int error = 0, read_in = 0;

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

  if ((my_bio = BIO_new_connect("127.0.0.1:9102")) == NULL) {
    ERR_print_errors_fp(stderr);
    exit(-1);
  }

  if (BIO_do_connect(my_bio) <= 0) {
    ERR_print_errors_fp(stderr);
    exit(-1);
  }

  SSL_set_bio(my_ssl, my_bio, my_bio);

  if (SSL_connect(my_ssl) <= 0) {
    ERR_print_errors_fp(stderr);
    exit(-1);
  }

  printf("Connection made with [version,cipher]: [%s,%s]\n", SSL_get_version(my_ssl), SSL_get_cipher(my_ssl));

  char sendbuf[] = "hello server!";
  char recvbuf[1024];
  SSL_write(my_ssl, sendbuf, strlen(sendbuf));   /* encrypt & send message */
  int bytes = SSL_read(my_ssl, recvbuf, sizeof(recvbuf)); /* get reply & decrypt */
  recvbuf[bytes] = 0;
  printf("Received: \"%s\"\n", recvbuf);

  SSL_shutdown(my_ssl);
  SSL_free(my_ssl);
  SSL_CTX_free(my_ssl_ctx);

  return 0;
}