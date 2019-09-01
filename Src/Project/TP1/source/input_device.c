/*
 * input_device.c
 *
 *  Created on: 30 Aug 2019
 *      Author: grein
 */

#include "input_device.h"

//Rotary Encoder State
static int re_state = 0; // {(!A),(!B)} = 00b when idle

//Simulate Left and Right Buttons
#define ROTARY_ENCODER_BUTTONS 2
enum {RE_LEFT, RE_RIGHT};
static int rotary_virtual_button_state[ROTARY_ENCODER_BUTTONS] = {0};

//Rotary Encoder Actions
static void re_left();
static void re_right();
static void re_nothing();

//Defines Rotary Encoder Behaviour
//Rotary Encoder FSM (xx = {(!A),(!B)} )
static void (* const  rotary_encoder_fsm[2][4])(void) =
{
                		//Is 00			//Is 01			//Is 10			// Is 11
/*WAS 00*/      		{re_nothing,	re_left,		re_right,		re_nothing},
/*WAS NOT 00*/			{re_nothing,	re_nothing,		re_nothing,		re_nothing}
};
//Note: Rotary encoder virtual button state is set to 0 after reading value.

void input_device_init()
{
	//TODO: Initialization
}

int input_device_get_button_state(button_id_t id)
{
	int ret_val = 0;
	//Case BTN_OK
	if(id == BTN_OK)	//BTN_OK is the only real button.
	{
		//TODO: Read port value
		int i = 1;
		ret_val = i;
	}
	//Case Rotary Encoder
	else switch(id)
	{
		case BTN_LEFT:
			ret_val = rotary_virtual_button_state[RE_LEFT];
			//Value to zero after reading
			rotary_virtual_button_state[RE_LEFT] = 0;
			break;
		case BTN_RIGHT:
			ret_val = rotary_virtual_button_state[RE_RIGHT];
			//Value to zero after reading
			rotary_virtual_button_state[RE_RIGHT] = 0;
			break;
		default:
			break;
	}
	return ret_val;
}

void input_device_ISR()
{
	//ISR is only for rotary encoder
	//TODO: Read port value
	int signal_A = 1;
	int signal_B = 1;
	//Get current signal (value is inverted) (interpret as event)
	int event =  (((signal_A? 0:1)) << 1) + (signal_B? 0:1);
	rotary_encoder_fsm[re_state == 0? 0 : 1][event]();	//Execute action
	//Update rotary encoder state
	re_state = event;
}

//FSM Actions
static void re_left(){rotary_virtual_button_state[RE_LEFT] = 1;}
static void re_right(){rotary_virtual_button_state[RE_RIGHT] = 1;}
static void re_nothing(){;}

