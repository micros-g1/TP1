/*
 * displayInterface.h
 *
 *  Created on: 30 ago. 2019
 *      Author: Tomas
 */

#ifndef DISPLAY_DISPLAYINTERFACE_H_
#define DISPLAY_DISPLAYINTERFACE_H_

#include "general.h"
#include <stdbool.h>

void marquee(char* sentence, direction dir);
void clear_all();
void write_word(char word[4]);
void set_blinking_all(bool on_off, int vel);
void set_blinking_one(bool on_off, int pos, int vel);
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
void init_display_interface();

#endif /* DISPLAY_DISPLAYINTERFACE_H_ */
