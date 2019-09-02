/*
 * input_device.c
 *
 *  Created on: 30 Aug 2019
 *      Author: grein
 */

//Idea based on
//https://github.com/ITBAALUMNOS/Tetris/blob/master/Sources/buttons_and_timers.c

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
bool rotary_encoder_turning[ROTARY_ENCODER_DIRECTIONS] = {0};
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
	gpioMode (PIN_BUTTON_SIGNAL, INPUT_PULLUP);
	gpioMode (PIN_SIGNAL_A, INPUT_PULLUP);
	gpioMode (PIN_SIGNAL_B, INPUT_PULLUP);
	systick_add_callback(rotary_encoder_ISR,ROTARY_ENCODER_ISR_PERIOD_TICKS);
}

bool rotary_encoder_get_input_state(re_id_t id)
{
	bool ret_val = 0;
	//Case Rotary Encoder
	switch(id)
	{
		case RE_CLICK:
			ret_val = !gpioRead(PIN_BUTTON_SIGNAL);
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
	//Get current signal (value is inverted) (interpret as event)
	int event =  ((gpioRead(PIN_SIGNAL_A)? 0 : 1) << 1) + (gpioRead(PIN_SIGNAL_B) ? 0 : 1);
	rotary_encoder_fsm[re_state == 0? 0 : 1][event]();	//Execute action
	//Update rotary encoder state
	re_state = event;
}

//FSM Actions
static void re_left(){rotary_encoder_turning[RE_LEFT] = 1;}
static void re_right(){rotary_encoder_turning[RE_RIGHT] = 1;}
static void re_nothing(){;}
