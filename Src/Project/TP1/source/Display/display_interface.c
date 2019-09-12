/*
 * displayInterface.c
 *
 *  Created on: 30 ago. 2019
 *      Author: Tomas
 */


#include <Interrupts/SysTick.h>
#include "displayInterface.h"
#include "displayDriver.h"
#include "leds.h"
#include "freedom_leds.h"
#include "stdlib.h"
/*-------------------------------------------
 ----------------DEFINES---------------------
 -------------------------------------------*/
#define MARQUEE_BUFFER_SIZE		20

/*-------------------------------------------
 ----------------GLOBAL_VARIABLES------------
 -------------------------------------------*/
static char marquee_buffer[MARQUEE_BUFFER_SIZE];
static int marquee_buffer_curr_size = 0;
static direction_display_t marquee_curr_dir;
//static int marquee_curr_vel = 0;
static display_info_t info;
static inform_event_callback_t event_callback = NULL;
static bool should_shift = false;				//boolean for marquee shifting
static int chars_written = 0;					//int for marquee shifting, amount of written chars counter.
static int finishing_shift_counter = 0;			//for marquee shifting, completing with null char counter.
static bool marquee_reset = false;				//boolean for reseting marquee shifting
/*-------------------------------------------
 ---------STATIC_FUNCTION_DECLARATION--------
 -------------------------------------------*/
//callback that will be called with each sysTick call when marquee has been enabled.
static void marquee_callback();
//clears the marquee buffer.
static void clear_marquee_buffer();
//sets the marquee buffer with the sentence that should be marqueed.
static void set_marquee_buffer(char* sentence);
//sets the callback that should be called when a new event has been generated by display.
static void set_inform_event_callback(inform_event_callback_t callback);
//gets last not null character from marquee buffer
static char look_for_last_not_null();
//removes last not null character from marquee buffer
static void remove_last_not_null();
/*-------------------------------------------
 ----------FUNCTION_IMPLEMENTATION-----------
 -------------------------------------------*/

void display_init_interface(inform_event_callback_t callback){
	static bool initialized = false;

	info.number_of_pos = AMOUNT_MAX_DISPLAY_POS;
	if(initialized) return;
	frdm_led_dr_init();
	led_dr_init();
	display_dr_init();

	clear_marquee_buffer();
	set_inform_event_callback(callback);
	systick_init();
	systick_add_callback(marquee_callback, SYSTICK_ISR_FREQUENCY_HZ, PERIODIC);
	systick_disable_callback(marquee_callback);
	initialized = true;
}

void display_marquee(char* sentence, direction_t dir){
	//FALTA TEMA TIEMPOOOOS!!!
	clear_marquee_buffer();
	set_marquee_buffer(sentence);
	marquee_curr_dir = (direction_display_t) dir;
	//marquee_curr_vel = vel;
	//set vel for sysTick!!!
	systick_enable_callback(marquee_callback);
}
void display_stop_marquee(){
	clear_marquee_buffer();
	systick_disable_callback(marquee_callback);
	marquee_reset = true;
}

static void clear_marquee_buffer(){
	for(int i = 0; i < AMOUNT_MAX_DISPLAY_POS; i++)
		marquee_buffer[i] = NULL_CHAR;
}

static void set_marquee_buffer(char* sentence){
	int i = 0;
	for(; i < MARQUEE_BUFFER_SIZE; i++)
		if(sentence[i] != NULL_CHAR)
			marquee_buffer[i] = sentence[i];
		else
			break;

	marquee_buffer_curr_size = i;
}
static void marquee_callback(){

	if(marquee_reset){
		should_shift = false;
		chars_written = 0;
		finishing_shift_counter = 0;
	}

	char aux = NULL_CHAR;

	if(!should_shift){
		if(marquee_curr_dir == DISPLAY_LEFT)
			chars_written = display_dr_write_sentence(marquee_buffer);
		else
			display_clear_all();
		should_shift = true;
	}
	else if((marquee_curr_dir == DISPLAY_LEFT) && marquee_buffer[chars_written] != NULL_CHAR){
		display_dr_shift(marquee_curr_dir, marquee_buffer[chars_written]);
		chars_written++;
	}
	else if((marquee_curr_dir == DISPLAY_RIGHT) && ((aux = look_for_last_not_null()) != NULL_CHAR) ){
		remove_last_not_null();
		display_dr_shift(marquee_curr_dir, aux);
	}
	else if(finishing_shift_counter < AMOUNT_MAX_DISPLAY_POS){
		display_dr_shift(marquee_curr_dir, NULL_CHAR);
		finishing_shift_counter++;
	}
	else if(finishing_shift_counter == AMOUNT_MAX_DISPLAY_POS){
		finishing_shift_counter = 0;
		should_shift = false;
		chars_written = 0;
		event_callback();
		systick_disable_callback(marquee_callback);
	}
	marquee_reset = false;
}

static void set_inform_event_callback(inform_event_callback_t callback){
	event_callback = callback;
}

