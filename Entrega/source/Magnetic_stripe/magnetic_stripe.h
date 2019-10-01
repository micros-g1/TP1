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
#include <Magnetic_stripe/mt_evqueue.h>
#include <stdbool.h>

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/



/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/


// SUCCESS: valid card passed
// FAIL: card was detected but data was not valid
typedef enum {MS_SUCCESS, MS_FAIL, MS_N_EVS} ms_ev_type_t;

typedef struct {
    ms_ev_type_t type;
    unsigned char * data; // only relevant in success
} ms_ev_t;

typedef void(*ms_callback_t)(ms_ev_t);



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
void ms_init(ms_callback_t callback);

/*******************************************************************************
 ******************************************************************************/


#endif //MAGNETIC_STRIPE
