/***************************************************************************//**
  @file     magtek_driver.c
  @brief    Driver for track 2 card reader
  @author   22.99 2019 Group 1: Alvarez, Gonzalez, Parra, Reina
 ******************************************************************************/

#include <Interrupts/interrupts.h>
#include <Magnetic_stripe/magnetic_stripe.h>
#include <Magnetic_stripe/magtek_driver.h>
#include <Magnetic_stripe/magtek_driver_fsm.h>
#include <stdlib.h>
#include "board.h"
#include <Interrupts/SysTick.h>

#define MT_SYSTICK 0

static mt_callback_t cb;
static unsigned char card_buffer[50];

static mt_fsm_callback_t const handler_table[MT_N_STATES][MT_N_EVS] = {
                    /* START ******    STOP *******    SS ********     ES *******      DATA 			SUCCESS*/
/* IDLE         */  {mt_cb_restart,    mt_cb_noaction, mt_cb_noaction, mt_cb_noaction, mt_cb_noaction,	mt_cb_noaction},
/* WAITING SS   */  {mt_cb_restart,    mt_cb_noaction, mt_cb_noaction, mt_cb_noaction, mt_cb_ssbit, 	mt_cb_noaction},
/* READING DATA */  {mt_cb_restart,    mt_cb_noaction, mt_cb_noaction, mt_cb_noaction, mt_cb_databit, 	mt_cb_noaction},
/* READING LRC  */  {mt_cb_restart,    mt_cb_noaction, mt_cb_noaction, mt_cb_noaction, mt_cb_lrcbit,	mt_cb_noaction}
};

static mt_state_t const state_table[MT_N_STATES][MT_N_EVS] = {
                /* 	START ******     	STOP ****   SS ******           ES *******      DATA 				SUCCESS */
/* IDLE         */  {MT_WAITING_SS,     MT_IDLE,    MT_IDLE,            MT_IDLE,        MT_IDLE,			MT_IDLE},
/* WAITING SS   */  {MT_WAITING_SS,     MT_IDLE,    MT_READING_DATA,    MT_IDLE,        MT_WAITING_SS,		MT_WAITING_SS},
/* READING DATA */  {MT_READING_DATA,   MT_IDLE,    MT_IDLE,            MT_READING_LRC, MT_READING_DATA, 	MT_READING_DATA},
/* READING LRC  */  {MT_READING_LRC,    MT_IDLE,    MT_IDLE,            MT_IDLE,        MT_READING_LRC,		MT_IDLE}
};


void mt_init(mt_callback_t callback)
{
    static bool is_init =  false;
    if (is_init)
        return;
    is_init = true;
    interrupts_init();
    systick_init();
    // configure data, enable and clock as input
    gpioMode(MT_ENABLEPIN, INPUT_PULLUP);
    gpioMode(MT_CLOCKPIN, INPUT_PULLUP);
    gpioMode(MT_DATAPIN, INPUT_PULLUP);

    // configure enable and clock interrupts
    gpioIRQ(MT_ENABLEPIN, GPIO_IRQ_MODE_BOTH_EDGES, mt_enable_callback);
    gpioIRQ(MT_CLOCKPIN, GPIO_IRQ_MODE_FALLING_EDGE, mt_clock_callback);
    gpioMode(IT_DEDICATED_1_PIN, OUTPUT);
    gpioWrite(IT_DEDICATED_1_PIN, false);
    gpioMode(IT_DEDICATED_2_PIN, OUTPUT);
    gpioWrite(IT_DEDICATED_2_PIN, false);
    cb = callback;
    systick_add_callback(mt_periodic, 100, PERIODIC);
    event_queue_flush();
    card_buffer[0] = 0;
}


void mt_enable_callback() {
    mt_ev_t ev;
    gpioWrite(IT_DEDICATED_1_PIN, true);
    //Active Low
    if (gpioRead(MT_ENABLEPIN)) {
    	ev.type = MT_STOP;
    	//Add at the end of the event queue
    	event_queue_add_event(ev);
    }
    else {
    	ev.type = MT_START;
    	//MT START NOW. front!
    	event_queue_add_event_front(ev);
    }
    gpioWrite(IT_DEDICATED_1_PIN, false);
}

void mt_clock_callback(void) {
	gpioWrite(IT_DEDICATED_2_PIN, true);
	//clock callback -> read data
    mt_ev_t ev;
    ev.type = MT_DATA;
    //Active low
    ev.data = !gpioRead(MT_DATAPIN);
    event_queue_add_event(ev);
    gpioWrite(IT_DEDICATED_2_PIN, false);
}

void mt_periodic(void)
{
    static mt_state_t state = MT_IDLE;
    //Wait for event...
    while (event_queue_get_length()) {
        mt_ev_t lastev = event_queue_pop_front();
        if (lastev.type == MT_SUCCESS) {
        	//Reset FSM when success.
        	state = MT_IDLE;
        	//Callback!
        	mt_get_card(card_buffer);
        	cb(card_buffer);
        }
        else {
        	if (lastev.type == MT_STOP && state != MT_IDLE) {
        		//Unexpected STOP. Fails.
        		card_buffer[0] = 0;
        		cb(NULL);
        	}
        	//Execute FSM Action
            handler_table[state][lastev.type](lastev);
            //Next state
            state = state_table[state][lastev.type];
        }
    }
}
