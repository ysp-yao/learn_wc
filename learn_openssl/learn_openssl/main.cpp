#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/ssl.h>

#include <thread>

#include <stdio.h>






void test_bio_mem()
{
  printf("****test_bio_mem****\n");
  BIO *b = NULL;
  int len = 0;
  char *out = NULL;
  b = BIO_new(BIO_s_mem());
  len = BIO_write(b, "openssl", 4); 
  printf("len=%d\n", len);
  len = BIO_printf(b, "%s", "zcp"); 
  printf("len=%d\n", len);
  len = BIO_ctrl_pending(b); 
  printf("len=%d\n", len);
  out = (char *)OPENSSL_malloc(len);
  len = BIO_read(b, out, len); // out=="openzcp"
  printf("len=%s\n", out);
  OPENSSL_free(out);
  BIO_free(b);
}

void test_bio_file()
{
  printf("****test_bio_file****\n");
  BIO *b = NULL;
  int len = 0, outlen = 0;
  char *out = NULL;
  b = BIO_new_file("bf.txt", "w");
  len = BIO_write(b, "openssl", 4);
  len = BIO_printf(b, "%s", "zcp");
  BIO_free(b);
  b = BIO_new_file("bf.txt", "r");
  len = BIO_pending(b);
  len = 50;
  out = (char *)OPENSSL_malloc(len);
  len = 1;
  while (len>0)
  {
    len = BIO_read(b, out + outlen, 1);
    printf("len=%d, out=%s\n", len, out);
    outlen += len;
  }
  OPENSSL_free(out);
  BIO_free(b);
}

void test_bio_socket_server()
{
  BIO *b = NULL, *c = NULL;
  int sock, ret, len;
  char *addr = NULL;
  char port[] = "2323";
  sock = BIO_get_accept_socket(port, 0);
  b = BIO_new_socket(sock, BIO_NOCLOSE);
  ret = BIO_accept(sock, &addr);
  BIO_set_fd(b, ret, BIO_NOCLOSE);
  while (1)
  {
    char out[80] = { 0 };
    len = BIO_read(b, out, 80);
    if (out[0] == 'q')
      break;
    printf("%s", out);
  }
  BIO_free(b);
}

void test_bio_socket_client()
{
  BIO *cbio, *out;
  int len;
  char tmpbuf[1024];
  cbio = BIO_new_connect("127.0.0.1:2323");
  //out = BIO_new_fp(stdout, BIO_NOCLOSE);
  if (BIO_do_connect(cbio) <= 0)
  {
    fprintf(stderr, "Error connecting to server\n");
  }
  BIO_puts(cbio, "GET / HTTP/1.0\n\n");
  for (;;)
  {
    len = BIO_read(cbio, tmpbuf, 1024);
    if (len <= 0) break;
//    BIO_write(out, tmpbuf, len);
  }
  BIO_free(cbio);
//  BIO_free(out);
}


void test_bio_socket()
{
  printf("****test_bio_socket****\n");
  std::thread t(test_bio_socket_server);
  std::this_thread::sleep_for(std::chrono::seconds(2));
  test_bio_socket_client();
}

void test_bio_md()
{
  BIO *bmd = NULL, *b = NULL;
  const EVP_MD *md = EVP_md5();
  int len;
  char tmp[1024];
  bmd = BIO_new(BIO_f_md());
  BIO_set_md(bmd, md);
  b = BIO_new(BIO_s_null());
  b = BIO_push(bmd, b);
  len = BIO_write(b, "openssl", 7);
  len = BIO_gets(b, tmp, 1024);
  BIO_free(b);
}

void test_bio_cipher()
{
  BIO *bc = NULL, *b = NULL;
  const EVP_CIPHER *c = EVP_des_ecb();
  int len, i;
  char tmp[1024] = {0};

  unsigned char key[8], iv[8];
  for (i = 0; i<8; i++)
  {
    memset(&key[i], i + 1, 1);
    memset(&iv[i], i + 1, 1);
  }
  bc = BIO_new(BIO_f_cipher());
  BIO_set_cipher(bc, c, key, iv, 1);
  b = BIO_new(BIO_s_null());
  b = BIO_push(bc, b);
  len = BIO_write(b, "openssl", 7);
  len = BIO_read(b, tmp, 1024);
  BIO_free(b);
  /* ½âÃÜ */
  BIO *bdec = NULL, *bd = NULL;
  const EVP_CIPHER *cd = EVP_des_ecb();
  bdec = BIO_new(BIO_f_cipher());
  BIO_set_cipher(bdec, cd, key, iv, 0);
  bd = BIO_new(BIO_s_null());
  bd = BIO_push(bdec, bd);
  len = BIO_write(bdec, tmp, len);
  len = BIO_read(bdec, tmp, 1024);
  BIO_free(bdec);
}

void test_bio_ssl()
{
  BIO *sbio, *out;
  int len;
  char tmpbuf[1024];
  SSL_CTX *ctx;
  SSL *ssl;
  SSLeay_add_ssl_algorithms();
  OpenSSL_add_all_algorithms();
  ctx = SSL_CTX_new(SSLv23_client_method());
  sbio = BIO_new_ssl_connect(ctx);
  BIO_get_ssl(sbio, &ssl);
  if (!ssl)
  {
    fprintf(stderr, "Can not locate SSL pointer\n");
    return;
  }
  SSL_set_mode(ssl, SSL_MODE_AUTO_RETRY);
  BIO_set_conn_hostname(sbio, "mybank.icbc.com.cn:https");
  out = BIO_new_fp(stdout, BIO_NOCLOSE);
  BIO_printf(out, "Á´½ÓÖÐ¡­.\n");
  if (BIO_do_connect(sbio) <= 0)
  {
    fprintf(stderr, "Error connecting to server\n");
    return;
  }
  if (BIO_do_handshake(sbio) <= 0)
  {
    fprintf(stderr, "Error establishing SSL connection\n");
    return;
  }
  BIO_puts(sbio, "GET / HTTP/1.0\n\n");
  for (;;)
  {
    len = BIO_read(sbio, tmpbuf, 1024);
    if (len <= 0) break;
    BIO_write(out, tmpbuf, len);
  }
  BIO_free_all(sbio);
  BIO_free(out);
}


int main()
{

#if OPENSSL_VERSION_NUMBER < 0x10100000L
  printf(" error ... OpenSSL pre-1.1.0\n");
#else
  printf("OpenSSL >= 1.1.0\n");
#endif

  test_bio_mem();
  printf("\n");
  test_bio_file();
  printf("\n");
  test_bio_socket();
  printf("\n");
  test_bio_md();
  printf("\n");
  test_bio_cipher();
  printf("\n");
  test_bio_ssl();

  return 0;
}