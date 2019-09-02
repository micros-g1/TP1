/***************************************************************************//**
  @file     SysTick.h
  @brief    SysTick driver
  @author   Nicol�s Magliola
 ******************************************************************************/

#ifndef _SYSTICK_H_
#define _SYSTICK_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <stdbool.h>


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define SYSTICK_ISR_FREQUENCY_HZ 1000U
#define MAX_N_ST_CALLBACKS 10

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef void (*systick_callback_t)(void);


/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Initialize SysTic driver
 * @return Initialization and registration succeed
 */
void systick_init ();

/**
 * @brief Add function to be called on systick interrupts. Enabled by default
 * @param callback: function to be called when counter hits 0
 * @param reload: max value of tick counter
 * @return Callback ID. MAX_N_ST_CALLBACKS if unsuccessful
 */
unsigned int systick_add_callback(systick_callback_t callback, unsigned int reload);

void systick_enable_callback(unsigned int id);
void systick_disable_callback(unsigned int id);

/*******************************************************************************
 ******************************************************************************/

#endif // _SYSTICK_H_
