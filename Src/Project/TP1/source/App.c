/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Nicolás Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

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
	init_display_interface(NULL);
	//write_char('F', 0);
	//write_char_2_display('A', 1);
	//write_char('I', 2);
	//write_char('L', 3);
	write_word_2_display("BOCA");

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
	write_to_led(1, true);
	write_to_led(0, true);
	write_to_led(2, true);
	for(int i = 0; i< 3; i++)
		set_blinking_led_one(i, true);
	set_blinking_all(true);
}


/*******************************************************************************
 ******************************************************************************/
