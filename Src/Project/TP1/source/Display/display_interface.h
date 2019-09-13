/*
 * displayInterface.h
 *
 *  Created on: 30 ago. 2019
 *      Author: Tomas
 */

#ifndef DISPLAY_DISPLAY_INTERFACE_H_
#define DISPLAY_DISPLAY_INTERFACE_H_

#include "general.h"


//directions, for shifting and controlling brightness
typedef enum{DISPLAY_INT_LEFT, DISPLAY_INT_RIGHT, DISPLAY_INT_UP, DISPLAY_INT_DOWN} display_direction_t;
//freedom led colors
typedef enum{RED, GREEN, BLUE}display_color_t;

#define DISPLAY_INT_MAX_BRIGHT	5
#define DISPLAY_INT_MIN_BRIGHT	1
//callback to call when a new display event (marquee event) has come.
typedef void (*display_marquee_event_callback_t)(void);


/*****************************************
*******display_init_interface*************
******************************************
* display_init_interface initializes the display.
* The function has no effect whatsoever if the display
* is already initialized (safe init)
* 	INPUT:
*		callback : callback that will be called whenever the marquee finishes displaying.
*	OUTPUT:
*		void
*/
void display_init_interface(display_marquee_event_callback_t callback);


/*****************************************
*****************display_marquee**********
******************************************
* display_marquee starts the marquee of a sentence.
* To stop the marquee, display_stop_marquee() should be called.
* 	INPUT:
*		sentence : null terminated string to be shown on the display.
*		dir : direction of the marquee.
*	OUTPUT:
*		void.
*/
void display_marquee(char* sentence, display_direction_t dir);


/*****************************************
***********display_write_or_marquee*******
******************************************
* display_marquee starts the marquee of a sentence if the sentence doesn't fit the display.
* To stop the marquee, display_stop_marquee() should be called.
* If the sentence does fit the display, then the whole sentence is shown on the display
* without enabling a marquee.
* 	INPUT:
*		sentence : null terminated string to be shown on the display.
*		dir : direction of the marquee in case the marquee option should be enabled
*	OUTPUT:
*		void.
*/
void display_write_or_marquee(char * sentence, display_direction_t dir);


/********************************************************
************display_stop_marquee*************************
*********************************************************
* display_stop_marquee stops the current marquee if one is active.
* 	INPUT:
*		void.
*	OUTPUT:
*		void.
*/
void display_stop_marquee();


/*****************************************
*****************clear_all****************
******************************************
* clear_all clears the entire display (does not include LEDS)
* 	INPUT:
*		void.
*	OUTPUT:
*		void.
*/
void display_clear_all();


/*****************************************
***********display_clear_all_leds*********
******************************************
* display_clear_all_leds clears the leds of the display
* 	INPUT:
*		void.
*	OUTPUT:
*		void.
*/
void display_clear_all_leds();


/*****************************************
*************display_write_word***********
******************************************
* display_write_word writes the first AMOUNT_MAX_DISPLAY_POS letters of a word
* into the display's buffer.
* 	INPUT:
*		word : null terminated string to be written on the display.
*	OUTPUT:
*		void.
*/
void display_write_word(const char* word);


/*****************************************
*************display_write_char***********
******************************************
* display_write_char writes a char to the display buffer.
* 	INPUT:
*		c : symbol to be written to the display.
*		pos : position of the display the symbol will be written to.
*	OUTPUT:
*		void.
*/
void display_write_char(char c, int pos);


/*****************************************
*************display_set_blinking_all*****
******************************************
* display_set_blinking_all sets all positions of the display on the specified blinking state
* (DOES NOT INCLUDE LEDS)
* 	INPUT:
* 		on_off : true for enabling blinking, false otherwise.
*	OUTPUT:
*		void.
*/
void display_set_blinking_all(bool on_off);


/*****************************************
*******display_get_is_blinking_any********
******************************************
* display_get_is_blinking_any tells the user if there's at least one
* display position that's currently blinking.
* 	INPUT:
*		void.
*	OUTPUT:
*		true if at least one position is currently blinking enabled.
*/
bool display_get_is_blinking_any();


/*****************************************
**********display_set_blinking_one********
******************************************
* display_set_blinking_one sets the blinking state of a specific position
* of the display to enabled or disabled
* 	INPUT:
* 		on_off : true for enabling blinking for the position, false otherwise.
*		pos : position of the display that will be enabled or disabled for blinking
*	OUTPUT:
*		void
*/
void display_set_blinking_one(bool on_off, int pos);


/*****************************************
**************display_get_is_blinking*****
******************************************
* display_get_is_blinking tells the user if a specific position of the
* display is currently blinking.
* 	INPUT:
*		pos : position of interest.
*	OUTPUT:
*		true if the position is currently blinking enabled.
*/
bool display_get_is_blinking(int pos);





