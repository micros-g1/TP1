/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Nicolás Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "Display/display_interface.h"
#include "Rotary_encoder/rotary_encoder.h"
#include "Database/database.h"
#include "Timer/timers.h"
#include "Magnetic_stripe/magnetic_stripe.h"
#include "fsm.h"
#include "events.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
void m_finished(void);
void rotary_encoder_callback(re_event_t ev);
void timeout_callback(unsigned int id);
extern void ms_callback(ms_ev_t ev);


fsm_state_t * state;
fsm_event_t event;

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (void)
{
	rotary_encoder_init();
	rotary_encoder_set_callback(rotary_encoder_callback);

	display_init_interface(m_finished);
	u_init();
	timers_init();
	ms_init(ms_callback);

	display_write_to_led(1, true);
	display_write_to_led(0, true);
	display_write_to_led(2, true);

	init_event_queue();
	state = fsm_get_init_state();

	timers_set_timer_mode(0, TIMER_REPEAT);
	timers_set_timer_period(0, 10*1000);
	timers_set_timer_callback(0, timeout_callback);
	timers_set_timer_enabled(0, true);
}

/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void)
{
	if(is_there_event()){
		timers_reset_timer(0);
		pop_event(&event);
		state = fsm_run(state, event);
	}
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
void rotary_encoder_callback(re_event_t ev)
{
	fsm_event_t new_event;
	bool ev_flag = false;
	switch(ev)
	{
		case RE_LEFT:
			new_event.code = DOWN_EV;
			ev_flag = true;
			break;
		case RE_RIGHT:
			new_event.code = UP_EV;
			ev_flag = true;
			break;
		case RE_KEY_LEFT:
			ev_flag = false;
			new_event.code = BRIGHT_DOWN_EV;
			display_set_brightness_up_down(DISPLAY_INT_DOWN);
			break;
		case RE_KEY_RIGHT:
			ev_flag = false;
			new_event.code = BRIGHT_UP_EV;
			display_set_brightness_up_down(DISPLAY_INT_UP);
			break;
		case RE_SHORT_CLICK:
			ev_flag = true;
			new_event.code = ENTER_EV;
			break;
		case RE_LONG_CLICK_1:
			ev_flag = true;
			new_event.code = BACK_EV;
			break;
		case RE_LONG_CLICK_2:
			ev_flag = true;
			new_event.code = CANCEL_EV;
			break;
		default:
			new_event.code = NO_EV;
			ev_flag = true;
			break;
	}
	if(new_event.code != NO_EV && ev_flag)
		push_event(new_event);
}

void m_finished()
{
	fsm_event_t ev;
	ev.code = MARQUEE_END_EV;
	push_event(ev);
}

void timeout_callback(unsigned int id)
{
	fsm_event_t ev;
	ev.code = TIMEOUT_EV;
	push_event(ev);
}

/*******************************************************************************
 ******************************************************************************/
