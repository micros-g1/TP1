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

display_event_t get_last_event();

void marquee(char* sentence, direction dir);
void clear_all();

void write_word_2_display(const char* word);

void set_blinking_all(bool on_off);
bool get_is_blinking_any();
void set_blinking_one(bool on_off, int pos);
bool get_is_blinking(int pos);
/*****************************************
*******init_display_interface*************
******************************************
* init_display_interface initializes the display.
* The function has no effect whatsoever if the display
* is already initialized.
* 	INPUT:
*		void.
*	OUTPUT:
*		void
*/
void init_display_interface(inform_event_callback_t callback);
void selectors_on_off(bool sel0 , bool sel1, bool sel2);
#endif /* DISPLAY_DISPLAYINTERFACE_H_ */
