#include "openssl/ssl.h"
//#include <openssl/bio.h>
#include "openssl/err.h"

#include <stdio.h>

#include <winsock2.h>

#define DTLS_CLIENT_DEFAULT_TIMEOUT 5
#define BUFFER_SIZE ( 1 << 16 )
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

int dtls_isAlive(SSL* ssl)
{
  return !(SSL_get_shutdown(ssl) & SSL_RECEIVED_SHUTDOWN);
}

int main() {

  union {
    struct sockaddr_storage ss;
    struct sockaddr_in s4;
  } server, client;

  WinSocket win_socket;
  BIO*     bio;
  const SSL_METHOD *my_ssl_method;
  SSL_CTX *my_ssl_ctx;
  SSL *my_ssl;
  SOCKET my_fd, client_fd;
  struct timeval timeout;

  timeout.tv_sec = DTLS_CLIENT_DEFAULT_TIMEOUT;
  timeout.tv_usec = 0;

  //struct sockaddr_in server, client;

  server.s4.sin_family = AF_INET;
  server.s4.sin_port = htons(23232);
  //server.s4.sin_addr.S_un.S_addr = inet_addr("172.16.16.194");
  server.s4.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

  my_fd = socket(AF_INET, SOCK_DGRAM, 0);

  my_ssl_ctx = SSL_CTX_new(DTLSv1_client_method());
  OpenSSL_add_ssl_algorithms();
  SSL_load_error_strings();

  SSL_CTX_use_certificate_file(my_ssl_ctx, "../dtls_certs/client-cert.pem", SSL_FILETYPE_PEM);
  SSL_CTX_use_PrivateKey_file(my_ssl_ctx, "../dtls_certs/client-key.pem", SSL_FILETYPE_PEM);
  if (SSL_CTX_check_private_key(my_ssl_ctx)) {
    // The key works
  }
  else {
    // The key is wrong
    fprintf(stderr, "Private key does not match certificate\n");
  }

  SSL_CTX_set_verify_depth(my_ssl_ctx, 2);
  SSL_CTX_set_read_ahead(my_ssl_ctx, 1);

  my_ssl = SSL_new(my_ssl_ctx);

  bio = BIO_new_dgram(my_fd, BIO_CLOSE);

  connect(my_fd, (struct sockaddr*)&server, sizeof(struct sockaddr_in));

  BIO_ctrl(bio, BIO_CTRL_DGRAM_SET_CONNECTED, 0, &(server.ss));

  SSL_set_bio(my_ssl, bio, bio);

  if (SSL_connect(my_ssl) < 0)
  {
  }


  BIO_ctrl(bio, BIO_CTRL_DGRAM_SET_RECV_TIMEOUT, 0, &timeout);

  X509* pX509 = SSL_get_peer_certificate(my_ssl);
  if (pX509)
  {
    printf("\n");
    X509_NAME_print_ex_fp(stdout, X509_get_subject_name(pX509),
      1, XN_FLAG_MULTILINE);
    printf("\n\n");

    X509_free(pX509);
  }

  char buffer[BUFFER_SIZE] = {};
  int  readlen;
  int  writelen;
  int  reading_flag = 0;
  int  i;

  for (i = 0; (i<1) && dtls_isAlive(my_ssl); i++)
  {
    sprintf(buffer, "message No. %d", i);
    writelen = SSL_write(my_ssl, buffer, strlen(buffer) + 1);
    printf("send : %s\n", buffer);
    memset(buffer, 0, BUFFER_SIZE);
    readlen = SSL_read(my_ssl, buffer, sizeof(buffer));
    printf("recv : %s\n", buffer);
  }

  closesocket(my_fd);
  SSL_free(my_ssl);
  SSL_CTX_free(my_ssl_ctx);

  ERR_remove_state(0);
  ERR_free_strings();
  EVP_cleanup();

  return 0;
}
