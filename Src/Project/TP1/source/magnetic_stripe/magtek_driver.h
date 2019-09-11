/***************************************************************************//**
  @file     magtek_driver.h
  @brief    Driver for track 2 card reader
  @author   22.99 2019 Group 1: Alvarez, Gonzalez, Parra, Reina
 ******************************************************************************/
#ifndef SOURCE_MAGTEK_DRIVER_H
#define SOURCE_MAGTEK_DRIVER_H



/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdbool.h>


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef void(*on_new_card_t)(char*);
/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Initialize magnetic stripe reading
 * @return Initialization succeed
 */

#define MT_DEBUG

#ifdef MT_DEBUG
extern bool datapin;
extern bool enablepin;
#endif


bool mt_init(on_new_card_t callback);

void mt_enable_callback();
void mt_clock_callback();
void mt_periodic(void);



/*******************************************************************************
 ******************************************************************************/



#endif //SOURCE_MAGTEK_DRIVER_H
