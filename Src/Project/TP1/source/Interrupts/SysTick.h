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
#include "general.h"
/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define SYSTICK_ISR_FREQUENCY_HZ 8000U
//maximum amout of callbacks that can be added to sysTick
#define MAX_N_ST_CALLBACKS 20

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef void (*systick_callback_t)(void);
typedef enum{SINGLE_SHOT, PERIODIC} callback_conf_t;

/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Initialize SysTic driver.
 * The function has no effect when called twice (safe init).
 * @return void.
 */
void systick_init();

/**
 * @brief Add function to be called on systick interrupts. Enabled by default
 * @param callback : function to be called when sysTick is called 'reload' times.
 * @param reload : amount of times sysTick has to tick for the callback to be called.s
 * @param set_up : configuration option for the callback.
 */
void systick_add_callback(systick_callback_t callback, unsigned int reload, callback_conf_t set_up);

/**
 * @brief Enable callback so that it may be called when the reload limit is reached.
 * @param callback: callback to be enabled.
 * @return void.
 */
void systick_enable_callback(systick_callback_t callback);
/**
 * @brief Disable callback so that it wont be called when the reload limit is reached.
 * @param callback: callback to be disabled.
 * @return void.
 */
void systick_disable_callback(systick_callback_t callback);

/**
 * @brief deletes a specific callback from those added with systick_add_callback function
 * @param callback: callback to be deleted.
 * @return void.
 */
void systick_delete_callback(systick_callback_t callback);

/**
 * @brief Inform the user if a specific callback has been added (and not deleted after that) to sysTick
 * @param callback: callback to check.
 * @return true if the callback was added to sysTick.
 */
bool systick_has_callback(systick_callback_t callback);
/**
 * @brief get a specific callaback's configuration.
 * @param callback: callback to get the configuration from.
 * @return current configuration for the callback.
 */
callback_conf_t systick_get_callback_conf(systick_callback_t callback);
/**
 * @brief set a specific callback's configuration.
 * @param callback: callback for which the sysTick configuration will be set.
 * @param conf: configuration for the callback
 * @return current configuration for the callback.
 */
void systick_set_callback_conf(systick_callback_t callback, callback_conf_t conf);
/*******************************************************************************
 ******************************************************************************/

#endif // _SYSTICK_H_