/*************************************************
**************display_get_blinking_led_one********
**************************************************
* display_get_blinking_led_one tells the user if a specific led is currently blinking.
* 	INPUT:
*		pos : position of interest.
*	OUTPUT:
*		true if at the position is currently blinking enabled.
*/
bool display_get_blinking_led_one(int pos);


/*****************************************
*************display_set_blinking_led_one*************
******************************************
* display_set_blinking_led_one tells sets the blinking state of a specific led
*  to enabled or disabled
* 	INPUT:
*		pos : number of the led that will be enabled or disabled for blinking
*		on_off : true for enabling blinking.
*	OUTPUT:
*		void
*/
void display_set_blinking_led_one(int pos, bool on_off);


/*****************************************
***********display_get_is_blinking_leds_any*******
******************************************
* display_get_blinking_leds_any tells at least one led is currently blinking.
* 	INPUT:
*		void.
*	OUTPUT:
*		true if at least one led is currently blinking
*/
bool display_get_is_blinking_leds_any();


/*************************************************
*********display_set_blinking_leds_all************
**************************************************
* display_set_blinking_leds_all sets all leds on the specified blinking state
* 	INPUT:
* 		on_off : true for enabling blinking, false otherwise.
*	OUTPUT:
*		void.
*/
void display_set_blinking_leds_all(bool on_off);


/*****************************************
*************display_write_to_led*********
******************************************
* display_write_to_led turns on or off a specific led.
* 	INPUT:
*		pos : position of the led.
*		on_off : true to turn the led on.
*	OUTPUT:
*		void.*/
void display_write_to_led(int pos, bool on_off);


/*******************************************
*****display_set_brightness_one_up_down*****
********************************************
* display_set_brightness_one_up_down sets the brightness level up or down of
* a specific position of the display.
* can be called even when current brightness is at its limits!! (has no effect in that case)
* 	INPUT:
* 		pos : position of the display that will change its brightness
*		dir : UP or DOWN.
*	OUTPUT:
*		void.*/
void display_set_brightness_one_up_down(int pos, display_direction_t dir);


/*******************************************
*****display_set_brightness_up_down*****
********************************************
* display_set_brightness_up_down sets the brightness level up or down of
* the whole display (DOES NOT INCLUDE LEDS!!)
* can be called even when current brightness is at its limits!! (has no effect in that case)
* 	INPUT:
*		dir : UP or DOWN.
*	OUTPUT:
*		void.*/
void display_set_brightness_up_down(display_direction_t dir);


/*******************************************
*****display_set_brightness_one_pos*****
********************************************
* display_set_brightness_one_pos sets the brightness of a specific position of the display
* (DOES NOT INCLUDE LEDS!!)
* can be called even with brighteness higher or lower that MAX_BRIGHT/MIN_BRIGHT!! (has no effect in that case)
* 	INPUT:
* 		pos : position of the display that will change its brightness
*		brightness : level of brightness to be set.
*	OUTPUT:
*		void.*/
void display_set_brightness_one_pos(int pos, int brightness);


/*******************************************
*****display_set_brightnes_led**************
********************************************
* display_set_brightnes_led sets the brightness level of a specific led
* can be called even with brighteness higher or lower that MAX_BRIGHT/MIN_BRIGHT!! (has no effect in that case)
* 	INPUT:
* 		pos : number of led to be set.
*		brightness : level of brightness to be set.
*	OUTPUT:
*		void.*/
void display_set_brightnes_led(int pos, int brightness);


/*******************************************
*****display_set_brightness_leds**************
********************************************
* display_set_brightness_leds sets the brightness level for all leds
* can be called even with brighteness higher or lower that MAX_BRIGHT/MIN_BRIGHT!! (has no effect in that case)
* 	INPUT:
*		brightness : level of brightness to be set.
*	OUTPUT:
*		void.*/
void display_set_brightness_leds(int brightness);


/*******************************************
*****display_set_brightness_led_up_down**************
********************************************
* display_set_brightness_led_up_down sets the brightness level of a specific led up or down
* can be called even when current brightness is at its limits!! (has no effect in that case)
* 	INPUT:
*		pos : number of the led to be set.
*		dir : UP or DOWN.
*	OUTPUT:
*		void.*/
void display_set_brightness_led_up_down(int pos, display_direction_t dir);


/*******************************************
*****display_set_brightness_leds_up_down****
********************************************
* display_set_brightness_led_up_down sets the brightness level for all leds up or down
* can be called even when current brightness is at its limits!! (has no effect in that case)
* 	INPUT:
*		dir : UP or DOWN.
*	OUTPUT:
*		void.*/
void display_set_brightness_leds_up_down(display_direction_t dir);

/*****************************************
**************display_frdm_led_write******
******************************************
* display_frdm_led_write turns on or off the freedom led.
* 	INPUT:
* 		color : color to write.
*		on_off : true to turn the led on.
*	OUTPUT:
*		void.*/
void display_frdm_led_write(display_color_t color, bool on_off);

#endif /* DISPLAY_DISPLAY_INTERFACE_H_ */
