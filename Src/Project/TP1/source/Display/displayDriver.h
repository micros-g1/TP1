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
/*-------------------------------------------
 ----------------DEFINES---------------------
 -------------------------------------------*/
//number of positions the display has.
#define AMOUNT_MAX_DISPLAY_POS		4
//symbol equivalent to nothing showing on the display.
#define NULL_CHAR	0X00
#define MAX_BRIGHT	5
#define MIN_BRIGHT	0

/*****************************************
*************init_display*****************
******************************************
* init_display initializes the display.
* The function has no effect whatsoever if the display
* is already initialized.
* 	INPUT:
*		void.
*	OUTPUT:
*		void
*/
void init_display();
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
* AMOUNT_MAX_DISPLAY_POS symbols of the parameter.
* 	INPUT:
*		sentence: pointer to the sentence to be written.
*	OUTPUT:
*		number of symbols belonging to sentence that were written to the display buffer.
*		always <= AMOUNT_MAX_DISPLAY_POS
*/
int write_sentence(const char* sentence);
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
*		brightness : level of brightness, between MIN_BRIGHT and MAX_BRIGHT.
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

/*****************************************
*******get_currently_displaying_word******
******************************************
* get_currently_displaying_word returns a string (null terminated word!!!)
* with the word that is currently being displayed.
* This word does not necessarily coincide with the buffered word (such is the case when blinking is enabled
* or when the display has not been updated yet with the new input).
* For that information, check get_currently_on_buffer_word.
* THE CONTENT OF THE POINTER WILL CHANGE WHEN THIS FUNCTION IS CALLED AGAIN,
* SO IT IS RECOMMENDED TO SAVE THE CONTENTS IN ANOTHER VARIABLE!!
* 	INPUT:
*		void.
*	OUTPUT:
*		the word that is currently being displayed.
*/
char* get_currently_curr_displaying_word();
/*****************************************
*******get_currently_on_buffer_word******
******************************************
* get_currently_on_buffer_word returns a string (null terminated word!!!)
* with the word contained by the display buffer. This word will be the
* next word to be displayed when the drawing function is called (when not blinking),
* but does not necessarily coincide with the word being shown on the display.
* For that information, check get_currently_curr_displaying_word() function.
* THE CONTENT OF THE POINTER WILL CHANGE WHEN THIS FUNCTION IS CALLED AGAIN,
* SO IT IS RECOMMENDED TO SAVE THE CONTENTS IN ANOTHER VARIABLE!!
* 	INPUT:
*		void.
*	OUTPUT:
*		the word that is currently on the display buffer.*/
char* get_currently_on_buffer_word();

#endif /* DISPLAY_DISPLAYDRIVER_H_ */
