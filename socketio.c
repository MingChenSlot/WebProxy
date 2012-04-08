/*
 * socketio.c
 *
 *  Created on: Apr 7, 2012
 *      Author: chenming
 */
#include "server.h"
#include <pthread.h>
#include <stdlib.h>
#include <malloc.h>

ssize_t readn(int fd, void *bufptr, size_t n)
{
	size_t nleft;
	size_t nread;
	char *ptr;

	ptr = bufptr;
	nleft = n;

	while(nleft > 0)
	{
		if( (nread = read(fd, ptr, nleft)) < 0 )
		{
			if(errno == EINTR)
				nread = 0;
			else
				return -1;
		}else if(nread == 0)
			break;

		nleft -= nread;
		ptr += nread;
	}
	return (n - nleft);
}

ssize_t writen(int fd, const void *bufptr, size_t n)
{
	size_t nleft;
	size_t nwritten;
	const char *ptr;

	ptr = bufptr;
	nleft = n;

	while(nleft > 0)
	{
		if( (nwritten = write(fd, ptr, nleft)) < 0)
		{
			if (nwritten <0 && errno == EINTR)
				nwritten = 0;  // call write() again
			else
				return -1;
		}

		nleft -= nwritten;
		ptr += nwritten;
	}
	return n;
}

void print(char *bufptr)
{
	int c;
	while ((c = *bufptr++))
		putchar(c);
	putchar('\n');
}

/* Thread safety readline */
static pthread_key_t rl_key;
static pthread_once_t rl_once = PTHREAD_ONCE_INIT;

static void readline_destructor(void *ptr)
{
	free(ptr);
}

static void readline_once(void)
{
	pthread_key_create(&rl_key, readline_destructor);
}

typedef struct {
	int rl_cnt;
	char *rl_bufptr;
	char rl_buf[MAXLINE];
} RLINE;

static ssize_t
read_helper(RLINE *tsd, int fd, char *ptr)
{
	if (tsd->rl_cnt <= 0)
	{
		again:
		 if((tsd->rl_cnt = read(fd, tsd->rl_buf, MAXLINE)) < 0)
		 {
			 if(errno == EINTR)
				 goto again;
			 return(-1);
		 }else if(tsd->rl_cnt == 0)
		 {
			 return 0;
		 }
		 tsd->rl_bufptr = tsd->rl_buf;
	}

	tsd->rl_cnt--;
	*ptr = *tsd->rl_bufptr++;
	return (1);
}

ssize_t
readline(int fd, void *bufptr, size_t maxlen)
{
	size_t n, rc;
	char c, *ptr;
	RLINE *tsd;

	pthread_once(&rl_once, readline_once);
	if((tsd = pthread_getspecific(rl_key)) == NULL)
	{
		tsd = calloc(1, sizeof(RLINE));
		pthread_setspecific(rl_key, tsd);
	}

	ptr = bufptr;
	for(n = 1; n < maxlen; n++)
	{
		if((rc = read_helper(tsd, fd, &c)) == 1)
		{
			*ptr++ = c;
			if(c == '\n')
				break;
		}else if(rc == 0)
		{
			*ptr = 0;
			return (n-1);
		}else
			return (-1);
	}

	*ptr = 0;
	return n;
}



