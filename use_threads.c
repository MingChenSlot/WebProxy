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
	char path[MAXCHAR + 1];
	if( sscanf(buf, "%[^ ] %[^ ] %[^ ]", method, url, protocol) != 3 )
	{
		perror("Can't parse msg");
		goto Exit;
	}
	sprintf(filename, "./%s", &url[1]);
	FILE *fp = fopen(filename, "r");
	if (fp == (FILE*) 0)
	{
		perror("Cannot open file");
		goto Exit;
	}

	sprintf(buf, "HTTP/1.0 %d%s\r\nContent-type: %s\r\n\r\n", 200, get_status_text(200), get_mime_type(&url));
	writen(connfd, buf, strlen(buf));

	while ( (n = fread(buf, 1, MAXCHAR, fp)) > 0) {
		writen(connfd, buf, n);
	}

	fclose(fp);
Exit:
	close(connfd);
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

