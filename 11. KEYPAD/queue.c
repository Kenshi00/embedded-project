/*
 * queue.c
 *
 */ 
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

#define TRUE 1
#define FALSE 0
#define QUEUE_MAX 100

int u_front=-1;   // read index
int u_rear=-1;    // insert index
uint8_t queue[QUEUE_MAX];

int queue_full(void)
{
	// queue에서 rear+1 % QUEUE_MAX의 값이 front와 같으면 queue full
	int tmp=(u_rear+1) % QUEUE_MAX;
	if (tmp == u_front)  // front와 같으면 queue full
		return TRUE;
	else return FALSE;	
}

int queue_empty()
{
	if (u_rear == u_front)  // front와 같으면 queue empty
		return TRUE;
	else return FALSE;	
}

uint8_t read_queue()
{
	if (queue_empty())
		printf("Queue is empty !!!\n");
	else
	{
		u_front = (u_front+1) % QUEUE_MAX;
		return (queue[u_front]);
	}
}

void queue_init()  // queue가 텅 빈경우 fron와 rear가 동일한 위치를 가리틴다.
{
	u_front=-1;   // read index
	u_rear=-1;    // insert index	
}

void insert_queue(uint8_t value)
{
	if (queue_full())
	{
		printf("queue full!!!!\n");
		return;
	}
	else   // save 
	{
		u_rear = (u_rear+1) % QUEUE_MAX;
		queue[u_rear]=value;
	}
}