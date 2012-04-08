/*
 * list.h
 *
 *  Created on: Apr 8, 2012
 *      Author: chenming
 */

#ifndef LIST_H_
#define LIST_H_

typedef struct node
{
	struct node *next;
	int datum;
}NODE;

typedef struct list
{
	NODE *head;
	NODE *tail;
	int size;
}LIST;

void init_list(LIST *l);
void push_back(LIST *l, int datum);
int pop_front(LIST *l);
void clear(LIST *l);

#endif /* LIST_H_ */
