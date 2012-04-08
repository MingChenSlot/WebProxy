/*
 * socketio.c
 *
 *  Created on: Apr 7, 2012
 *      Author: chenming
 */
#include "server.h"

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
