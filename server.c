/*
 * server.c
 *
 *  Created on: Apr 7, 2012
 *      Author: chenming
 */
#include "server.h"
#include "list.h"

uint16_t serverport;
uint16_t threads;
short int cache;

static void
usage(char *program)
{
	fprintf(stderr, "usage: %s [-l port] [-t threads] [-c]\n", program);
	exit(1);
}

static void
parse_args(int argc, char** argv)
{
	int n = 1;
	while (n < argc) {
		if (strcmp(argv[n], "-l") == 0) {
			serverport = atoi(argv[++n]);
		} else if (strcmp(argv[n], "-t") == 0) {
			threads = atoi(argv[++n]);
		} else if (strcmp(argv[n], "-c") == 0) {
			cache = 1;
		} else
		  usage(argv[0]);
		n++;
	}
}

int
main(int argc, char **argv)
{
	int listenfd, connfd;
	socklen_t clisocklen;
	struct sockaddr_in servaddr, cliaddr;
	int flag=1;	//reuse port

	parse_args(argc, argv);

//	printf("usage: server port: %d\nthreads number: %d\ncache: %d\n", serverport, threads, cache);

	if ( (listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		err_sys("socket error");

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(serverport);

	if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(int)) == -1)
		err_sys("setsockopt error");

	if (bind(listenfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
	    err_sys("bind error");

	if (listen(listenfd, LISTENQSIZE) < 0)
	    err_sys("listen error");

	printf("Listen to %s:%d\n", inet_ntoa(servaddr.sin_addr), serverport);
	init_thread_pool(threads);
	while(1)
	{
		clisocklen = sizeof(cliaddr);
		if ( (connfd = accept(listenfd, (SA *) &cliaddr, &clisocklen)) < 0) {
			if (errno == EINTR)
				continue;		/* back to while() */
			else
				err_sys("accept error");
		}
		printf("Received a request from %s:%u \n",inet_ntoa(cliaddr.sin_addr),ntohs(cliaddr.sin_port));
		process_one_request(listenfd, connfd);
	}
	return 1;
}


