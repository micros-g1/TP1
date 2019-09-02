/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Nicolás Magliola
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "Display/displayDriver.h"
#include "interrupts.h"
#include "SysTick.h"
#include "general.h"
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/




/*******************************************************************************
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
	init_display();
	//write_char('F', 0);
	//write_char('A', 1);
	//write_char('I', 2);
	//write_char('L', 3);
	write_sentence("GIL");
	//write_sentence("FA");
    SysTick_Init(systick_callback);
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
	static int counter = 0;
	if(counter == 0){
		//blink(true);
		set_brightness(2);
		counter ++;
		//set_brightness(2);
		//shift(LEFT, 'E');
	}
	if(counter>=10000){
		write_sentence("CIL");
		set_brightness(4);
	}

	counter++;

	display_draw_callback();
}


/*******************************************************************************
 ******************************************************************************/
