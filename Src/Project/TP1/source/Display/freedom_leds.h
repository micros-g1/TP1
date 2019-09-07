/*
 * freedom_leds.h
 *
 *  Created on: 7 sep. 2019
 *      Author: Tomas
 */

#ifndef DISPLAY_FREEDOM_LEDS_H_
#define DISPLAY_FREEDOM_LEDS_H_
#include "general.h"

typedef enum {FRDM_RED, FRDM_GREEN, FRDM_BLUE} frdm_led_colors_t;
#define MAX_BRIGHT_FRDM_LED		5
#define MIN_BRIGHT_FRDM_LED		0
/*****************************************
*************frdm_led_dr_init*************
******************************************
* frdm_led_dr_init initializes the led's driver.
* Initializes in blue.
* The function has no effect whatsoever if the display
* is already initialized (safe init).
* 	INPUT:
*		void.
*	OUTPUT:
*		void
*/
void frdm_led_dr_init();
/*****************************************
*************frdm_led_dr_reset************
******************************************
* frdm_led_dr_reset resets the led buffer.
* disables blinking, and sets brightness to maximum.
* 	INPUT:
*		void.
*	OUTPUT:
*		void.
*/
void frdm_led_dr_reset();
/*****************************************
**************frdm_led_dr_blink***********
******************************************
* frdm_led_dr_blink enables or disables the blinking function for the freedom led
* 	INPUT:
*		on_off : true if the blinking function should be enabled
*	OUTPUT:
*		void.*/
void frdm_led_dr_blink(bool on_off);
/*****************************************
**************frdm_led_dr_write***********
******************************************
* frdm_led_dr_write turns on or off the freedom led.
* 	INPUT:
*		on_off : true to turn the led on.
*	OUTPUT:
*		void.*/
void frdm_led_dr_write(bool on_off);
/*****************************************
***********frdm_led_dr_is_blinking*********
******************************************
* led_dr_is_blinking_one tells the user if the freedom led is currently blinking enabled.
* 	INPUT:
*		void. :
*	OUTPUT:
*		true if the led is currently blinking enabled.*/
bool frdm_led_dr_is_blinking();
/*****************************************
***********display_frdm_led_set_color*****
******************************************
* display_frdm_led_set_color sets the color for the freedom led (RGB)
* 	INPUT:
*		void. :
*	OUTPUT:
*		void.*/
void frdm_led_dr_set_color(frdm_led_colors_t color);

/*****************************************
*********frdm_led_dr_set_brightness********
******************************************
* frdm_led_dr_set_brightness sets the brightness of the freedom led to a given level.
* 	INPUT:
*		brightness : level of brightness, between MIN_BRIGHT_FRDM_LED and MIN_BRIGHT_FRDM_LED.
*					 an incorrect level of brightness will be ignored.
*	OUTPUT:
*		void.
*/
void frdm_led_dr_set_brightness(int bright);

//gets the brightness level of the freedom led.
int frdm_led_dr_get_brightness();



#endif /* DISPLAY_FREEDOM_LEDS_H_ */