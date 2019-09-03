/*
 * displayDriver.c
 *
 *  Created on: 30 ago. 2019
 *      Author: Tomas
 */

#include "displayDriver.h"
#include "gpio.h"
#include "MK64F12.h"
#include "SysTick.h"


/*-------------------------------------------
 ----------------DEFINES---------------------
 -------------------------------------------*/
//CALL_FREQ_HZ lower than 80U flickers! (with SYSTICK_ISR_FREQUENCY_HZ 1000U)
#define CALL_FREQ_HZ 500U
#define COUNTER_FREQ 	SYSTICK_ISR_FREQUENCY_HZ/CALL_FREQ_HZ-1
#if SYSTICK_ISR_FREQUENCY_HZ % CALL_FREQ_HZ != 0
#warning BLINK cannot implement this exact frequency. Using floor(SYSTICK_ISR_FREQUENCY_HZ/CALL_FREQ_HZ) instead.
#endif

#define NULL_TERMINATOR 0 //for strings
#define AMOUNT_SEGS	7
#define AMOUNT_POSSIBLE_CHAR	128

#define SHIFT_BUFFER_LENGTH		20
/*-------------------------------------------
 ----------GLOBAL_VARIABLES------------------
 -------------------------------------------*/
static bool blinking = false;					//blinking (true) or not (false)
static bool blink_cleared = false;				//status of the blinking (showing:true, not showing: false)

static int brightness = MAX_BRIGHT;			//brightness level, from MIN_BRIGHT to MAX_BRIGHT
static int blink_counter_vel = SYSTICK_ISR_FREQUENCY_HZ/2;

static bool display_on = true;			//display on:true, off: false.

static unsigned char curr_displaying[AMOUNT_MAX_DISPLAY_POS];			//display buffer. Initialized at init_display()
static char curr_displaying_photo[AMOUNT_MAX_DISPLAY_POS+1];			//display buffer at the moment get_currently_on_buffer_word was called. null terminated

static char last_drawn_word[AMOUNT_MAX_DISPLAY_POS]; //last drawn word Initialized at init_display().
static char last_drawn_word_photo[AMOUNT_MAX_DISPLAY_POS + 1]; //last drawn word at the moment get_currently_curr_displaying_word was called. null terminated

