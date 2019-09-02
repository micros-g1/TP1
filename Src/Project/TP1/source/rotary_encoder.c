/*
 * input_device.c
 *
 *  Created on: 30 Aug 2019
 *      Author: grein
 */

//Idea based on
//https://github.com/ITBAALUMNOS/Tetris/blob/master/Sources/buttons_and_timers.c

//TODO: Make sure everything is active high.

#include <rotary_encoder.h>

//Rotary encoder has 3 electrical signals
//One for button
//Two used for encoding left and right turns
//The negated value of this signal is saved in
static int re_state = 0;	//Negated so that == 0 when idle

//Rotary Encoder Directions
#define ROTARY_ENCODER_DIRECTIONS 2
enum {RE_DIR_LEFT, RE_DIR_RIGHT};
//Variable used to indicate if rotary encoder is turning in a given direction
static int rotary_encoder_turning[ROTARY_ENCODER_DIRECTIONS] = {0};
//Note that this variable will be cleared after reading it!

//Rotary Encoder Actions
static void re_left();		//When rotary encoder is turned left (counterclockwise)
static void re_right();		//When rotary encoder is turned right (clockwise)
static void re_nothing();	//idle

//Rotary Encoder Behaviour defined by FSM
static void (* const  rotary_encoder_fsm[2][4])(void) =
{
                		//Is 00			//Is 01			//Is 10			// Is 11
/*WAS 00*/      		{re_nothing,	re_left,		re_right,		re_nothing},
/*WAS NOT 00*/			{re_nothing,	re_nothing,		re_nothing,		re_nothing}
};

void rotary_encoder_init()
{
	//TODO: Initialization
}

int rotary_encoder_get_input_state(re_id_t id)
{
	int ret_val = 0;
	int i = 0;	//temp
	//Case Rotary Encoder
	switch(id)
	{
		case RE_CLICK:
			//TODO: Read port value
			ret_val = i;
			break;
		case RE_LEFT:
			ret_val = rotary_encoder_turning[RE_LEFT];
			//Value to zero after reading
			rotary_encoder_turning[RE_LEFT] = 0;
			break;
		case RE_RIGHT:
			ret_val = rotary_encoder_turning[RE_RIGHT];
			//Value to zero after reading
			rotary_encoder_turning[RE_RIGHT] = 0;
			break;
		default:
			break;
	}
	return ret_val;
}

void rotary_encoder_ISR()
{
	//ISR is for rotary encoder only
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
static void re_left(){rotary_encoder_turning[RE_LEFT] = 1;}
static void re_right(){rotary_encoder_turning[RE_RIGHT] = 1;}
static void re_nothing(){;}

