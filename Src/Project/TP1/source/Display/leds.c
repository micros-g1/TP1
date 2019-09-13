/*
 * leds.c
 *
 *  Created on: 5 sep. 2019
 *      Author: Tomas
 */

#ifndef DISPLAY_LEDS_C_
#define DISPLAY_LEDS_C_
#include <gpio.h>
#include <Interrupts/SysTick.h>
#include "leds.h"
#include "MK64F12.h"
#include "board.h"
/*-------------------------------------------
 ----------------DEFINES---------------------
 -------------------------------------------*/
#define CALL_FREQ_HZ 2000U
#define COUNTER_FREQ 	SYSTICK_ISR_FREQUENCY_HZ/CALL_FREQ_HZ-1
#if SYSTICK_ISR_FREQUENCY_HZ % CALL_FREQ_HZ != 0
#warning BLINK cannot implement this exact frequency. Using floor(SYSTICK_ISR_FREQUENCY_HZ/CALL_FREQ_HZ) instead.
#endif
#define DISPLAY_MAX_BRIGHT	5
#define DISPLAY_MIN_BRIGHT	0
/*-------------------------------------------
 ----------GLOBAL_VARIABLES------------------
 -------------------------------------------*/
static bool blinking[AMOUNT_MAX_DIODES_POS];				//blinking (true) or not (false)
static bool blink_cleared[AMOUNT_MAX_DIODES_POS];		//status of the blinking (showing:true, not showing: false)
static int brightness[AMOUNT_MAX_DIODES_POS];			//brightness level, from MIN_BRIGHT to MAX_BRIGHT
static int blink_counter_vel = CALL_FREQ_HZ/AMOUNT_MAX_DIODES_POS/2;	//half a second.
static bool curr_displaying[AMOUNT_MAX_DIODES_POS];			//leds buffer. Initialized at leds_init()
static int bright_counter[AMOUNT_MAX_DIODES_POS];
/*-------------------------------------------
 ----------STATIC_FUNCTION_DECLARATION-------
 -------------------------------------------*/
static void swap_bools(bool * sw1, bool *sw2);
static bool handle_blinking(int pos);
static void draw_diode(int pos, bool on_off);
static void draw_leds(int pos);
static bool handle_brightness(int pos);
static void led_draw_callback();

/*-------------------------------------------
 ------------FUNCTION_IMPLEMENTATION---------
 -------------------------------------------*/
static void draw_leds(int pos){

	if( !handle_blinking(pos) || !handle_brightness(pos))
		draw_diode(pos, false);
	else
		draw_diode(pos, (int)curr_displaying[pos]);
}
static void draw_diode(int pos, bool on_off){

	//could optimize if necessary
	pos++; // 1 2 3
	gpioWrite (PIN_DIODE_0, on_off && (pos & 0X1));
	gpioWrite (PIN_DIODE_1, on_off && (pos & 0X2));

}
void led_dr_write(int pos, bool on_off){
	if( pos < AMOUNT_MAX_DIODES_POS)
		curr_displaying[pos] = on_off;
}
void led_dr_init(){
	static bool initialized = false;

	if(initialized) return;

	gpioMode(PIN_DIODE_0, OUTPUT);
	gpioMode(PIN_DIODE_1, OUTPUT);
	led_dr_reset();

	systick_init();
	systick_add_callback(led_draw_callback, COUNTER_FREQ, PERIODIC);

	initialized = true;
}

void led_dr_reset(){
	for(int i = 0; i < AMOUNT_MAX_DIODES_POS; i++){
		curr_displaying[i] = false;
		blinking[i] = false;
		blink_cleared[i] = false;
		bright_counter[i] = 0;
		brightness[i] = MAX_BRIGHT_LED;
	}
}

static void led_draw_callback(){
	static int pos = 0;
	//first, clear display in order to prevent bleeding
	draw_diode(pos, false);
	//now, increment diode pointer
	pos = (pos == AMOUNT_MAX_DIODES_POS-1) ? 0 : pos + 1;
	//finally, draw leds
	draw_leds(pos);
}

//brightness from MIN_BRIGHT_LED to MAX_BRIGHT_LED
void led_dr_set_brightness_one(int pos, int bright){
	if( (bright < MAX_BRIGHT_LED) && (bright >= MIN_BRIGHT_LED) )
		brightness[pos] = bright;
}
void led_dr_blink(int pos, bool on_off){
	blinking[pos] = on_off;
	blink_cleared[pos] = false;
}
bool led_dr_is_blinking_one(int pos){
	return blinking[pos];
}

static bool handle_blinking(int pos){
	static int blink_counter[AMOUNT_MAX_DIODES_POS] = {0, 0, 0};

	if(!blinking[pos]) return true;		//not currently blinking, so should not handle blinking.

	if(blink_counter[pos] < blink_counter_vel)
		blink_counter[pos]++;				//the time has not come for the blinker to blink
	else{
		blink_counter[pos] = 0;
		blink_cleared[pos] = !blink_cleared[pos];
	}

	return !blink_cleared[pos];
}
void led_dr_shift(direction_led_t dir, bool to_insert){
	if(dir == LED_LEFT)
		for(int i = AMOUNT_MAX_DIODES_POS-1; i >= 0; i--)
			swap_bools(&curr_displaying[i], &to_insert);
	else
		for(int i = 0; i < AMOUNT_MAX_DIODES_POS; i++)
			swap_bools(&curr_displaying[i], &to_insert);
}
static void swap_bools(bool * sw1, bool *sw2){
	bool aux = *sw1;
	*sw1 = *sw2;
	*sw2 = aux;
}
void led_dr_on_off(bool on_off){
	if(!on_off)
		systick_disable_callback(led_draw_callback);
	else
		systick_enable_callback(led_draw_callback);
}
static bool handle_brightness(int pos){


	bool should_show = true;
	if(brightness[pos] < MAX_BRIGHT_LED)
		if((bright_counter[pos]++) == brightness[pos]){
			should_show = false;
			bright_counter[pos] = 0;
		}
	return should_show;
}

int led_dr_get_brightness_one(int pos){
	if(pos < AMOUNT_MAX_DIODES_POS)
		return brightness[pos];
	else
		return MIN_BRIGHT_LED;
}

#endif /* DISPLAY_LEDS_C_ */
