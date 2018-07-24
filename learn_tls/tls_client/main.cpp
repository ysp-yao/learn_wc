
#include "openssl/ssl.h"
#include "openssl/err.h"
#include <winsock2.h>
#include <stdio.h>


SSL_CTX* InitCTX(void)
{
  const SSL_METHOD *method;
  SSL_CTX *ctx;

  OpenSSL_add_all_algorithms();  /* Load cryptos, et.al. */
  SSL_load_error_strings();   /* Bring in and register error messages */
  method = TLSv1_2_client_method();  /* Create new client-method instance */
  ctx = SSL_CTX_new(method);   /* Create new context */
  if (ctx == NULL)
  {
    ERR_print_errors_fp(stderr);
    abort();
  }
  return ctx;
}

void ShowCerts(SSL* ssl)
{
  X509 *cert;
  char *line;

  cert = SSL_get_peer_certificate(ssl); /* get the server's certificate */
  if (cert != NULL)
  {
    printf("Server certificates:\n");
    line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
    printf("Subject: %s\n", line);
    //free(line);       /* free the malloc'ed string */
    line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
    printf("Issuer: %s\n", line);
    //free(line);       /* free the malloc'ed string */
    X509_free(cert);     /* free the malloc'ed certificate copy */
  }
  else
    printf("Info: No client certificates configured.\n");
}




SOCKET OpenConnection(const char *hostname, int port)
{
  SOCKADDR_IN serveraddr;
  SOCKET clientsocket;

  //创建套接字
  if ((clientsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) <= 0)
  {
    printf("套接字socket创建失败!\n");
    return -1;
  }

  serveraddr.sin_family = AF_INET;
  serveraddr.sin_port = htons(9102);
  serveraddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

  //请求连接
  printf("尝试连接中...\n");
  if (connect(clientsocket, (SOCKADDR *)&serveraddr, sizeof(serveraddr)) != 0)
  {
    printf("连接失败!\n");
    return -1;
  }
  printf("连接成功!\n");

  return clientsocket;
}


int main()
{
  SOCKET clientsocket;

  char recvbuf[1024];

  WSADATA wsa;
  WSAStartup(MAKEWORD(2, 0), &wsa);	//初始化WS2_32.DLL

  SSL *ssl;
  SSL_CTX *ctx;
  SSL_library_init();
  ctx = InitCTX();
  clientsocket = OpenConnection(nullptr, 0);

  ssl = SSL_new(ctx);      /* create new SSL connection state */
  SSL_set_fd(ssl, clientsocket);    /* attach the socket descriptor */
  if (SSL_connect(ssl) == -1)   /* perform the connection */
    ERR_print_errors_fp(stderr);
  else
  {
    char msg[] = "Hello server";
  
    printf("Connected with %s encryption\n", SSL_get_cipher(ssl));
    ShowCerts(ssl);        /* get any certs */
    SSL_write(ssl, msg, strlen(msg));   /* encrypt & send message */
    int bytes = SSL_read(ssl, recvbuf, sizeof(recvbuf)); /* get reply & decrypt */
    recvbuf[bytes] = 0;
    printf("Received: \"%s\"\n", recvbuf);
    SSL_free(ssl);        /* release connection state */
  }

    //关闭套接字
  closesocket(clientsocket);
  WSACleanup();    //释放WS2_32.DLL
  return 0;
}
