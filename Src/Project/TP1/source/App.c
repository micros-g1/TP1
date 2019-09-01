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

/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (void)
{
	interrupts_init();

	gpioMode(PORTNUM2PIN(PD,0), INPUT);
	gpioMode(PORTNUM2PIN(PD,2), INPUT);
	gpioIRQ(PORTNUM2PIN(PD,0), GPIO_IRQ_MODE_BOTH_EDGES, enable_callback); // enable
	gpioIRQ(PORTNUM2PIN(PD,2), GPIO_IRQ_MODE_FALLING_EDGE, clock_callback); // clock
	gpioMode(PIN_LED_BLUE, OUTPUT);
	gpioWrite(PIN_LED_BLUE,1);
}

/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void)
{
	while(1);
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void enable_callback(void)
{
	gpioToggle(PIN_LED_BLUE);
}

void clock_callback(void)
{
	;
}

/*******************************************************************************
 ******************************************************************************/
