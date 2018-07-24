﻿
// Secure Server Client using OpenSSL in C
// http://simplestcodings.blogspot.com/2010/08/secure-server-client-using-openssl-in-c.html

// SSL/TLS 握手过程详解
// https://www.jianshu.com/p/7158568e4867


#include "openssl/ssl.h"
#include "openssl/err.h"

#include <stdio.h>

#include <winsock2.h>

SSL_CTX* InitServerCTX(void)
{
  const SSL_METHOD *method;
  SSL_CTX *ctx;

  OpenSSL_add_all_algorithms();  // load & register all cryptos, etc.
  SSL_load_error_strings();  // load all error messages
  method = TLSv1_2_server_method();  // create new server-method instance
  ctx = SSL_CTX_new(method); // create new context from method
  if (ctx == NULL)
  {
    ERR_print_errors_fp(stderr);
    abort();
  }
  return ctx;
}

void LoadCertificates(SSL_CTX* ctx, const char* CertFile, const char* KeyFile)
{
  // set the local certificate from CertFile
  if (SSL_CTX_use_certificate_file(ctx, CertFile, SSL_FILETYPE_PEM) <= 0)
  {
    ERR_print_errors_fp(stderr);
    abort();
  }
  
  // set the private key from KeyFile (may be the same as CertFile)
  if (SSL_CTX_use_PrivateKey_file(ctx, KeyFile, SSL_FILETYPE_PEM) <= 0)
  {
    ERR_print_errors_fp(stderr);
    abort();
  }

  // verify private key
  if (!SSL_CTX_check_private_key(ctx))
  {
    fprintf(stderr, "Private key does not match the public certificate\n");
    abort();
  }
}

void ShowCerts(SSL* ssl)
{
  X509 *cert;
  char *line;

  cert = SSL_get_peer_certificate(ssl); /* Get certificates (if available) */
  if (cert != NULL)
  {
    printf("Server certificates:\n");
    line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
    printf("Subject: %s\n", line);
    free(line);
    line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
    printf("Issuer: %s\n", line);
    free(line);
    X509_free(cert);
  }
  else
    printf("No certificates.\n");
}

void Servlet(SSL* ssl) /* Serve the connection -- threadable */
{
  char buf[1024];
  char reply[1024];
  int sd, bytes;
  const char* HTMLecho = "<html><body><pre>%s</pre></body></html>\n\n";

  if (SSL_accept(ssl) == -1)     /* do SSL-protocol accept */
    ERR_print_errors_fp(stderr);
  else
  {
    ShowCerts(ssl);        /* get any certificates */
    bytes = SSL_read(ssl, buf, sizeof(buf)); /* get request */
    if (bytes > 0)
    {
      buf[bytes] = 0;
      printf("Client msg: \"%s\"\n", buf);
      sprintf(reply, HTMLecho, buf);   /* construct reply */
      SSL_write(ssl, reply, strlen(reply)); /* send reply */
    }
    else
      ERR_print_errors_fp(stderr);
  }
  sd = SSL_get_fd(ssl);       /* get socket connection */
  SSL_free(ssl);         /* release SSL state */
  closesocket(sd);          /* close connection */
}


SOCKET OpenListener(int port)
{
  SOCKET serversoc;
  SOCKADDR_IN serveraddr;

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

  return serversoc;
}


int main()
{
  SOCKET serversoc;
  SOCKET clientsoc;

  SOCKADDR_IN clientaddr;
  char buf[1024];
  char sendbuf[] = "hello client";
  int len;
  char reply[1024];
  WSADATA wsa;
  WSAStartup(MAKEWORD(2, 0), &wsa);	//初始化WS2_32.DLL

  SSL *ssl;
  SSL_CTX* ctx;
  SSL_library_init();
    
  ctx = InitServerCTX();  // initialize SSL
  LoadCertificates(ctx, "../certs/mycert.pem", "../certs/mycert.pem");  // load certs
  serversoc = OpenListener(-1);

  len = sizeof(SOCKADDR_IN);

  //接收请求
  if ((clientsoc = accept(serversoc, (SOCKADDR *)&clientaddr, &len)) <= 0)
  {
    printf("接受连接失败!\n");
    return -1;
  }
  printf("连接成功\n");
  printf("Connection: %s:%d\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
  ssl = SSL_new(ctx);              /* get new SSL state with context */
  SSL_set_fd(ssl, clientsoc);      /* set connection socket to SSL state */

  if (SSL_accept(ssl) == -1)     /* do SSL-protocol accept */
    ERR_print_errors_fp(stderr);
  else
  {
    ShowCerts(ssl);        /* get any certificates */
    int bytes = SSL_read(ssl, buf, sizeof(buf)); /* get request */
    if (bytes > 0)
    {
      buf[bytes] = 0;
      printf("Client msg: \"%s\"\n", buf);
      //sprintf(reply, HTMLecho, buf);   /* construct reply */
      SSL_write(ssl, sendbuf, strlen(sendbuf)); /* send reply */
    }
    else
      ERR_print_errors_fp(stderr);
  }
  int sd = SSL_get_fd(ssl);       /* get socket connection */
  SSL_free(ssl);         /* release SSL state */
  closesocket(sd);          /* close connection */

  WSACleanup();     //释放WS2_32.DLL
  return 0;
}
