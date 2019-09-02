/*
 * displayDriver.h
 *
 *  Created on: 30 ago. 2019
 *      Author: Tomas
 */

#ifndef DISPLAY_DISPLAYDRIVER_H_
#define DISPLAY_DISPLAYDRIVER_H_

#include "board.h"
#include "general.h"

//number of positions the display has.
#define AMOUNT_MAX_DISPLAY_POS		4


/***********************************
*********init_display***************
************************************
* init_display initializes the display.s
* 	INPUT:
*		void.
*	OUTPUT:
*		void.
*/
void init_display(void);
/***********************************
*********blink**********************
************************************
* blink enables or disables the blinking function for the display.
* 	INPUT:
*		on_off : true for enabling, false for disabling.
*	OUTPUT:
*		void.
*/
void blink(bool on_off);
/***********************************
*********is_blinking****************
************************************
* is_blinking lets the user know whether the blinking function is enabled or not.
* 	INPUT:
*		void.
*	OUTPUT:
*		true when the blinking option is currently enabled.
*/
bool is_blinking(void);
/***********************************
*********write_sentence****************
************************************
* write_sentence writes to the display buffer the first
* AMOUNT_MAX_DISPLAY_POS characters of the parameter.
* 	INPUT:
*		sentence: pointer to the sentence to be written.
*	OUTPUT:
*		void.
*/
void write_sentence(const char* sentence);
/***********************************
*********write_char****************
************************************
* write_char writes to the display buffer a given symbol in
* a given position of the display.
* 	INPUT:
*		c : symbol to be written
*		pos : position c will be written on.
*	OUTPUT:
*		void.
*/
void write_char(char c, int pos);
/***********************************
*********display_draw_callback******
************************************
* display_draw_callback callback to be called when
* the display should draw/show the contents of its buffer.
* each function calls handles only one position of the display at a time.
* 	INPUT:
*		void.
*	OUTPUT:
*		void.
*/
void display_draw_callback();
/***********************************
*********display_clear_all**********
************************************
* display_clear_all
* 	INPUT:
*		void.
*	OUTPUT:
*		void.
*/
void display_clear_all();
/*****************************************
*********display_clear_pos****************
******************************************
* display_clear_pos clears a given position of the display (nothing will be shown)
* 	INPUT:
*		pos : position that will be cleared.
*	OUTPUT:
*		void.
*/
void display_clear_pos(int pos);
/*****************************************
*********set_brightness*******************
******************************************
* set_brightness sets the display brightness to a given level.
* 	INPUT:
*		brightness : level of brightness, between 1 and 100.
*	OUTPUT:
*		void.
*/
void set_brightness(int brightness);
/*****************************************
******************shift*******************
******************************************
* shift shifts the sentence that is currently displaying on the
* display according to the direction indicated, an inserting a new symbol
* on the position that is left blank (left position for right direction and viceversa).
* 	INPUT:
*		dir : level of brightness, between 1 and 100.
*		to_insert : symbol to be inserted
*	OUTPUT:
*		void.
*/
void shift(direction dir, char to_insert);
/*****************************************
************display_on_off****************
******************************************
* display_on_off turns the display on or off (off: nothing showing, on: showing what is on the buffer)
* 	INPUT:
*		on_off : true when the display should be turned on, false when turning off.
*	OUTPUT:
*		void.
*/
void display_on_off(bool on_off);
#endif /* DISPLAY_DISPLAYDRIVER_H_ */
