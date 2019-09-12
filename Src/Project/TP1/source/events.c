//
// Created by lisan on 6/9/2019.
//
#include <stdbool.h>
#include "events.h"

bool event_flag;
fsm_event_queue queue;

void init_event_queue(){
    queue.tail = queue.head = queue.count = 0;
    event_flag = false;
}

void push_event(fsm_event_t event){
    if(MAX_QUEUE_LENGTH == queue.count)
        return;
    queue.buffer[queue.head].code = event.code;
    queue.count++;
    queue.head = (queue.head + 1) % MAX_QUEUE_LENGTH;
    event_flag = true;
}

void pop_event(fsm_event_t * event)
{
    if(queue.count == 0)
        return;
    queue.count--;
    event->code = queue.buffer[queue.tail].code;
    queue.tail = (queue.tail + 1) % MAX_QUEUE_LENGTH;
    if(queue.tail == queue.head)
    	event_flag = false;
}

bool is_there_event(void){
	return event_flag;
}
