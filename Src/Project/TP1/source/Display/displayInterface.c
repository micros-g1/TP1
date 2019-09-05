/*
 * displayInterface.c
 *
 *  Created on: 30 ago. 2019
 *      Author: Tomas
 */


#include "displayInterface.h"
#include "displayDriver.h"
#include "SysTick.h"
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
static direction marquee_curr_dir;
//static int marquee_curr_vel = 0;
static display_info_t info;
static inform_event_callback_t event_callback = NULL;
/*-------------------------------------------
 ---------STATIC_FUNCTION_DECLARATION--------
 -------------------------------------------*/
static void marquee_callback();
static void clear_marquee_buffer();
static void set_marquee_buffer(char* sentence);
static void set_inform_event_callback(inform_event_callback_t callback);
/*-------------------------------------------
 ----------FUNCTION_IMPLEMENTATION-----------
 -------------------------------------------*/

void init_display_interface(inform_event_callback_t callback){
	static bool initialized = false;

	info.number_of_pos = AMOUNT_MAX_DISPLAY_POS;
	if(initialized) return;
	display_init();
	clear_marquee_buffer();
	set_inform_event_callback(callback);
	systick_init();
	systick_add_callback(marquee_callback, 10000, PERIODIC);
	systick_disable_callback(marquee_callback);
	initialized = true;
}

void marquee(char* sentence, direction dir){
	//FALTA TEMA TIEMPOOOOS!!!
	clear_marquee_buffer();
	set_marquee_buffer(sentence);
	marquee_curr_dir = dir;
	//marquee_curr_vel = vel;
	//set vel for sysTick!!!
	systick_enable_callback(marquee_callback);
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
	static bool should_shift = false;
	static int chars_written = 0;
	static int finishing_shift_counter = 0;

	if(!should_shift){
		if( ( chars_written = write_sentence(marquee_buffer) ) == AMOUNT_MAX_DISPLAY_POS )
			if( marquee_buffer[chars_written+1] != NULL_CHAR )
				should_shift = true;
	}
	else if(marquee_buffer[chars_written+1] != NULL_CHAR){
		shift(marquee_curr_dir, marquee_buffer[chars_written+1]);
		chars_written++;
	}
	else if(finishing_shift_counter < AMOUNT_MAX_DISPLAY_POS){
		shift(marquee_curr_dir, NULL_CHAR);
		finishing_shift_counter++;
	}
	else if(finishing_shift_counter == AMOUNT_MAX_DISPLAY_POS){
		finishing_shift_counter = 0;
		should_shift = false;
		chars_written = 0;
		display_event_t event;
		event.id = MARQUEE_FINISHED;
		event_callback(event);
		systick_disable_callback(marquee_callback);
	}
}

static void set_inform_event_callback(inform_event_callback_t callback){
	event_callback = callback;
}

void set_blinking_one(bool on_off, int pos){
	blink_one(pos, on_off);
}
bool get_is_blinking(int pos){
	return is_blinking_one(pos);
}
bool get_is_blinking_any(){
	bool blinking_any = false;
	for(int i = 0; i < AMOUNT_MAX_DISPLAY_POS; i++)
		if( (blinking_any = is_blinking_one(i)) )
			break;

	return blinking_any;
}
void set_blinking_all(bool on_off){
	for(int i = 0; i < AMOUNT_MAX_DISPLAY_POS; i++)
		blink_one(i, on_off);
}
void write_word_2_display(const char* word){
	write_sentence(word);
}

display_info_t get_display_info(){
	return info;
}


