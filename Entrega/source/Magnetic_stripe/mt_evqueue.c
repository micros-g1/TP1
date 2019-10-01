//
// Created by Rocío Parra on 9/9/2019.
//

#include <Magnetic_stripe/mt_evqueue.h>
#include "hardware.h"

//Event queue Variables
static volatile mt_ev_t event_queue[EV_QUEUE_MAX_LENGTH];	//Circular queue
static volatile unsigned int queue_length = 0;				//Keeps track of queue length
static volatile unsigned int in_offset = 0;					//Offset for adding next event
static volatile unsigned int out_offset = 0;				//Offset for reading next event

//Wait for event
void event_queue_wait_for_event(mt_ev_t* ev)
{
    //Atomic operation (assembly)
    while(queue_length == 0);
    *ev = event_queue[out_offset++];
    if(out_offset == EV_QUEUE_MAX_LENGTH)
        out_offset = 0;
    //Atomic operation (assembly)
    queue_length--;
}

//Flush event queue
void event_queue_flush()
{
    hw_DisableInterrupts();
    //Must set these three variables to zero before continuing...
    queue_length = in_offset = out_offset = 0;
    hw_EnableInterrupts();
}

//Add event to event queue
bool event_queue_add_event(mt_ev_t ev)
{
    bool ret_val = false;
    hw_DisableInterrupts();
    //Only one event source can add an event to the queue at a given time
    if(queue_length != EV_QUEUE_MAX_LENGTH)
    {
        event_queue[in_offset++] = ev;
        if(in_offset == EV_QUEUE_MAX_LENGTH)
            in_offset = 0;
        queue_length++;
        ret_val = true;
    }
    hw_EnableInterrupts();
    return ret_val;
}


//Add event to event queue
bool event_queue_add_event_front(mt_ev_t ev)
{
    bool ret_val = false;
    hw_DisableInterrupts();
    //Only one event source can add an event to the queue at a given time
    if(queue_length != EV_QUEUE_MAX_LENGTH)
    {
    	if(out_offset == 0)
    		out_offset = EV_QUEUE_MAX_LENGTH;
    	out_offset--;
        event_queue[out_offset] = ev;
        queue_length++;
        ret_val = true;
    }
    hw_EnableInterrupts();
    return ret_val;
}


//Get current queue length
unsigned int event_queue_get_length()
{
    //atomic read operation (assembly)
    return queue_length;
}


mt_ev_t event_queue_pop_front()
{
	mt_ev_t ev;
	ev.type = MT_N_EVS;

    if (queue_length) {
        queue_length--;

        ev = event_queue[out_offset++];
        if(out_offset == EV_QUEUE_MAX_LENGTH) {
        	out_offset = 0;
        }
        //DEBUG
        if(queue_length == EV_QUEUE_MAX_LENGTH)
        	queue_length = EV_QUEUE_MAX_LENGTH+1;
    }

    return ev;
}
