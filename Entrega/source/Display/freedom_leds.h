/*
 * freedom_leds.h
 *
 *  Created on: 7 sep. 2019
 *      Author: Tomas
 */

#ifndef DISPLAY_FREEDOM_LEDS_H_
#define DISPLAY_FREEDOM_LEDS_H_
#include "general.h"

typedef enum {FRDM_RED, FRDM_GREEN, FRDM_BLUE, FRDM_AMOUNT_OF_COLORS} frdm_led_colors_t;

/*****************************************
*************frdm_led_dr_init*************
******************************************
* frdm_led_dr_init initializes the led's driver.
* The function has no effect whatsoever if the display
* is already initialized (safe init).
* 	INPUT:
*		void.
*	OUTPUT:
*		void
*/
void frdm_led_dr_init();

/*****************************************
**************frdm_led_dr_write***********
******************************************
* frdm_led_dr_write turns on or off the freedom led.
* 	INPUT:
* 		color : color to turn on or off
*		on_off : true to turn the led on.
*	OUTPUT:
*		void.*/
void frdm_led_dr_write(frdm_led_colors_t color, bool on_off);

#endif /* DISPLAY_FREEDOM_LEDS_H_ */
