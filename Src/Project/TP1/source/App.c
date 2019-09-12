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
#include "magnetic_stripe/magnetic_stripe.h"

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

void ms_callback(ms_ev_t);

/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (void)
{
	interrupts_init();
	systick_init();
	ms_init(ms_callback);
}

/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void)
{
	while(1);
}


void ms_callback(ms_ev_t ev)
{
	volatile static bool i = false;
	if (ev.type == MS_SUCCESS) {
		i = true;
	}
	else {
		i = false;
	}
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/



/*******************************************************************************
 ******************************************************************************/
