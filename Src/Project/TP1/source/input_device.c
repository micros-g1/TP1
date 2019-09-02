/*
 * buttons.c
 *
 *  Created on: 2 Sep 2019
 *      Author: grein
 */
#include <stdio.h>
#include <stdbool.h>
#include "input_device.h"
#include "rotary_encoder.h"

#define TOTAL_ID_EVENTS 6
typedef enum {ID_DOWN_EV,ID_UP_EV,ID_LEFT_EV,ID_RIGHT_EV,ID_TOUT_EV,ID_NOTHING_EV} id_events_t;
#define TOTAL_ID_STATES 5
typedef enum {ID_IDLE_S,ID_PRESS_1_S,ID_PRESS_2_S,ID_PRESS_3_S,ID_KEY_S} id_state_t;
static id_state_t id_state;
//Timer
static int timer_ticks = 0;

//Save rotary encoder states
static bool rotary_encoder_previous_state[ROTARY_ENCODER_INPUTS];

//Actions
void id_left();
void id_right();
void id_key_left();
void id_key_right();
void id_click();
void id_long_click_1();
void id_long_click_2();
void id_nothing();

static void (* const  rotary_encoder_fsm_action[TOTAL_ID_STATES][TOTAL_ID_EVENTS])(void) =
{
                		//ID_DOWN_EV	//ID_UP_EV		//ID_LEFT_EV	//ID_RIGHT_EV	//ID_TOUT_EV		//ID_NOTHING_EV
/*ID_IDLE_S*/      		{id_nothing,	id_nothing,		id_left,		id_right,		id_nothing,			id_nothing},
/*ID_PRESS_1_S*/		{id_nothing,	id_click,		id_key_left,	id_key_right,	id_long_click_1,	id_nothing},
/*ID_PRESS_2_S*/		{id_nothing,	id_nothing,		id_nothing,		id_nothing,		id_long_click_2,	id_nothing},
/*ID_PRESS_3_S*/		{id_nothing,	id_nothing,		id_nothing,		id_nothing,		id_nothing,			id_nothing},
/*ID_KEY_S*/			{id_nothing,	id_nothing,		id_key_left,	id_key_right,	id_nothing,			id_nothing}
};

static const id_state_t  rotary_encoder_fsm_next_state[TOTAL_ID_STATES][TOTAL_ID_EVENTS] =
{
						//ID_DOWN_EV	//ID_UP_EV		//ID_LEFT_EV	//ID_RIGHT_EV	//ID_TOUT_EV		//ID_NOTHING_EV
/*ID_IDLE_S*/      		{ID_PRESS_1_S,	ID_IDLE_S,		ID_IDLE_S,		ID_IDLE_S,		ID_IDLE_S,			ID_IDLE_S},
/*ID_PRESS_1_S*/		{ID_PRESS_1_S,	ID_IDLE_S,		ID_KEY_S,		ID_KEY_S,		ID_PRESS_1_S,		ID_PRESS_1_S},
/*ID_PRESS_2_S*/		{ID_PRESS_2_S,	ID_IDLE_S,		ID_PRESS_2_S,	ID_PRESS_2_S,	ID_PRESS_3_S,		ID_PRESS_2_S},
/*ID_PRESS_3_S*/		{ID_PRESS_3_S,	ID_IDLE_S,		ID_PRESS_3_S,	ID_PRESS_3_S,	ID_PRESS_3_S,		ID_PRESS_3_S},
/*ID_KEY_S*/			{ID_KEY_S,		ID_IDLE_S,		ID_KEY_S,		ID_KEY_S,		ID_KEY_S,			ID_KEY_S}
};

void input_device_init()
{
	systick_add_callback(input_device_ISR,INPUT_DEVICE_ISR_PERIOD_TICKS);
}

void input_device_ISR()
{
	bool input_state = 0;
	id_events_t ev = ID_NOTHING_EV;
	input_state = rotary_encoder_get_input_state(RE_CLICK);
	if(rotary_encoder_previous_state[RE_CLICK] != input_state)
		ev = input_state ? ID_DOWN_EV : ID_UP_EV;
	rotary_encoder_previous_state[RE_CLICK] = input_state;
	input_state = rotary_encoder_get_input_state(RE_LEFT);
	if(rotary_encoder_previous_state[RE_LEFT] != input_state && !input_state)
		ev = ID_LEFT_EV;
	rotary_encoder_previous_state[RE_LEFT] = input_state;
	input_state = rotary_encoder_get_input_state(RE_RIGHT);
	if(rotary_encoder_previous_state[RE_RIGHT] != input_state && !input_state)
			ev = ID_RIGHT_EV;
	rotary_encoder_previous_state[RE_RIGHT] = input_state;
	if(ev != ID_NOTHING_EV)
		timer_ticks = 0;	//Got event. Reset Timer
	else
	{
		if(timer_ticks == INPUT_DEVICE_T_RELOAD_VALUE)
		{
			//Timer Event.
			timer_ticks = 0;
			ev = ID_TOUT_EV;
		}
		else
			timer_ticks++;
	}
	rotary_encoder_fsm_action[id_state][ev]();
	id_state = rotary_encoder_fsm_next_state[id_state][ev];
}

void id_left()
{
	event_t ev;
	ev.id = INPUT_DEVICE_EV;
	ev.metadata = (int) ID_LEFT;
	ev.data = NULL;
	event_queue_add_event(ev);
}
void id_right()
{
	event_t ev;
	ev.id = INPUT_DEVICE_EV;
	ev.metadata = (int) ID_RIGHT;
	ev.data = NULL;
	event_queue_add_event(ev);
}
void id_click()
{
	event_t ev;
	ev.id = INPUT_DEVICE_EV;
	ev.metadata = (int) ID_SHORT_CLICK;
	ev.data = NULL;
	event_queue_add_event(ev);
}
void id_long_click_1()
{
	event_t ev;
	ev.id = INPUT_DEVICE_EV;
	ev.metadata = (int) ID_LONG_CLICK_1;
	ev.data = NULL;
	event_queue_add_event(ev);
}
void id_long_click_2()
{
	event_t ev;
	ev.id = INPUT_DEVICE_EV;
	ev.metadata = (int) ID_LONG_CLICK_2;
	ev.data = NULL;
	event_queue_add_event(ev);
}
void id_key_left()
{
	event_t ev;
	ev.id = INPUT_DEVICE_EV;
	ev.metadata = (int) ID_KEY_LEFT;
	ev.data = NULL;
	event_queue_add_event(ev);
}
void id_key_right()
{
	event_t ev;
	ev.id = INPUT_DEVICE_EV;
	ev.metadata = (int) ID_KEY_RIGHT;
	event_queue_add_event(ev);
}
void id_nothing(){;}
