/*
 * event_queue.h
 *
 *  Created on: 30 Aug 2019
 *      Author: grein
 */

#ifndef EVENT_QUEUE_H_
#define EVENT_QUEUE_H_

//Possible Events
//TODO: Fill with possible events
typedef enum {EV_1,EV_2} event_id_t;

//Total number of events that event queue can hold
#define EV_QUEUE_SIZE	20

//Event structure
typedef struct
{
	event_id_t id;
	void*	data;
}event_t;

//Wait for event
void queue_wait_for_event(event_t* ev);
//Flush event queue
void queue_flush();
//Add event to event queue. Can be used by ISRs
void queue_add_event(event_t ev);
//Get current queue length. Can be used by ISRs
unsigned int queue_get_length();

#endif /* EVENT_QUEUE_H_ */