void display_set_blinking_one(bool on_off, int pos){
	display_dr_blink_one(pos, on_off);
}
bool display_get_is_blinking(int pos){
	return display_dr_is_blinking_one(pos);
}
bool display_get_is_blinking_any(){
	bool blinking_any = false;
	for(int i = 0; i < AMOUNT_MAX_DISPLAY_POS; i++)
		if( (blinking_any = display_dr_is_blinking_one(i)) )
			break;

	return blinking_any;
}
void display_set_blinking_all(bool on_off){
	for(int i = 0; i < AMOUNT_MAX_DISPLAY_POS; i++)
		display_dr_blink_one(i, on_off);
}
void display_write_word(const char* word){
	display_dr_write_sentence(word);
}
void display_write_char(char c, int pos){
	if(pos < AMOUNT_MAX_DISPLAY_POS)
		display_dr_write_char(c, pos);
}
display_info_t get_display_info(){
	return info;
}
void display_clear_all(){
	display_dr_clear_all();
}
static char look_for_last_not_null(){
	for(int i = MARQUEE_BUFFER_SIZE-1; i >=0; i--)
		if(marquee_buffer[i] != NULL_CHAR)
			return marquee_buffer[i];

	return NULL_CHAR;

}

static void remove_last_not_null(){
	for(int i = MARQUEE_BUFFER_SIZE-1; i >=0; i--)
			if(marquee_buffer[i] != NULL_CHAR){
				marquee_buffer[i] = NULL_CHAR;
				break;
			}
}
void display_clear_all_leds(){
	for(int i =0; i < AMOUNT_MAX_DIODES_POS; i++)
		led_dr_write(i, false);
}
void display_write_to_led(int pos, bool on_off){
	led_dr_write(pos, on_off);
}
void display_set_blinking_leds_all(bool on_off){
	for(int i =0; i < AMOUNT_MAX_DIODES_POS; i++)
		led_dr_blink(i, on_off);
}
bool display_get_is_blinking_leds_any(){
	bool any = false;
	for(int i =0; i < AMOUNT_MAX_DIODES_POS; i++)
		if( (any = led_dr_is_blinking_one(i)) )
			break;
	return any;
}
bool display_get_blinking_led_one(int pos){
	return led_dr_is_blinking_one(pos);
}
void display_set_blinking_led_one(int pos, bool on_off){
	led_dr_blink(pos, on_off);
}
void display_set_brightness_one_up_down(int pos, direction_t dir){
	if(dir == UP)
		display_set_brightness_one_pos(pos, display_dr_get_brightness_one(pos) + 1);
	else if(dir == DOWN)
		display_set_brightness_one_pos(pos, display_dr_get_brightness_one(pos) - 1);
}

void display_set_brightnes_led(int pos, int brightness){
	led_dr_set_brightness_one(pos, brightness);
}
void display_set_brightness_leds(int brightness){
	for(int i =0; i < AMOUNT_MAX_DIODES_POS; i++)
		led_dr_set_brightness_one(i, brightness);
}
void display_set_brightness_led_up_down(int pos, direction_t dir){
	if(dir == UP)
		led_dr_set_brightness_one(pos, led_dr_get_brightness_one(pos) + 1);
	else if(dir == DOWN)
		led_dr_set_brightness_one(pos, led_dr_get_brightness_one(pos) - 1);
}
void display_set_brightness_leds_up_down(direction_t dir){
	for (int i =0 ; i < AMOUNT_MAX_DIODES_POS; i++)
		display_set_brightness_led_up_down(i, dir);
}
void display_set_brightness_up_down(direction_t dir){
	for (int i =0 ; i < AMOUNT_MAX_DISPLAY_POS; i++)
		display_set_brightness_one_up_down(i,dir);
}
void display_set_brightness_one_pos(int pos, int brightness){
	display_dr_set_brightness_one(pos, brightness);
}

void display_write_or_marquee(char * sentence, direction_t dir){
	int i =0;
	for( ; ; i++)
		if(sentence[i] == NULL_CHAR)
			break;
	if(i >= AMOUNT_MAX_DISPLAY_POS)
		display_marquee(sentence, dir);
	else
		display_dr_write_sentence(sentence);
}

void display_set_brightness_frdm_led_up_down(color_t color, direction_t dir){
	if(dir == UP)
		frdm_led_dr_set_brightness(color, frdm_led_dr_get_brightness(color) + 1);
	else if(dir == DOWN)
		frdm_led_dr_set_brightness(color, frdm_led_dr_get_brightness(color) - 1);
}
void display_frdm_led_reset(){
	frdm_led_dr_reset();
}

void display_frdm_led_blink(color_t color, bool on_off){
	frdm_led_dr_blink(color, on_off);
}

void display_frdm_led_write(color_t color, bool on_off){
	frdm_led_dr_write(color, on_off);
}

bool display_frdm_led_is_blinking(color_t color){
	return frdm_led_dr_is_blinking(color);
}


void display_set_brightness_frdm_led(color_t color, int brightness){
	frdm_led_dr_set_brightness(color, brightness);
}