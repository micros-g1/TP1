/*
 * rotary_encoder.h
 *
 *  Created on: 3 Sep 2019
 *      Author: grein
 */

#ifndef ROTARY_ENCODER_ROTARY_ENCODER_H_
#define ROTARY_ENCODER_ROTARY_ENCODER_H_

//Rotary Encoder Event
typedef enum {RE_LEFT,RE_RIGHT,RE_SHORT_CLICK,RE_LONG_CLICK_1,RE_LONG_CLICK_2,RE_KEY_LEFT,RE_KEY_RIGHT,RE_NO_EVENT} re_event_t;
//Description of events:
//
//RE_LEFT: Rotary encoder idle, turn left
//RE_RIGHT: Rotary encoder idle, turn right
//RE_SHORT_CLICK: Rotary encoder idle, press down, release before time T
//RE_LONG_CLICK_1: Rotary encoder idle, press down, wait longer than T
//RE_LONG_CLICK_2: Rotary encoder idle, press down, wait longer than 2T
//RE_KEY_LEFT: Rotary encoder idle, press down, rotate left before time T - like a key #
//RE_KEY_RIGHT: Rotary encoder idle, press down, rotate right before time T - like a key #
//Notes:
//* # Further left or right rotations while pressed down, it will generate
//		ID_KEY_LEFT and ID_KEY_RIGHT, respectively
//* If a RE_KEY_LEFT or RE_KEY_RIGHT is generated then RE_SHORT_CLICK, RE_LONG_CLICK_1 and
//		RE_LONG_CLICK_2 will not be generated, and button must be released before those events
//		can be generated.
//* If RE_LONG_CLICK_1 is generated then RE_KEY_LEFT and RE_KEY_RIGHT cannot be generated and
//		button must be released before these events can be generated.

//Driver Callback Function Definition
typedef void (*rotary_encoder_callback_t)(re_event_t);


//Initialize Rotary Encoder
void rotary_encoder_init();

//Set Rotary Encoder Callback (will not be called with RE_NO_EVENT)
void rotary_encoder_set_callback(rotary_encoder_callback_t callback);

#endif /* ROTARY_ENCODER_ROTARY_ENCODER_H_ */
