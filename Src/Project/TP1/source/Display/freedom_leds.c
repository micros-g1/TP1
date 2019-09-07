/*
 * freedom_leds.c
 *
 *  Created on: 7 sep. 2019
 *      Author: Tomas
 */
#include "freedom_leds.h"
#include <Interrupts/SysTick.h>
#include "leds.h"
#include "gpio.h"
#include "MK64F12.h"
#include "board.h"

/*-------------------------------------------
 ----------------DEFINES---------------------
 -------------------------------------------*/
#define CALL_FREQ_HZ 200U
#define COUNTER_FREQ 	SYSTICK_ISR_FREQUENCY_HZ/CALL_FREQ_HZ-1

/*-------------------------------------------
 ----------GLOBAL_VARIABLES------------------
 -------------------------------------------*/
static bool blinking;				//blinking (true) or not (false)
static bool blink_cleared;		//status of the blinking (showing:true, not showing: false)
static int brightness;			//brightness level, from MIN_BRIGHT to MAX_BRIGHT
static int blink_counter_vel = CALL_FREQ_HZ/2;	//half a second.
static bool curr_displaying;			//leds buffer. Initialized at leds_init()
static frdm_led_colors_t curr_color;					//current pin to write on
/*-------------------------------------------
 ----------STATIC_FUNCTION_DECLARATION-------
 -------------------------------------------*/
static bool handle_blinking();
static bool handle_brightness();
static void frdm_led_draw_callback();
static void draw_frdm_led(bool on_off);
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
	if( !handle_blinking() || !handle_brightness())
		draw_frdm_led(false);
	else
		draw_frdm_led(curr_displaying);
}
static void draw_frdm_led(bool on_off){
	switch (curr_color){
		case FRDM_RED:
			gpioWrite(PIN_LED_RED, on_off);
		case FRDM_GREEN:
			gpioWrite(PIN_LED_GREEN, on_off);
		case FRDM_BLUE:
			gpioWrite(PIN_LED_BLUE, on_off);
	}
}
void frdm_led_dr_reset(){
	curr_displaying = false;
	blinking = false;
	blink_cleared = false;
	brightness = MAX_BRIGHT_FRDM_LED;
	curr_color = PIN_LED_BLUE;
}

void frdm_led_dr_blink(bool on_off){
	blinking = on_off;
	blink_cleared = false;
}

void frdm_led_dr_write(bool on_off){
	curr_displaying = on_off;
}

bool frdm_led_dr_is_blinking(){
	return blinking;
}

void frdm_led_dr_set_color(frdm_led_colors_t color){
	curr_color = color;
}

static bool handle_blinking(){
	static int blink_counter = false;

	if(!blinking) return true;		//not currently blinking, so should not handle blinking.

	if(blink_counter < blink_counter_vel)
		blink_counter++;				//the time has not come for the blinker to blink
	else{
		blink_counter = 0;
		blink_cleared = !blink_cleared;
	}

	return !blink_cleared;
}
static bool handle_brightness(){
	static int bright_counter = false;

	bool should_show = true;
	if(brightness < MAX_BRIGHT_FRDM_LED)
		if((bright_counter++) == brightness){
			should_show = false;
			bright_counter = 0;
		}
	return should_show;
}

//brightness from MIN_BRIGHT_LED to MAX_BRIGHT_LED
void frdm_led_dr_set_brightness(int bright){
	if( (bright < MAX_BRIGHT_LED) && (bright >= MIN_BRIGHT_LED) )
		brightness = bright;
}

int frdm_led_dr_get_brightness(){
	return brightness;
}
