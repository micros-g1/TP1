/*
 * displayDriver.h
 *
 *  Created on: 30 ago. 2019
 *      Author: Tomas
 */

#ifndef DISPLAY_DISPLAYDRIVER_H_
#define DISPLAY_DISPLAYDRIVER_H_

#include "board.h"
#define AMOUNT_MAX_DISPLAY_POS		4

void init_display(void);
void write_char(char c, int pos);
void display_draw_callback();
void clear_all();
void clear_pos(int pos);
#endif /* DISPLAY_DISPLAYDRIVER_H_ */