//https://en.wikichip.org/wiki/seven-segment_display/representing_letters
static const unsigned char seven_seg_chars[AMOUNT_POSSIBLE_CHAR]= {
/*  0     1     2     3     4     5     6     7     8     9  */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/*  10    11    12    13    14    15    16    17    18    19  */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/*  20    21    22    23    24    25    26    27    28    29  */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/*  30    31    32    33    34    35    36    37    38    39  */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/*  40    41    42    43    44    45    46    47   */
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/*  0     1     2     3     4     5     6     7     8     9     :     ;     */
	0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x70, 0x7F, 0x7B, 0x00, 0x00,
/*  <     =     >     ?     @     A     B     C     D     E     F     G     */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x00, 0x4E, 0x00, 0x4F, 0x47, 0x5E,
/*  H     I     J     K     L     M     N     O     P     Q     R     S     */
	0x37, 0x06, 0x3C, 0x00, 0x0E, 0x00, 0x00, 0x7E, 0x67, 0x00, 0x00, 0x5B,
/*  T     U     V     W     X     Y     Z     [     \     ]     ^     _     */
	0x00, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/*  `     a     b     c     d     e     f     g     h     i     j     k     */
	0x00, 0x00, 0x1F, 0x0D, 0x3D, 0x00, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00,
/*  l     m     n     o     p     q     r     s     t     u     v     w     */
	0x00, 0x00, 0x15, 0x1D, 0x00, 0x73, 0x05, 0x00, 0x0F, 0x1C, 0x00, 0x00,
/*  x     y     z     {		|	  }		~	  cuadradito*/
    0x00, 0x6E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static const unsigned int char_pins[AMOUNT_SEGS] = {PIN_DISPLAY_CHAR0 ,PIN_DISPLAY_CHAR1,PIN_DISPLAY_CHAR2,PIN_DISPLAY_CHAR3,
			PIN_DISPLAY_CHAR4, PIN_DISPLAY_CHAR5, PIN_DISPLAY_CHAR6};

/*-------------------------------------------
 ----------STATIC_FUNCTION_DECLARATION-------
 -------------------------------------------*/

/***********************************
*********draw_display***************
************************************
* draw_display updates a given position of the buffer to the display
* (draws a given position). This function checks internally if blinking is enabled
* and the level of brightness.
* 	INPUT:
*		pos : position of the display to be updated.
*	OUTPUT:
*		void.
*/
static void draw_display(int pos);
/***********************************
*********draw_char***************
************************************
* draw_char updates a given position of the display with a given symbol,
* without going through the buffer.
* 	INPUT:
*		printable_char : symbol to be updated directly to the display.
*		pos : position of the display the given symbol will be updated to.
*	OUTPUT:
*		void.
*/
static void draw_char(unsigned char printable_char, int pos);
/***************************************
*********swap_chars*********************
****************************************
* swap_chars swaps the content of two booleans.
* 	INPUT:
*		void.
*	OUTPUT:
*		void.
*/
static void swap_chars(unsigned char * sw1, char *sw2);

/******************************************
*************handle_blinking***************
*******************************************
* handle_blinking tells whether the display should
* show something or not when the blinking option is enabled.
* If the blinking option is disabled, handle_blinking will always return false.
* 	INPUT:
*		void.
*	OUTPUT:
*		true when the display has blinked (nothing should be shown on the display).
*/
static bool handle_blinking(void);
/***********************************
*********handle_brightness***************
************************************
* handle_brightness
* 	INPUT:
*		void.
*	OUTPUT:
*		void.
*/
static bool handle_brightness(void);

/*-------------------------------------------
 ------------FUNCTION_IMPLEMENTATION---------
 -------------------------------------------*/

static void draw_display(int pos){

	if(!display_on || !handle_blinking() || !handle_brightness())
		draw_char(NULL_CHAR, pos);
	else
		draw_char(seven_seg_chars[(int)curr_displaying[pos]], pos);

}

static void draw_char(unsigned char printable_char, int pos){
	GPIO_Type * addr_array[] = GPIO_BASE_PTRS;
	GPIO_Type * gpio = addr_array[DISPLAY_PORT];		//assuming all PIN_DISPLAY pins belong to the same port.

	//could optimize if necessary
	gpioWrite (PIN_DISPLAY_ENC0, pos & 0X1);
	gpioWrite (PIN_DISPLAY_ENC1, pos & 0X2);

	//same as gpioWrite(char_pins[k], (printable_character & ( 1 << k )) >> k); for every k
	for(int k = 0; k < AMOUNT_SEGS; k++)
		if((printable_char & ( 1 << k )) >> k)			//gets the value of the kth bit of printable_char
			gpio->PSOR |= (1 << PIN2NUM(char_pins[k]));
		else
			gpio->PCOR |= (1 << PIN2NUM(char_pins[k]));

	last_drawn_word[pos] = printable_char;
}

void write_char(char c, int pos){
	curr_displaying[pos] = c;
}
void display_clear_all(){
	for(int pos =0; pos < AMOUNT_MAX_DISPLAY_POS; pos++)
		curr_displaying[pos] = NULL_CHAR;
}
void display_clear_pos(int pos){
	curr_displaying[pos] = NULL_CHAR;
}
void init_display(void){
	static bool initialized = false;

	if(initialized)
		return;
	//encoder pins
	gpioMode(PIN_DISPLAY_ENC0, OUTPUT);
	gpioMode(PIN_DISPLAY_ENC1, OUTPUT);

	//char pins
	gpioMode(PIN_DISPLAY_CHAR0, OUTPUT);
	gpioMode(PIN_DISPLAY_CHAR1, OUTPUT);
	gpioMode(PIN_DISPLAY_CHAR2, OUTPUT);
	gpioMode(PIN_DISPLAY_CHAR3, OUTPUT);
	gpioMode(PIN_DISPLAY_CHAR4, OUTPUT);
	gpioMode(PIN_DISPLAY_CHAR5, OUTPUT);
	gpioMode(PIN_DISPLAY_CHAR6, OUTPUT);

	for(int i =0; i < AMOUNT_MAX_DISPLAY_POS; i++){
		last_drawn_word[i] = NULL_CHAR;
		curr_displaying[i] = NULL_CHAR;
	}
	last_drawn_word[AMOUNT_MAX_DISPLAY_POS] = NULL_TERMINATOR;
	initialized = true;
}

void display_draw_callback(){
	static uint32_t counter = 0;
	static int pos = 0;

	// use counter for dividing systick frequency
	if (counter == COUNTER_FREQ) {
		draw_display(pos);
		pos = (pos == AMOUNT_MAX_DISPLAY_POS-1) ? 0 : pos+1;
	}
	else
		counter++;
}

int write_sentence(const char* sentence){
	int i = 0;
	for( ; i < AMOUNT_MAX_DISPLAY_POS; i++)
		if(sentence[i] != 0)
			curr_displaying[i] = sentence[i];
		else{
			for(int j = i; j < AMOUNT_MAX_DISPLAY_POS; j++)
				curr_displaying[j] = NULL_CHAR;
			break;
		}
	return i;
}

//brightness from MIN_BRIGHT to MAX_BRIGHT
void set_brightness(int bright){
	if( (bright < MAX_BRIGHT) && (bright >= MIN_BRIGHT) )
		brightness = bright;
}
void blink(bool on_off){
	/*in case blinking is turned off when curr_displaying is NULL_CHAR,
	the original displaying data should be reloaded into curr_displaying before the
	turning operation off can be performed*/
	//if(!on_off && blinking && blink_cleared)
	//	for(int i = 0; i < AMOUNT_MAX_DISPLAY_POS; i++)
	//		curr_displaying[i] = blink_aux[i];

	blinking = on_off;
	blink_cleared = false;

}
bool is_blinking(void){
	return blinking;
}

static bool handle_blinking(void){
	static int blink_counter = 0;
	if(!blinking)
		return true;

	if(blink_counter < blink_counter_vel)
		blink_counter++;
	else{
		blink_counter = 0;
		blink_cleared = !blink_cleared;
	}
	return !blink_cleared;
}

void shift(direction dir, char to_insert){
	if(dir == LEFT)
		for(int i = AMOUNT_MAX_DISPLAY_POS-1; i >= 0; i--)
			swap_chars(&curr_displaying[i], &to_insert);
	else
		for(int i = 0; i < AMOUNT_MAX_DISPLAY_POS; i++)
			swap_chars(&curr_displaying[i], &to_insert);
}

static void swap_chars(unsigned char * sw1, char *sw2){
	unsigned char aux = *sw1;
	*sw1 = *sw2;
	*sw2 = aux;
}

void display_on_off(bool on_off){
	display_on = on_off;
}

static bool handle_brightness(void){
	static int bright_counter = 0;

	bool should_show = true;
	if(brightness < MAX_BRIGHT)
		if((bright_counter++) == brightness){
			should_show = false;
			bright_counter = 0;
		}
	return should_show;
}

char * get_currently_on_buffer_word(){
	//should disable interrupts!!!
	for(int i = 0; i < AMOUNT_MAX_DISPLAY_POS; i++)
		curr_displaying_photo[i] = curr_displaying[i];
	curr_displaying_photo[AMOUNT_MAX_DISPLAY_POS+1] = NULL_TERMINATOR;
	return curr_displaying_photo;
}

char * get_currently_curr_displaying_word(){
	//should disable interrupts!!!
	for(int i = 0; i < AMOUNT_MAX_DISPLAY_POS; i++)
		last_drawn_word_photo[i] = last_drawn_word[i];
	last_drawn_word_photo[AMOUNT_MAX_DISPLAY_POS+1] = NULL_TERMINATOR;
	return last_drawn_word_photo;
}
