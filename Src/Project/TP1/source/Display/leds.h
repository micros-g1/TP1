/*
 * leds.h
 *
 *  Created on: 5 sep. 2019
 *      Author: Tomas
 */

#ifndef DISPLAY_LEDS_H_
#define DISPLAY_LEDS_H_

#include "general.h"

typedef enum{LED_LEFT, LED_RIGHT, LED_UP, LED_DOWN} direction_led_t;
#define AMOUNT_MAX_DIODES_POS		3
#define MAX_BRIGHT_LED	5
#define MIN_BRIGHT_LED	1
/*****************************************
*************led_dr_init******************
******************************************
* led_dr_init initializes the led's driver.
* The function has no effect whatsoever if the display
* is already initialized (safe init)
* 	INPUT:
*		void.
*	OUTPUT:
*		void
*/
void led_dr_init();
/*****************************************
*************led_dr_reset****************
******************************************
* led_dr_reset resets the led buffer,
* disables blinking, and sets brightness to maximum.
* 	INPUT:
*		void.
*	OUTPUT:
*		void.
*/
void led_dr_reset();

/*****************************************
**************led_dr_blink****************
******************************************
* led_dr_blink enables or disables the blinking function of a specific led
* 	INPUT:
*		pos : position of the led.
*		on_off : true if the blinking function should be enabled
*	OUTPUT:
*		void.*/
void led_dr_blink(int pos, bool on_off);
/*****************************************
*****************led_dr_write*************
******************************************
* led_dr_write turns on or off a specific led.
* 	INPUT:
*		pos : position of the led.
*		on_off : true to turn the led on.
*	OUTPUT:
*		void.*/
void led_dr_write(int pos, bool on_off);
/*****************************************
***********led_dr_is_blinking_one*********
******************************************
* led_dr_is_blinking_one tells the user if a specific led is currently blinking enabled.
* 	INPUT:
*		pos : position of the led.
*	OUTPUT:
*		true if the led is currently blinking enabled.*/
bool led_dr_is_blinking_one(int pos);
/*****************************************
*********led_dr_set_brightness_one********
******************************************
* led_dr_set_brightness_one sets the brightness of a specific led to a given level.
* 	INPUT:
* 		pos : led for which brightness will be updated.
*		brightness : level of brightness, between MIN_BRIGHT_LED and MIN_BRIGHT_LED.
*					 an incorrect level of brightness will be ignored.
*	OUTPUT:
*		void.
*/
void led_dr_set_brightness_one(int pos, int bright);
/*****************************************
*********led_dr_shift*********************
******************************************
* led_dr_shift shifts the leds values according to the direction indicated
* and inserting a new value on the position that would be left blank.
* 	INPUT:
*		dir : direction for the shift.
*		to_insert : value to insert.
*	OUTPUT:
*		void.
*/
void led_dr_shift(direction_led_t dir, bool to_insert);
/*****************************************
*********led_dr_on_off*******************
******************************************
* led_dr_on_off turns the leds off
* 	INPUT:
*		on_off : leds off.
*	OUTPUT:
*		void.
*/
void led_dr_on_off(bool on_off);
/*****************************************
*********led_dr_get_brightness_one********
******************************************
* led_dr_get_brightness_one gets the brightness level of a specific led.
* If the position given is inexistent, the default return value is MIN_BRIGHT_LED
* 	INPUT:
*		pos : number of led to get the current brightness level from.
*	OUTPUT:
*		void.
*/
int led_dr_get_brightness_one(int pos);
#endif /* DISPLAY_LEDS_H_ */
