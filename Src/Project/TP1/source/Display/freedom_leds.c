/*
 * freedom_leds.c
 *
 *  Created on: 7 sep. 2019
 *      Author: Tomas
 */
#include <gpio.h>
#include "freedom_leds.h"
#include <Interrupts/SysTick.h>
#include "leds.h"
#include "MK64F12.h"
#include "board.h"

/*-------------------------------------------
 ----------------DEFINES---------------------
 -------------------------------------------*/
#define CALL_FREQ_HZ 800U
#define COUNTER_FREQ 	SYSTICK_ISR_FREQUENCY_HZ/CALL_FREQ_HZ-1

/*-------------------------------------------
 ----------GLOBAL_VARIABLES------------------
 -------------------------------------------*/
static bool blinking[AMOUNT_MAX_FRDM_LEDS_POS];				//blinking (true) or not (false)
static bool blink_cleared[AMOUNT_MAX_FRDM_LEDS_POS];		//status of the blinking (showing:true, not showing: false)
static int brightness[AMOUNT_MAX_FRDM_LEDS_POS];			//brightness level, from MIN_BRIGHT to MAX_BRIGHT
static int blink_counter_vel = CALL_FREQ_HZ/AMOUNT_MAX_FRDM_LEDS_POS/2;	//half a second.
static bool curr_displaying[AMOUNT_MAX_FRDM_LEDS_POS];			//leds buffer. Initialized at leds_init()
/*-------------------------------------------
 ----------STATIC_FUNCTION_DECLARATION-------
 -------------------------------------------*/
static bool handle_blinking(frdm_led_colors_t color);
static bool handle_brightness(frdm_led_colors_t color);
static void frdm_led_draw_callback();
static void frdm_draw_leds(int pos);
static void draw_frdm_led(frdm_led_colors_t color, bool on_off);
/*-------------------------------------------
 ------------FUNCTION_IMPLEMENTATION---------
 -------------------------------------------*/
void frdm_led_dr_init(){
	static bool initialized = false;

	if(initialized) return;

	gpioMode(PIN_LED_RED, OUTPUT);
	gpioMode(PIN_LED_GREEN, OUTPUT);
	gpioMode(PIN_LED_BLUE, OUTPUT);

	frdm_led_dr_reset();

	systick_init();
	systick_add_callback(frdm_led_draw_callback, CALL_FREQ_HZ, PERIODIC);

	initialized = true;
}
static void frdm_led_draw_callback(){
	static int pos = 0;

	frdm_draw_leds(pos);
	pos = (pos == (AMOUNT_MAX_FRDM_LEDS_POS-1)) ? 0 : pos+1;
}

static void frdm_draw_leds(int pos){
	if( !handle_blinking(pos) || !handle_brightness(pos))
		draw_frdm_led(pos, false);
	else
		draw_frdm_led(pos, (int)curr_displaying[pos]);
}

static void draw_frdm_led(frdm_led_colors_t color, bool on_off){
	switch (color){
		case FRDM_RED:
			gpioWrite(PIN_LED_RED, on_off);
		case FRDM_GREEN:
			gpioWrite(PIN_LED_GREEN, on_off);
		case FRDM_BLUE:
			gpioWrite(PIN_LED_BLUE, on_off);
	}
}
void frdm_led_dr_reset(){
	for(int i =0; i < AMOUNT_MAX_FRDM_LEDS_POS; i++){
		curr_displaying[i] = false;
		blinking[i] = false;
		blink_cleared[i] = false;
		brightness[i] = MAX_BRIGHT_FRDM_LED;
	}
}

void frdm_led_dr_blink(frdm_led_colors_t color, bool on_off){
	blinking[color] = on_off;
	blink_cleared[color] = false;
}

void frdm_led_dr_write(frdm_led_colors_t color, bool on_off){
	curr_displaying[(int)color] = on_off;
}

bool frdm_led_dr_is_blinking(frdm_led_colors_t color){
	return blinking[(int)color];
}

static bool handle_blinking(frdm_led_colors_t color){
	static int blink_counter[AMOUNT_MAX_FRDM_LEDS_POS] = {0, 0, 0};

	if(!blinking[(int)color]) return true;		//not currently blinking, so should not handle blinking.

	if(blink_counter[(int)color] < blink_counter_vel)
		blink_counter[(int)color]++;				//the time has not come for the blinker to blink
	else{
		blink_counter[(int)color] = 0;
		blink_cleared[(int)color] = !blink_cleared[(int)color];
	}

	return !blink_cleared[(int)color];
}
static bool handle_brightness(frdm_led_colors_t color){
	static int bright_counter[AMOUNT_MAX_FRDM_LEDS_POS] = {0, 0, 0};
	int pos = (int) color;

	bool should_show = true;
	if(brightness[pos] < MAX_BRIGHT_FRDM_LED)
		if((bright_counter[pos]++) == brightness[pos]){
			should_show = false;
			bright_counter[pos] = 0;
		}
	return should_show;
}

//brightness from MIN_BRIGHT_LED to MAX_BRIGHT_LED
void frdm_led_dr_set_brightness(frdm_led_colors_t color, int bright){
	if( (bright < MAX_BRIGHT_LED) && (bright >= MIN_BRIGHT_LED) )
		brightness[(int)color] = bright;
}

int frdm_led_dr_get_brightness(frdm_led_colors_t color){
	return brightness[(int)color];
}
