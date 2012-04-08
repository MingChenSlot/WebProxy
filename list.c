/*
 * list.c
 *
 *  Created on: Apr 8, 2012
 *      Author: chenming
 */
#include <stdlib.h>
#include "list.h"

void init_list(LIST *l)
{
	l->size = 0;
	l->head = NULL;
	l->tail = NULL;
}

void push_back(LIST *l, int datum)
{
	NODE *tmp;
	tmp = (NODE *)malloc(sizeof(NODE));
	tmp->datum = datum;
	tmp->next = NULL;
	if(l->head == NULL)
		l->head = l->tail = tmp;
	else
	{
		l->tail->next = tmp;
		l->tail = tmp;
	}
	l->size++;
}

int pop_front(LIST *l)
{
	if (l->head == NULL)
		return -1;
	int val;
	NODE *tmp;
	val = l->head->datum;
	tmp = l->head;
	if(l->head == l->tail)
		l->head = l->tail = NULL;
	else
		l->head = l->head->next;
	free(tmp);
	l->size--;
	return val;
}

void clear(LIST *l)
{
	while(l->head != NULL)
		pop_front(l);
}
