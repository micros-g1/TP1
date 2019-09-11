/***************************************************************************//**
  @file     magnetic_stripe.c
  @brief    User identification via magnetic stripe, track 2
  @author   22.99 2019 Group 1: Alvarez, Gonzalez, Parra, Reina
 ******************************************************************************/

#include "magnetic_stripe.h"
#include <stdint.h>
#include <stdio.h>
#include "magtek_driver.h"


void callback (char * c) { ; }

static uint8_t currword;

bool ms_init()
{
    mt_init(callback);
	// port config
	currword = 0;
}




