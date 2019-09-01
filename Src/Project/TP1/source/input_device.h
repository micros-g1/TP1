/*
 * input_device.h
 *
 *  Created on: 30 Aug 2019
 *      Author: grein
 */

#ifndef INPUT_DEVICE_H_
#define INPUT_DEVICE_H_

#define BUTTONS_QUANTITY 3
typedef enum {BTN_OK,BTN_LEFT,BTN_RIGHT} button_id_t;

//Initialize input device
void input_device_init();

//Get button state
int input_device_get_button_state(button_id_t id);

//ISR for reading buttons
void input_device_ISR();

#endif /* INPUT_DEVICE_H_ */
