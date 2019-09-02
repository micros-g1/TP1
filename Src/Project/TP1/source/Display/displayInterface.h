/*
 * displayInterface.h
 *
 *  Created on: 30 ago. 2019
 *      Author: Tomas
 */

#ifndef DISPLAY_DISPLAYINTERFACE_H_
#define DISPLAY_DISPLAYINTERFACE_H_
#include "general.h"
void marquee(char* str, direction dir, int vel);
void clear_all();
void write_word(char word[4]);
void blink_one(int pos, int vel);

#endif /* DISPLAY_DISPLAYINTERFACE_H_ */
