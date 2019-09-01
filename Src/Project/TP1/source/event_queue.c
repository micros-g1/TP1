/*
 * event_queue.c
 *
 *  Created on: 30 Aug 2019
 *      Author: grein
 */

#include "event_queue.h"

//Event Queue Variables
static volatile unsigned int queue_length = 0;
static volatile event_t event_queue[EV_QUEUE_SIZE];
static volatile unsigned int in_offset = 0;
static volatile unsigned int out_offset = 0;

//Wait for event
void queue_wait_for_event(event_t* ev)
{
	//Atomic operation (assembly)
	while(queue_length == 0);
	//TODO: disable interrupts
	*ev = event_queue[out_offset++];
	if(out_offset == EV_QUEUE_SIZE)
		out_offset = 0;
	queue_length--;
	//TODO: enable interrupts
}

//Flush event queue
void queue_flush()
{
	//TODO: disable interrupts
	queue_length = in_offset = out_offset = 0;
	//TODO: enable interrupts
}

//Add event to event queue
void queue_add_event(event_t ev)
{
	//TODO: disable interrupts
	event_queue[in_offset++] = ev;
	if(in_offset == EV_QUEUE_SIZE)
		in_offset = 0;
	queue_length++;
	//TODO: enable interrupts
}

//Get current queue length
unsigned int queue_get_length()
{
	//atomic read operation (assembly)
	return queue_length;
}
