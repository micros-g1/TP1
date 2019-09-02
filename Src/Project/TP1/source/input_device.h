/*
 * buttons.h
 *
 *  Created on: 2 Sep 2019
 *      Author: grein
 */

// Input device event source
#ifndef INPUT_DEVICE_H_
#define INPUT_DEVICE_H_

#include "event_queue.h"

#define INPUT_DEVICE_ISR_PERIOD_TICKS 	10U
#define INPUT_DEVICE_T_RELOAD_VALUE		100U

typedef enum {ID_LEFT,ID_RIGHT,ID_SHORT_CLICK,ID_LONG_CLICK_1,ID_LONG_CLICK_2,ID_KEY_LEFT,ID_KEY_RIGHT} idev_input_id;
//ID_LEFT: Rotary encoder idle, turn left
//ID_RIGHT: Rotary encoder idle, turn right
//ID_SHORT_CLICK: Rotary encoder idle, press down, release before time T
//ID_LONG_CLICK_1: Rotary encoder idle, press down, release after time T
//ID_LONG_CLICK_2: Rotary encoder idle, press down, release after time 2T (RE can turn after time T)
//ID_KEY_LEFT: Rotary encoder idle, press down, rotate left before time T - like a key *
//ID_KEY_RIGHT: Rotary encoder idle, press down, rotate right before time T - like a key *
//* Further left or right rotations will generate ID_KEY_LEFT and ID_KEY_RIGHT respectively

void input_device_init();
void input_device_ISR();

#endif /* INPUT_DEVICE_H_ */
