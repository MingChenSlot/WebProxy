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
	char buf[MAXCHAR + 1];
	size_t n;
	int connfd;
	connfd = *((int *) connectionfd);

	printf("Connected by Client\n");

	if( (n = read(connfd, buf, MAXCHAR)) < 0 )
		err_sys("No data has been read!\n");

	print(buf);

	if( writen(connfd, buf, n) < 0)
		err_sys("Can't send msg to clinet!\n");

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

