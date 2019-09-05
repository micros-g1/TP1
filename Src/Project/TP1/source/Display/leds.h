/*
 * leds.h
 *
 *  Created on: 5 sep. 2019
 *      Author: Tomas
 */

#ifndef DISPLAY_LEDS_H_
#define DISPLAY_LEDS_H_

#include "general.h"
#define AMOUNT_MAX_DIODES_POS		3
#define MAX_BRIGHT_LED	5
#define MIN_BRIGHT_LED	0
void leds_init();
void leds_reset();

/*****************************************
*******blink_led******
******************************************
* blink_led enables or disables the blinking function of a specific led
* 	INPUT:
*		pos : position of the led.
*		on_off : true if the blinking function should be enabled
*	OUTPUT:
*		void.*/
void blink_led(int pos, bool on_off);
/*****************************************
********************write_led*************
******************************************
* write_led turns on or off a specific led.
* 	INPUT:
*		pos : position of the led.
*		on_off : true to turn the led on.
*	OUTPUT:
*		void.*/
void write_led(int pos, bool on_off);
/*****************************************
**************is_blinking_led_one*********
******************************************
* is_blinking_led_one tells the user if a specific led is currently blinking enabled.
* 	INPUT:
*		pos : position of the led.
*	OUTPUT:
*		true if the led is currently blinking enabled.*/
bool is_blinking_led_one(int pos);
/*****************************************
*********set_brightness_led_one*******************
******************************************
* set_brightness_one sets the brightness of a specific led to a given level.
* 	INPUT:
* 		pos : led for which brightness will be updated.
*		brightness : level of brightness, between MIN_BRIGHT_LED and MIN_BRIGHT_LED.
*					 an incorrect level of brightness will be ignored.
*	OUTPUT:
*		void.
*/
void set_brightness_led_one(int pos, int bright);
/*****************************************
*********shift_leds*******************
******************************************
* shift_leds shifts the leds values according to the direction indicated
* and inserting a new value on the position that would be left blank.
* 	INPUT:
*		dir : direction for the shift.
*		to_insert : value to insert.
*	OUTPUT:
*		void.
*/
void shift_leds(direction dir, bool to_insert);
/*****************************************
*********leds_on_off*******************
******************************************
* leds_on_off turns the leds off
* 	INPUT:
*		on_off : leds off.
*	OUTPUT:
*		void.
*/
void leds_on_off(bool on_off);

#endif /* DISPLAY_LEDS_H_ */
