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
#include "gpio.h"
#include "SysTick.h"

#include "rotary_encoder.h"
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define BLINK_FREQ_HZ 1U
#if SYSTICK_ISR_FREQUENCY_HZ % (2*BLINK_FREQ_HZ) != 0
#warning BLINK cannot implement this exact frequency.
		Using floor(SYSTICK_ISR_FREQUENCY_HZ/BLINK_FREQ_HZ/2) instead.
#endif


/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
void enable_callback(void);
void clock_callback(void);
/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void rotary_encoder_callback(re_event_t ev);

/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (void)
{
	interrupts_init();
	systick_init();
	rotary_encoder_init();
	rotary_encoder_set_callback(rotary_encoder_callback);

}

/* Función que se llama constantemente en un ciclo infinito */

void App_Run (void)
{
	while(1);
}

char* ev_name[] = {"RE_LL_LEFT","RE_LL_RIGHT","RE_LL_BUTTON_DOWN","RE_LL_BUTTON_UP","RE_LL_TIMEOUT","RE_LL_NO_EVENT","RE_LL_TOTAL_EVENTS"};


void rotary_encoder_callback(re_event_t ev)
{
	//TODO: Test Rotary Encoder
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/



/*******************************************************************************
 ******************************************************************************/
