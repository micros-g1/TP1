/*
 * displayDriver.h
 *
 *  Created on: 30 ago. 2019
 *      Author: Tomas
 */

#ifndef DISPLAY_DISPLAY_DRIVER_H_
#define DISPLAY_DISPLAY_DRIVER_H_

#include "general.h"
/*-------------------------------------------
 ----------------DEFINES---------------------
 -------------------------------------------*/
//number of positions the display has.
#define AMOUNT_MAX_DISPLAY_POS		4

//symbol equivalent to nothing showing on the display.
#define NULL_CHAR	0X00
#define DISPLAY_MAX_BRIGHT	5
#define DISPLAY_MIN_BRIGHT	1

typedef enum{DISPLAY_LEFT, DISPLAY_RIGHT, DISPLAY_UP, DISPLAY_DOWN} direction_display_t;

/*****************************************
*************display_dr_init**************
******************************************
* display_dr_init initializes the display.
* The function has no effect whatsoever if the display
* is already initialized (safe init)
* 	INPUT:
*		void.
*	OUTPUT:
*		void
*/
void display_dr_init();
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
void display_dr_on_off(bool on_off);
/*****************************************
*************display_dr_reset****************
******************************************
* display_dr_reset resets the display buffer,
* disables blinking, and sets brightness to maximum.
* 	INPUT:
*		void.
*	OUTPUT:
*		void.
*/
void display_dr_reset();
/***********************************
******display_dr_is_blinking_one****
************************************
* display_dr_is_blinking_one lets the user know whether the blinking function is enabled or not
* (for one specific position of the display). Blinking has a specific frequency set!
* 	INPUT:
*		void.
*	OUTPUT:
*		true when the blinking option is enabled for the specific position.
*		false when the position has its blinking option disabled.
*/
bool display_dr_is_blinking_one(int pos);
/***********************************
*********display_dr_blink_one****************
************************************
* display_dr_blink_one enables or disables the blinking function
* for one specific position of the display.
* 	INPUT:
* 		pos : position of the display for whcich the blink will be enabled or disabled.
*		on_off : true for enabling the blinking function. false for disabling it.
*	OUTPUT:
*		void.
*/
void display_dr_blink_one(int pos, bool on_off);
/***************************************
*********display_dr_write_sentence******
****************************************
* display_dr_write_sentence writes to the display buffer the first
* AMOUNT_MAX_DISPLAY_POS symbols of the parameter.
* 	INPUT:
*		sentence: pointer to the sentence to be written (should be null terminated).
*	OUTPUT:
*		number of symbols belonging to sentence that were written to the display buffer.
*		always <= AMOUNT_MAX_DISPLAY_POS
*/
int display_dr_write_sentence(const char* sentence);
/***********************************
*********display_dr_write_char******
************************************
* display_dr_write_char writes to the display buffer a given symbol in
* a given position of the display.
* 	INPUT:
*		c : symbol to be written
*		pos : position c will be written to.
*	OUTPUT:
*		void.
*/
void display_dr_write_char(char c, int pos);

/***********************************
*********display_clear_all**********
************************************
* display_dr_clear_all clears the display buffer.
* 	INPUT:
*		void.
*	OUTPUT:
*		void.
*/
void display_dr_clear_all();
/*****************************************
*********display_dr_clear_pos*************
******************************************
* display_dr_clear_pos clears a given position of the display's buffer (nothing will be shown when it is redrawn).
* 	INPUT:
*		pos : position that will be cleared.
*	OUTPUT:
*		void.
*/
void display_dr_clear_pos(int pos);
/*****************************************
*********display_dr_set_brightness_one*******
******************************************
* display_dr_set_brightness_one sets the brightness of a specific
* position of the display to a given level.
* 	INPUT:
* 		pos : position of the display for which brightness will be updated.
*		brightness : level of brightness, between MIN_BRIGHT and MAX_BRIGHT.
*					 an incorrect level of brightness will be ignored.
*	OUTPUT:
*		void.
*/
void display_dr_set_brightness_one(int pos, int brightness);
/*****************************************
*********display_dr_get_brightness_one****
******************************************
* display_dr_get_brightness_one get the brightness level of a specific position of the display
* 	INPUT:
* 		pos : position of the display.
*	OUTPUT:
*		brightness level.
*/
int display_dr_get_brightness_one(int pos);
/*****************************************
******************display_dr_shift********
******************************************
* display_dr_shift shifts the sentence that is currently displaying on the
* display according to the direction indicated, and inserting a given symbol
* on the position that would be left blank (left position for right direction and viceversa).
* 	INPUT:
*		dir : direction for the shifting operation
*		to_insert : symbol to be inserted
*	OUTPUT:
*		void.
*/
void display_dr_shift(direction_display_t dir, char to_insert);

/*********************************************************
*******display_dr_get_currently_curr_displaying_word******
**********************************************************
* display_dr_get_currently_curr_displaying_word returns a string (null terminated word!!!)
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
char* display_dr_get_currently_curr_displaying_word();
/***************************************************
*******display_dr_get_currently_on_buffer_word******
****************************************************
* display_dr_get_currently_on_buffer_word returns a string (null terminated word!!!)
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
char* display_dr_get_currently_on_buffer_word();



#endif /* DISPLAY_DISPLAY_DRIVER_H_ */
