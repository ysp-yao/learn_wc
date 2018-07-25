
#include "openssl/ssl.h"
//#include <openssl/bio.h>
#include "openssl/err.h"

#include <stdio.h>

#include <winsock2.h>

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
  BIO *server_bio, *client_bio;

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


  if ((server_bio = BIO_new_accept("9102")) == NULL) {
    ERR_print_errors_fp(stderr);
    exit(-1);
  }

  if (BIO_do_accept(server_bio) <= 0) { // 相当于bind
    ERR_print_errors_fp(stderr);
    exit(-1);
  }

  if (BIO_do_accept(server_bio) <= 0) { // 相当于accept
    ERR_print_errors_fp(stderr);
    exit(-1);
  }

  client_bio = BIO_pop(server_bio);

  if ((my_ssl = SSL_new(my_ssl_ctx)) == NULL) {
    ERR_print_errors_fp(stderr);
    exit(-1);
  }
  SSL_set_bio(my_ssl, client_bio, client_bio);

  if (SSL_accept(my_ssl) <= 0) {
    ERR_print_errors_fp(stderr);
    exit(-1);
  }

  printf("Connection made with [version,cipher]: [%s,%s]\n", SSL_get_version(my_ssl), SSL_get_cipher(my_ssl));

  char buf[1024];
  char sendbuf[] = "hello client!";
  int bytes = SSL_read(my_ssl, buf, sizeof(buf)); /* get request */
  if (bytes > 0)
  {
    buf[bytes] = 0;
    printf("Client msg: \"%s\"\n", buf);
    SSL_write(my_ssl, sendbuf, strlen(sendbuf)); /* send reply */
  }


  //char buf[1024];
  //char sendbuf[] = "hello client!";
  //int bytes = BIO_read(client_bio, buf, sizeof(buf)); /* get request */
  //if (bytes > 0)
  //{
  //  buf[bytes] = 0;
  //  printf("Client msg: \"%s\"\n", buf);
  //  BIO_write(client_bio, sendbuf, strlen(sendbuf)); /* send reply */
  //}



  SSL_shutdown(my_ssl);
  SSL_free(my_ssl);

  SSL_CTX_free(my_ssl_ctx);
  BIO_free(server_bio);

  return 0;
}