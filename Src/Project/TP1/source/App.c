/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Nicolás Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/


#include "interrupts.h"
#include "board.h"
#include "general.h"
#include "gpio.h"
#include "SysTick.h"
#include "Display/displayInterface.h"
#include "rotary_encoder.h"

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/




/******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
void enable_callback(void);
void clock_callback(void);
void systick_callback(void);
void m_finished(display_event_t ev);
void rotary_encoder_callback(re_event_t ev);

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (void)
{

	systick_init();
	rotary_encoder_init();
	rotary_encoder_set_callback(rotary_encoder_callback);
	systick_add_callback(systick_callback, 10000, SINGLE_SHOT);
	init_display_interface(m_finished);
}

/* Función que se llama constantemente en un ciclo infinito */

void App_Run (void)
{
	while(1);
}

char* ev_name[] = {"RE_LL_LEFT","RE_LL_RIGHT","RE_LL_BUTTON_DOWN","RE_LL_BUTTON_UP","RE_LL_TIMEOUT","RE_LL_NO_EVENT","RE_LL_TOTAL_EVENTS"};


void rotary_encoder_callback(re_event_t ev)
{
	switch(ev)
	{
		case RE_LEFT:
			write_char_2_display('1', 0);
			break;
		case RE_RIGHT:
			write_char_2_display('2', 0);
			break;
		case RE_KEY_LEFT:
			write_char_2_display('3', 0);
			break;
		case RE_KEY_RIGHT:
			write_char_2_display('4', 0);
			break;
		case RE_SHORT_CLICK:
			write_char_2_display('5', 0);
			break;
		case RE_LONG_CLICK_1:
			write_char_2_display('6', 0);
			break;
		case RE_LONG_CLICK_2:
			write_char_2_display('7', 0);
			break;
		default:
			break;
	}
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void systick_callback(void)
{
	write_to_led(1, true);
	write_to_led(0, true);
	write_to_led(2, true);
	for(int i = 0; i< 3; i++)
		set_blinking_led_one(i, true);
	set_blinking_all(false);
	marquee("123456", RIGHT);
}

void m_finished(display_event_t ev)
{
	if(ev.id == MARQUEE_FINISHED)
		write_char_2_display('A', 1);
}



/*******************************************************************************
 ******************************************************************************/
