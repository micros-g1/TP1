/***************************************************************************//**
  @file     magnetic_stripe.h
  @brief    User identification via magnetic stripe, track 2
  @author   22.99 2019 Group 1: Alvarez, Gonzalez, Parra, Reina
 ******************************************************************************/

#ifndef MAGNETIC_STRIPE_H
#define MAGNETIC_STRIPE_H


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
typedef enum {MS_START, MS_STOP, MS_SS, MS_ES, MS_DATA, MS_N_EVS} ms_ev_id_t;
typedef struct {
    ms_ev_id_t type;
    bool data;
} ms_ev_t;

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
bool ms_init();
void ms_flush_queue();
void ms_add_to_queue(ms_ev_t ev);

/*******************************************************************************
 ******************************************************************************/


#endif //MAGNETIC_STRIPE
