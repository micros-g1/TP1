/*
 * displayInterface.c
 *
 *  Created on: 30 ago. 2019
 *      Author: Tomas
 */


#include "displayInterface.h"
#include "displayDriver.h"
#include "SysTick.h"

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

/*-------------------------------------------
 ---------STATIC_FUNCTION_DECLARATION--------
 -------------------------------------------*/
static void marquee_callback();
static void clear_marquee_buffer();
static void set_marquee_buffer(char* sentence);
/*-------------------------------------------
 ----------FUNCTION_IMPLEMENTATION-----------
 -------------------------------------------*/
//void marquee(char* sentence, direction dir, int vel)
void marquee(char* sentence, direction dir){
	//FALTA TEMA TIEMPOOOOS!!!
	clear_marquee_buffer();
	set_marquee_buffer(sentence);
	marquee_curr_dir = dir;
	//marquee_curr_vel = vel;
	//set vel for sysTick!!!
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
	//bool should_shift = false;
	//int chars_written;
	/*
	if( ( chars_written = write_sentence(sentence) ) == AMOUNT_MAX_DISPLAY_POS ){
		if( sentence[chars_written+1] != NULL_CHAR ){
			should_shift = true;
			shift(marquee_curr_dir, sentence[chars_written+1]);
		}
		else
			for(int i = 0; i < AMOUNT_MAX_DISPLAY_POS; i++)
				shift(dir, NULL_CHAR);
	}
	*/
}

void init_display_interface(){
	static bool initialized = false;

	if(initialized) return;
	init_display();
	clear_marquee_buffer();
	systick_init();
	//systick_add_callback(marquee_callback, 100);
	initialized = true;
}

void set_blinking_one(bool on_off, int pos, int vel){

}

void set_blinking_all(bool on_off, int vel){
	for(int i = 0; i < AMOUNT_MAX_DISPLAY_POS; i++)
		set_blinking_one(on_off, i, vel);
}
