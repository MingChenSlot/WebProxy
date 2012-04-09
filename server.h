/*
 * server.h
 *
 *  Created on: Apr 7, 2012
 *      Author: chenming
 */

#ifndef SERVER_H_
#define SERVER_H_

#include <sys/types.h>    /* for basic system types */
#include <sys/socket.h>   /* for basic socket definitions */
#include <netinet/in.h>   /* for sockaddr_in, network ordering */
#include <arpa/inet.h>    /* for inet(3) functions */
#include <signal.h>       /* for signal*/
#include <netdb.h>        /* for gethostbyname() */
#include <stdio.h>        /* for perror() */
#include <stdlib.h>
#include <errno.h>        /* for sys_errlist */
#include <unistd.h>
#include <sys/wait.h>     /* for waipidt() */
#include <fcntl.h>        /* for nonblicking */
#include <string.h>       /* for bzero() */
#include <time.h>
#include <sys/stat.h>

/* error msg */
#define err_sys(x) do { perror(x); exit(1); } while (0)

/* global variables */
#define SERVERPORT  12345
#define LISTENQSIZE 1024
#define MAXCHAR 4096*4
#define MAXLINE 2000
#define MAXBUF 1460
#define SA struct sockaddr

extern char wwwroot[];
extern short int cache_flag;

void process_one_request(int listenfd, int connectionfd);
void init_thread_pool(int n); // only used in use_threads.cc

/* Cache */
#define MAX_VALID_DATA_LEN 100000  //最大有效数据
#define MAX_BUF_LEN (MAX_VALID_DATA_LEN+1) //最大缓冲区大小


typedef struct _cache
{
	unsigned char buf[MAX_BUF_LEN];
	unsigned char current; //下一byte指针
	int loc[1000];
	int len[1000];
}CACHE;

/* Socket IO */

ssize_t readn(int fd, void *bufptr, size_t n);
ssize_t writen(int fd, const void *bufptr, size_t n);
void print(char *bufptr);
ssize_t readline(int fd, void *bufptr, size_t maxlen);
int snprintf_buf(char *write_buf, int *write_cnt, const char *fmt, ...);
int flush_buf(int fd, char *write_buf, int *write_cnt);

/* Parse HTTP */

const char *get_mime_type(char *filename);
const char *get_status_text(int status);

#endif /* SERVER_H_ */
