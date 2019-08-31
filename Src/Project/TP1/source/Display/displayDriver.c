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

#define CALL_FREQ_HZ 200U
#if SYSTICK_ISR_FREQUENCY_HZ % CALL_FREQ_HZ != 0
#warning BLINK cannot implement this exact frequency.
		Using floor(SYSTICK_ISR_FREQUENCY_HZ/CALL_FREQ_HZ) instead.
#endif

#define AMOUNT_SEGS	7
#define AMOUNT_POSSIBLE_CHAR	128
#define NULL_CHAR	0X00

void draw_display(int pos);

unsigned char curr_displaying[AMOUNT_MAX_DISPLAY_POS] = {NULL_CHAR, NULL_CHAR, NULL_CHAR, NULL_CHAR};

const unsigned char seven_seg_chars[AMOUNT_POSSIBLE_CHAR]= {
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
    0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x00, 0x00,
/*  <     =     >     ?     @     A     B     C     D     E     F     G     */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x00, 0x39, 0x00, 0x79, 0x71, 0x3D,
/*  H     I     J     K     L     M     N     O     P     Q     R     S     */
    0x76, 0x30, 0x1E, 0x00, 0x38, 0x00, 0x00, 0x3F, 0x73, 0x00, 0x00, 0x6D,
/*  T     U     V     W     X     Y     Z     [     \     ]     ^     _     */
    0x00, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
/*  `     a     b     c     d     e     f     g     h     i     j     k     */
    0x00, 0x00, 0x7C, 0x58, 0x5E, 0x00, 0x00, 0x00, 0x74, 0x00, 0x00, 0x00,
/*  l     m     n     o     p     q     r     s     t     u     v     w     */
    0x00, 0x00, 0x54, 0x5C, 0x00, 0x67, 0x50, 0x00, 0x78, 0x1C, 0x00, 0x00,
/*  x     y     z     {		|	  }		~	  cuadradito*/
    0x00, 0x6E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
const unsigned int char_pins[AMOUNT_SEGS] = {PIN_DISPLAY_CHAR0 ,PIN_DISPLAY_CHAR1,PIN_DISPLAY_CHAR2,PIN_DISPLAY_CHAR3,
			PIN_DISPLAY_CHAR4, PIN_DISPLAY_CHAR5, PIN_DISPLAY_CHAR6};

void draw_display(int pos){
	GPIO_Type * addr_array[] = GPIO_BASE_PTRS;
	GPIO_Type * gpio = addr_array[DISPLAY_PORT];		//assuming all PIN_DISPLAY pins belong to the same port.

	//could optimize if necessary
	gpioWrite (PIN_DISPLAY_ENC0, pos & 0X1);
	gpioWrite (PIN_DISPLAY_ENC1, pos & 0X2);
	unsigned int printable_char = seven_seg_chars[(int)curr_displaying[pos]];

	//same as gpioWrite(char_pins[k], (printable_character & ( 1 << k )) >> k); for every k
	for(int k = 0; k < AMOUNT_SEGS; k++)
		if((printable_char & ( 1 << k )) >> k)			//gets the value of the kth bit of printable_char
			gpio->PSOR |= (1 << PIN2NUM(char_pins[k]));
		else
			gpio->PCOR |= (1 << PIN2NUM(char_pins[k]));
}
void write_char(char c, int pos){
	curr_displaying[pos] = c;
}
void clear_all(){
	for(int pos =0; pos < AMOUNT_MAX_DISPLAY_POS; pos++)
		curr_displaying[pos] = NULL_CHAR;
}
void clear_pos(int pos){
	curr_displaying[pos] = NULL_CHAR;
}
void init_display(void){
	//pines encoder
	gpioMode(PIN_DISPLAY_ENC0, OUTPUT);
	gpioMode(PIN_DISPLAY_ENC1, OUTPUT);

	//pines char
	gpioMode(PIN_DISPLAY_CHAR0, OUTPUT);
	gpioMode(PIN_DISPLAY_CHAR1, OUTPUT);
	gpioMode(PIN_DISPLAY_CHAR2, OUTPUT);
	gpioMode(PIN_DISPLAY_CHAR3, OUTPUT);
	gpioMode(PIN_DISPLAY_CHAR4, OUTPUT);
	gpioMode(PIN_DISPLAY_CHAR5, OUTPUT);
	gpioMode(PIN_DISPLAY_CHAR6, OUTPUT);

}

void display_draw_callback(){
	static uint32_t counter = 0;
	static int pos = 0;
	// use counter for dividing systick frequency
	if (counter == SYSTICK_ISR_FREQUENCY_HZ/CALL_FREQ_HZ/4) {
		draw_display(pos);
		pos = (pos == 3) ? 0 : pos + 1;
		counter = 0;
	}
	else
		counter++;
}

void write_sentence(const char* sentence){
	for(int i = 0; i < AMOUNT_MAX_DISPLAY_POS; i++)
		if(sentence[i] != 0)
			curr_displaying[i] = sentence[i];
		else{
			for(int j = i; j < AMOUNT_MAX_DISPLAY_POS; j++)
				curr_displaying[j] = NULL_CHAR;
			break;
		}
}
