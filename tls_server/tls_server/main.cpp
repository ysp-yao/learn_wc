
//
// http://simplestcodings.blogspot.com/2010/08/secure-server-client-using-openssl-in-c.html
//



//#include "openssl/ssl.h"
//#include "openssl/err.h"
//
//#include <winsock2.h>
//
//SSL_CTX* InitServerCTX(void)
//{
//  const SSL_METHOD *method;
//  SSL_CTX *ctx;
//
//  OpenSSL_add_all_algorithms();  // load & register all cryptos, etc.
//  SSL_load_error_strings();  // load all error messages
//  method = TLSv1_2_server_method();  // create new server-method instance
//  ctx = SSL_CTX_new(method); // create new context from method
//  if (ctx == NULL)
//  {
//    ERR_print_errors_fp(stderr);
//    abort();
//  }
//  return ctx;
//}
//
//void LoadCertificates(SSL_CTX* ctx, const char* CertFile, const char* KeyFile)
//{
//  // set the local certificate from CertFile
//  if (SSL_CTX_use_certificate_file(ctx, CertFile, SSL_FILETYPE_PEM) <= 0)
//  {
//    ERR_print_errors_fp(stderr);
//    abort();
//  }
//  
//  // set the private key from KeyFile (may be the same as CertFile)
//  if (SSL_CTX_use_PrivateKey_file(ctx, KeyFile, SSL_FILETYPE_PEM) <= 0)
//  {
//    ERR_print_errors_fp(stderr);
//    abort();
//  }
//
//  // verify private key
//  if (!SSL_CTX_check_private_key(ctx))
//  {
//    fprintf(stderr, "Private key does not match the public certificate\n");
//    abort();
//  }
//}
//
//int OpenListener(int port)
//{
//  int sd;
//  struct sockaddr_in addr;
//
//  sd = socket(PF_INET, SOCK_STREAM, 0);
//  memset(&addr,0, sizeof(addr));
//  addr.sin_family = AF_INET;
//  addr.sin_port = htons(port);
//  addr.sin_addr.s_addr = INADDR_ANY;
//  if (bind(sd, (struct sockaddr*)&addr, sizeof(addr)) != 0)
//  {
//    perror("can't bind port");
//    abort();
//  }
//  if (listen(sd, 10) != 0)
//  {
//    perror("Can't configure listening port");
//    abort();
//  }
//  return sd;
//}
//
//void ShowCerts(SSL* ssl)
//{
//  X509 *cert;
//  char *line;
//
//  cert = SSL_get_peer_certificate(ssl); /* Get certificates (if available) */
//  if (cert != NULL)
//  {
//    printf("Server certificates:\n");
//    line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
//    printf("Subject: %s\n", line);
//    free(line);
//    line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
//    printf("Issuer: %s\n", line);
//    free(line);
//    X509_free(cert);
//  }
//  else
//    printf("No certificates.\n");
//}
//
//void Servlet(SSL* ssl) /* Serve the connection -- threadable */
//{
//  char buf[1024];
//  char reply[1024];
//  int sd, bytes;
//  const char* HTMLecho = "<html><body><pre>%s</pre></body></html>\n\n";
//
//  if (SSL_accept(ssl) == -1)     /* do SSL-protocol accept */
//    ERR_print_errors_fp(stderr);
//  else
//  {
//    ShowCerts(ssl);        /* get any certificates */
//    bytes = SSL_read(ssl, buf, sizeof(buf)); /* get request */
//    if (bytes > 0)
//    {
//      buf[bytes] = 0;
//      printf("Client msg: \"%s\"\n", buf);
//      sprintf(reply, HTMLecho, buf);   /* construct reply */
//      SSL_write(ssl, reply, strlen(reply)); /* send reply */
//    }
//    else
//      ERR_print_errors_fp(stderr);
//  }
//  sd = SSL_get_fd(ssl);       /* get socket connection */
//  SSL_free(ssl);         /* release SSL state */
//  closesocket(sd);          /* close connection */
//}
//
//int main(int argc, char *argv) {
//
//  WSADATA wsaData;
//  WSAStartup(MAKEWORD(2, 2), &wsaData);
//
//  int port = 8888;
//  int server;
//  SSL_CTX* ctx;
//  SSL_library_init();
//
//  ctx = InitServerCTX();  // initialize SSL
//  LoadCertificates(ctx, "../certs/mycert.pem", "../certs/mycert.pem");  // load certs
//  server = OpenListener(port); // create server socket
//
//  while (1) {
//    struct sockaddr_in addr;
//    int len = sizeof(addr);
//    SSL *ssl;
//
//    int client = accept(server, (struct sockaddr*)&addr, &len);  /* accept connection as usual */
//    printf("Connection: %s:%d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
//    ssl = SSL_new(ctx);              /* get new SSL state with context */
//    SSL_set_fd(ssl, client);      /* set connection socket to SSL state */
//    Servlet(ssl);         /* service connection */
//  }
//
//  closesocket(server);          /* close server socket */
//  SSL_CTX_free(ctx);         /* release context */
//
//  return 0;
//}


#include <winsock2.h>
#include <stdio.h>


int main()
{
  SOCKET serversoc;
  SOCKET clientsoc;
  SOCKADDR_IN serveraddr;
  SOCKADDR_IN clientaddr;
  char buf[1024];
  int len;

  WSADATA wsa;
  WSAStartup(MAKEWORD(2, 0), &wsa);	//初始化WS2_32.DLL

                                    //创建套接字
  if ((serversoc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) <= 0)
  {
    printf("套接字socket创建失败!\n");
    return -1;
  }

  //命名协议，IP，端口
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_port = htons(9102);
  serveraddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);

  //绑定套接字
  if (bind(serversoc, (SOCKADDR *)&serveraddr, sizeof(serveraddr)) != 0)
  {
    printf("套接字绑定失败!\n");
    return -1;
  }

  printf("开始监听...\n");
  //监听请求
  if (listen(serversoc, 1) != 0)
  {
    printf("监听失败!\n");
    return -1;
  }

  len = sizeof(SOCKADDR_IN);

  //接收请求
  if ((clientsoc = accept(serversoc, (SOCKADDR *)&clientaddr, &len)) <= 0)
  {
    printf("接受连接失败!\n");
    return -1;
  }
  printf("连接成功\n");

  if (recv(clientsoc, buf, 1024, 0) <= 0)
  {
    printf("关闭连接!\n");
    closesocket(clientsoc);
  }
  printf("接收来自客户端的信息: %s\n", buf);
  if (send(clientsoc, buf, strlen(buf) + 1, 0) <= 0)
  {
    printf("发送错误!\n");
  }

  WSACleanup();     //释放WS2_32.DLL
  return 0;
}
