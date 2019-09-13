/***************************************************************************//**
  @file     board.h
  @brief    Board management
  @author   Nicolás Magliola
 ******************************************************************************/

#ifndef _BOARD_H_
#define _BOARD_H_

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "gpio.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

/***** BOARD defines **********************************************************/

// On Board User LEDs
#define PIN_LED_RED 	PORTNUM2PIN (PB,22)
#define PIN_LED_GREEN	PORTNUM2PIN (PE,26)
#define PIN_LED_BLUE    PORTNUM2PIN (PB,21) // PTB21

#define LED_ACTIVE      LOW


// On Board User Switches
#define PIN_SW2         // ???
#define PIN_SW3         PORTNUM2PIN(PA, 4)


// Rotary Encoder Pins
#define RE_PIN_BUTTON_SIGNAL 	PORTNUM2PIN(PB, 2)
#define RE_PIN_SIGNAL_A		PORTNUM2PIN(PB, 10)
#define RE_PIN_SIGNAL_B     PORTNUM2PIN(PB, 3)

// Display Pins
#define DISPLAY_PORT		PC
#define PIN_DISPLAY_ENC0	PORTNUM2PIN(DISPLAY_PORT, 3)
#define PIN_DISPLAY_ENC1	PORTNUM2PIN(DISPLAY_PORT, 2)

#define PIN_DISPLAY_CHAR0	PORTNUM2PIN(DISPLAY_PORT, 5)
#define PIN_DISPLAY_CHAR1	PORTNUM2PIN(DISPLAY_PORT, 7)
#define PIN_DISPLAY_CHAR2	PORTNUM2PIN(DISPLAY_PORT, 0)
#define PIN_DISPLAY_CHAR3	PORTNUM2PIN(DISPLAY_PORT, 9)
#define PIN_DISPLAY_CHAR4	PORTNUM2PIN(DISPLAY_PORT, 8)
#define PIN_DISPLAY_CHAR5	PORTNUM2PIN(DISPLAY_PORT, 1)
#define PIN_DISPLAY_CHAR6	PORTNUM2PIN(DISPLAY_PORT, 17)

#define PIN_DIODE_0			PORTNUM2PIN(DISPLAY_PORT, 11)
#define PIN_DIODE_1			PORTNUM2PIN(DISPLAY_PORT, 10)

#define PIN_DOT			PORTNUM2PIN(PB, 18)

//Magnetic Stripe Pins
#define MT_ENABLEPIN   (PORTNUM2PIN(PD, 0u))
#define MT_CLOCKPIN    (PORTNUM2PIN(PD, 2u))
#define MT_DATAPIN     (PORTNUM2PIN(PD, 3u))


//Interrupts timing pins
#define IT_PERIODIC_PIN	(PORTNUM2PIN(PB, 19u))
#define IT_DEDICATED_1_PIN	(PORTNUM2PIN(PB, 20u))
#define IT_DEDICATED_2_PIN	(PORTNUM2PIN(PB, 23u))
/*******************************************************************************
 ******************************************************************************/

#endif // _BOARD_H_
