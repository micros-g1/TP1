/*
 * freedom_leds.c
 *
 *  Created on: 7 sep. 2019
 *      Author: Tomas
 */
#include <gpio.h>
#include "freedom_leds.h"
#include "MK64F12.h"
#include "board.h"

/*-------------------------------------------
 ----------------DEFINES---------------------
 -------------------------------------------*/

/*-------------------------------------------
 ----------GLOBAL_VARIABLES------------------
 -------------------------------------------*/


/*-------------------------------------------
 ----------STATIC_FUNCTION_DECLARATION-------
 -------------------------------------------*/

/*-------------------------------------------
 ------------FUNCTION_IMPLEMENTATION---------
 -------------------------------------------*/
void frdm_led_dr_init(){
	static bool initialized = false;

	if(initialized) return;

	gpioMode(PIN_LED_RED, OUTPUT);
	gpioMode(PIN_LED_GREEN, OUTPUT);
	gpioMode(PIN_LED_BLUE, OUTPUT);

	initialized = true;
}

void frdm_led_dr_write(frdm_led_colors_t color, bool on_off){
	//the freedom leds are active low!
	switch (color){
		case FRDM_RED:
			gpioWrite(PIN_LED_RED, !on_off);
			break;
		case FRDM_GREEN:
			gpioWrite(PIN_LED_GREEN, !on_off);
			break;
		case FRDM_BLUE:
			gpioWrite(PIN_LED_BLUE, !on_off);
			break;
		default:
			break;
	}
}

