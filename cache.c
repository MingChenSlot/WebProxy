/*
 * cache.c
 *
 *  Created on: Apr 8, 2012
 *      Author: chenming
 */
#include "server.h"

void b_resetbuf(CACHE *cache)
{
	cache->current = 1;
}

int b_getid(unsigned char* url, int len)
{
	int a = 0;
	int i = 0;
	for (i = 0; i< len; i++)
		a += url[i];
	return a % 1000;
}

int b_putbuf(CACHE *cache, unsigned char* data, int len, int id)
{
	int i = 0;
	if( len >= 0.5 * MAX_BUF_LEN || len + cache->current > MAX_BUF_LEN)
	{
		return -1;
	}
	cache->len[id] = len;
	cache->loc[id] = cache->current;
	for (i = 0; i < len; i++)
	{
		cache->buf[cache->current++] = data[i];
	}
	return 0;
}

int b_putbuf_c(CACHE *cache, unsigned char* data, int len, int id)
{
	int i = 0;
	if( len >= 0.5 * MAX_BUF_LEN || len + cache->current > MAX_BUF_LEN)
	{
		cache->len[id] = 0;
		cache->current = cache->loc[id];
		return -1;
	}
	cache->len[id] += len;
	for (i = 0; i < len; i++)
	{
		cache->buf[cache->current++] = data[i];
	}
	return 0;
}

int b_getbuf(CACHE *cache, unsigned char *data, int id)
{
	unsigned char i = 0;
	int len = cache->len[id];
	int loc = cache->loc[id];

	for(i=0; i<len; i++)
	{
		data[i] = cache->buf[loc++];
	}

	return len;
}
