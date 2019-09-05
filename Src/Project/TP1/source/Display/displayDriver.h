/*
 * displayDriver.h
 *
 *  Created on: 30 ago. 2019
 *      Author: Tomas
 */

#ifndef DISPLAY_DISPLAYDRIVER_H_
#define DISPLAY_DISPLAYDRIVER_H_

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
*************display_init*****************
******************************************
* display_init initializes the display.
* The function has no effect whatsoever if the display
* is already initialized (safe init)
* 	INPUT:
*		void.
*	OUTPUT:
*		void
*/
void display_init();
/*****************************************
************display_on_off****************
******************************************
* display_on_off turns the display on or off (off: nothing showing, on: showing what is on the buffer).
* The previous status of the display before turning off is remembered when turned on, so the user should call
* display_reset() function if he/she/it/helicopter wants to reset the display status.
* 	INPUT:
*		on_off : true when the display should be turned on, false when turning off.
*	OUTPUT:
*		void.
*/
void display_on_off(bool on_off);
/*****************************************
*************display_reset****************
******************************************
* display_reset resets the display buffer,
* disables blinking, and sets brightnes to maximum.
* 	INPUT:
*		void.
*	OUTPUT:
*		void.
*/
void display_reset();
/***********************************
*********is_blinking_one************
************************************
* is_blinking_one lets the user know whether the blinking function is enabled or not
* (for one specific position of the display). Blinking has a specific frequency set!
* 	INPUT:
*		void.
*	OUTPUT:
*		true when the blinking option is enabled for the specific position.
*		false when the position has its blinking option disabled.
*/
bool is_blinking_one(int pos);
/***********************************
*********blink_one****************
************************************
* blink_one enables or disables the blinking function
* for one specific position of the display.
* 	INPUT:
* 		pos : position of the display for whcich the blink will be enabled or disabled.
*		on_off : true for enabling the blinking function. false for disabling it.
*	OUTPUT:
*		void.
*/
void blink_one(int pos, bool on_off);
/***********************************
*********write_sentence****************
************************************
* write_sentence writes to the display buffer the first
* AMOUNT_MAX_DISPLAY_POS symbols of the parameter.
* 	INPUT:
*		sentence: pointer to the sentence to be written (should be null terminated).
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
*		pos : position c will be written to.
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
* display_clear_all clears the display buffer.
* 	INPUT:
*		void.
*	OUTPUT:
*		void.
*/
void display_clear_all();
/*****************************************
*********display_clear_pos****************
******************************************
* display_clear_pos clears a given position of the display's buffer (nothing will be shown when it is redrawn).
* 	INPUT:
*		pos : position that will be cleared.
*	OUTPUT:
*		void.
*/
void display_clear_pos(int pos);
/*****************************************
*********set_brightness_one*******************
******************************************
* set_brightness_one sets the brightness of a specific position of the display to a given level.
* 	INPUT:
* 		pos : position of the display for which brightness will be updated.
*		brightness : level of brightness, between MIN_BRIGHT and MAX_BRIGHT.
*					 an incorrect level of brightness will be ignored.
*	OUTPUT:
*		void.
*/
void set_brightness_one(int pos, int brightness);
/*****************************************
******************shift*******************
******************************************
* shift shifts the sentence that is currently displaying on the
* display according to the direction indicated, and inserting a given symbol
* on the position that would be left blank (left position for right direction and viceversa).
* 	INPUT:
*		dir : direction for the shifting operation
*		to_insert : symbol to be inserted
*	OUTPUT:
*		void.
*/
void shift(direction dir, char to_insert);

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
