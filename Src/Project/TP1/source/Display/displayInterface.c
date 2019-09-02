/*
 * displayInterface.c
 *
 *  Created on: 30 ago. 2019
 *      Author: Tomas
 */


#include "displayInterface.h"
#include "displayDriver.h"

/*-------------------------------------------
 ----------------DEFINES---------------------
 -------------------------------------------*/
#define MARQUEE_BUFFER_SIZE		20

/*-------------------------------------------
 ----------------GLOBAL_VARIABLES------------
 -------------------------------------------*/
char marquee_buffer[MARQUEE_BUFFER_SIZE];

/*-------------------------------------------
 ----------FUNCTION_IMPLEMENTATION-----------
 -------------------------------------------*/
void marquee(char* str, direction dir, int vel){
	//FALTA TEMA TIEMPOOOOS!!!
	int chars_written;
	if( ( chars_written = write_sentence(str) ) < AMOUNT_MAX_DISPLAY_POS ){
		if( str[chars_written+1] != NULL_CHAR )
			shift(dir, str[chars_written+1]);
		else
			for(int i = 0; i < AMOUNT_MAX_DISPLAY_POS; i++)
				shift(dir, NULL_CHAR);
	}
}
