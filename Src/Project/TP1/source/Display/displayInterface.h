/*
 * displayInterface.h
 *
 *  Created on: 30 ago. 2019
 *      Author: Tomas
 */

#ifndef DISPLAY_DISPLAYINTERFACE_H_
#define DISPLAY_DISPLAYINTERFACE_H_

#include "general.h"

typedef struct {
	int number_of_pos;
}display_info_t;

typedef enum{NO_EVENT, MARQUEE_FINISHED} display_event_id;
typedef struct {
	display_event_id id;
}display_event_t;
typedef void (*inform_event_callback_t)(display_event_t ev);


/*****************************************
*****************marquee******************
******************************************
* marquee starts the marquee of a specific
* 	INPUT:
*		sentence : null terminated string to be shown on the display.
*		dir : direction of the marquee.
*	OUTPUT:
*		void.
*/
void marquee(char* sentence, direction dir);
/*****************************************
*****************clear_all****************
******************************************
* clear_all tells clears the entire display (does not include LEDS)
* 	INPUT:
*		void.
*	OUTPUT:
*		void.
*/
void clear_all();
/*****************************************
*************write_word_2_display*********
******************************************
* write_word_2_display writes the first AMOUNT_MAX_DISPLAY_POS letters of a word
* into the displays buffer.
* 	INPUT:
*		word : null terminated string to be written on the display.
*	OUTPUT:
*		void.
*/
void write_word_2_display(const char* word);
/*****************************************
*************set_blinking_all*************
******************************************
* set_blinking_all sets all positions of the display on the specified blinking state
* 	INPUT:
* 		on_off : true for enabling blinking, false otherwise.
*	OUTPUT:
*		void.
*/
void set_blinking_all(bool on_off);
/*****************************************
*******get_is_blinking_any****************
******************************************
* get_is_blinking_any tells the user if there's at least one
* display position that's currently blinking.
* 	INPUT:
*		void.
*	OUTPUT:
*		true if at least one position is currently blinking enabled.
*/
bool get_is_blinking_any();
/*****************************************
*************set_blinking_one*************
******************************************
* set_blinking_one tells sets the blinking state of a specific position
* of the display to enabled or disabled
* 	INPUT:
* 		on_off : true for enabling blinking for the position, false otherwise.
*		pos : position of the display that will be enabled or disabled for blinking
*	OUTPUT:
*		void
*/
void set_blinking_one(bool on_off, int pos);
/*****************************************
**************get_is_blinking*************
******************************************
* get_is_blinking tells the user if a specific position of the
* display is currently blinking.
* 	INPUT:
*		pos : position of interest.
*	OUTPUT:
*		true if at the position is currently blinking enabled.
*/
bool get_is_blinking(int pos);
/*****************************************
*******init_display_interface*************
******************************************
* init_display_interface initializes the display.
* The function has no effect whatsoever if the display
* is already initialized (safe init)
* 	INPUT:
*		void.
*	OUTPUT:
*		void
*/
void init_display_interface(inform_event_callback_t callback);
#endif /* DISPLAY_DISPLAYINTERFACE_H_ */
