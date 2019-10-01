/***************************************************************************//**
  @file     magnetic_stripe.c
  @brief    User identification via magnetic stripe, track 2
  @author   22.99 2019 Group 1: Alvarez, Gonzalez, Parra, Reina
 ******************************************************************************/

#include <Magnetic_stripe/magnetic_stripe.h>
#include <Magnetic_stripe/magtek_driver.h>
#include <stdint.h>
#include <stdio.h>

static ms_callback_t higher_callback;

void lower_callback(unsigned char * card);


void ms_init(ms_callback_t callback)
{
    static bool is_init = false;
    if (is_init)
        return;
    is_init = true;
    higher_callback = callback;
    mt_init(lower_callback);
}



void lower_callback (unsigned char * card)
{
    ms_ev_t ev;
    ev.data = card;
    if (card != NULL) {
        ev.type = MS_SUCCESS;
    }
    else {
        ev.type = MS_FAIL;
    }

    higher_callback(ev);
}

