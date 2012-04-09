/*
 * use_threads.c
 *
 *  Created on: Apr 7, 2012
 *      Author: chenming
 */
#include "server.h"
#include <pthread.h>
#include "list.h"

pthread_t *tid;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

static LIST list;
static CACHE cache;

void enqueue(int sock)
{
	pthread_mutex_lock(&lock);
	push_back(&list, sock);
	pthread_cond_signal(&cond);
	pthread_mutex_unlock(&lock);
}

int dequeue()
{
	int sock = -1;

	pthread_mutex_lock(&lock);
	while (list.head == NULL)
		pthread_cond_wait(&cond, &lock);
	sock = pop_front(&list);
	pthread_mutex_unlock(&lock);

	return sock;
}

static int get_host_name(char* buf,char *hostname, int length)
{

        char *p=strstr(buf,"Host: ");
        int i,j = 0;
        if(!p) {
                p=strstr(buf,"host: ");
        }
        bzero(hostname,256);
        for(i = (p-buf) + 6, j = 0; i < length; i++, j++)
        {
                if(buf[i] =='\r') {
                        hostname[j] ='\0';
                        return 0;
                }
                else
                        hostname[j] = buf[i];
        }
        return -1;
}

void *process_one_http_request(void *connectionfd)
{
	char buf[MAXCHAR];
	char method[MAXLINE], url[MAXLINE], protocol[MAXLINE];
	char filename[MAXCHAR];
	size_t n;
	int connfd;
	connfd = *((int *) connectionfd);

	printf("Connected by Client\n");

	if( (n = readline(connfd, buf, MAXCHAR)) < 0 )
	{
		perror("No data has been read!\n");
		goto Exit;
	}

	print(buf);
	char webserverAddr[256];
	get_host_name(buf, webserverAddr, MAXLINE);

	printf("webserver address is: %s:", webserverAddr);
	struct sockaddr_in webserver_addr;
	int sfd;
	memset(&webserver_addr, 0, sizeof(webserver_addr));
	webserver_addr.sin_family = AF_INET;
	webserver_addr.sin_port = htons(80);
	webserver_addr.sin_addr.s_addr = inet_addr(webserverAddr);
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sfd < 0)
	{
			perror("cannot create socket");
			goto Exit;
	}
	if (connect(sfd, (SA *) &webserver_addr, sizeof(webserver_addr)) < 0)
	{
			perror("cannot connect to webserver");
			goto Exit;
	}
	writen(sfd, buf, strlen(buf) + 1);

	while ( (n = read(sfd, buf, MAXCHAR)) > 0) {
			writen(connfd, buf, n);
	}

Exit:
	close(connfd);
	close(sfd);
	printf("Connection closed.\n");
	return(NULL);
}

void* thread_main(void *arg)
{
	while (1) {
		int sock = dequeue();
		if (sock != -1)
			process_one_http_request(&sock);
	}
	return(NULL);
}

void init_thread_pool(int n)
{
	tid = (pthread_t*) malloc(n*sizeof(pthread_t));
	int i;
	init_list(&list);
	for (i = 0; i < n; i++) {
		if (pthread_create(tid+n, NULL, thread_main, NULL))
			err_sys("pthread_create()");
	}
}

void process_one_request(int listenfd, int connfd)
{
//  pthread_t  tid;
//  if (pthread_create(&tid, NULL, process_one_http_request, &connfd)) {
//    err_sys("pthread_creat()");
//  }
	enqueue(connfd);
}

