/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Nicolás Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "Display/displayDriver.h"
#include "Display/displayInterface.h"
#include "interrupts.h"
#include "SysTick.h"
#include "general.h"
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/




/******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/
void systick_callback(void);

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/* Función que se llama 1 vez, al comienzo del programa */
void App_Init (void)
{

	systick_init();
	systick_add_callback(systick_callback, 10000, SINGLE_SHOT);
	//display_init();
	//write_char('F', 0);
	//write_char('A', 1);
	//write_char('I', 2);
	//write_char('L', 3);
	init_display_interface(NULL);
	write_sentence("BOCA");

}

/* Función que se llama constantemente en un ciclo infinito */
void App_Run (void)
{
	while(1);
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

void systick_callback(void)
{
	//static int counter = 0;
	/*
	 * static int counter = 0;
	if(counter == 0){

		set_brightness(2);
		counter ++;
		//set_brightness(2);

	}
	if(counter>=10000){
		write_sentence("CIL");
		set_brightness(4);
	}
	*/

	//blink_one(2,true);
	//shift(LEFT, 'E');
	//shift(RIGHT, 'E');
	//for(int i = 0; i < AMOUNT_MAX_DISPLAY_POS; i++)
	//	set_brightness_one(i, MIN_BRIGHT+1);
	//set_blinking_all(true);
	marquee("HOLA CHAU", RIGHT);
	//shift(LEFT, 'A');
}


/*******************************************************************************
 ******************************************************************************/
