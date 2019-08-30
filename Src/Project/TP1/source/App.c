/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Nicolás Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <interrupts.h>
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
void button_handler(void);
void systick_callback(void);

bool baliza;

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (void)
{
	baliza = false;
	interrupts_init();
	gpioMode(PIN_LED_BLUE,OUTPUT);
    gpioWrite(PIN_LED_BLUE, HIGH);
    gpioMode(PIN_SW3, INPUT);
    gpioIRQ(PIN_SW3, GPIO_IRQ_MODE_FALLING_EDGE, &button_handler);
    SysTick_Init(systick_callback);
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

void button_handler(void){
	baliza = !baliza;
	if(!baliza){
		gpioWrite(PIN_LED_BLUE, HIGH);
	}
}

void systick_callback(void)
{
	static uint32_t counter = 0;

	// use counter for dividing systick frequency
	if(baliza){
		if (counter == SYSTICK_ISR_FREQUENCY_HZ/BLINK_FREQ_HZ/2) {
			// toggle pin twice per period
			gpioToggle(PIN_LED_BLUE);
			counter = 0;
		}
		else {
			counter++;
		}
	}

}


/*******************************************************************************
 ******************************************************************************/
