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
#ifdef MT_DEBUG
#include <stdio.h>
#endif
#include <Interrupts/SysTick.h>

#define MT_SYSTICK 0


#define MT_ENABLEPIN   (PORTNUM2PIN(PD, 0u))
#define MT_CLOCKPIN    (PORTNUM2PIN(PD, 2u))
#define MT_DATAPIN     (PORTNUM2PIN(PD, 3u))
//#define MT_DEGUBPIN		(PORTNUM2PIN(PC, 4u))

static mt_callback_t cb;
static volatile unsigned char card_buffer[50];


#ifdef MT_DEBUG
bool datapin = true;
bool enablepin = true;
#endif


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
/* READING DATA */  {MT_WAITING_SS,     MT_IDLE,    MT_IDLE,            MT_READING_LRC, MT_READING_DATA, 	MT_READING_DATA},
/* READING LRC  */  {MT_WAITING_SS,     MT_IDLE,    MT_IDLE,            MT_IDLE,        MT_READING_LRC,		MT_IDLE}
};



void mt_init(mt_callback_t callback)
{
    static bool is_init =  false;
    if (is_init)
        return;
    is_init = true;

#ifndef MT_DEBUG
    gpioMode(MT_ENABLEPIN, INPUT); // configure data, enable and clock as input
    gpioMode(MT_CLOCKPIN, INPUT);
    gpioMode(MT_DATAPIN, INPUT);
//    gpioMode(MT_DEGUBPIN, OUTPUT);

    gpioIRQ(MT_ENABLEPIN, GPIO_IRQ_MODE_BOTH_EDGES, mt_enable_callback); // configure enable and clock interrupts
    // TODO make mt_enable a systick interrupt rather than an IRQ interrupt
    gpioIRQ(MT_CLOCKPIN, GPIO_IRQ_MODE_FALLING_EDGE, mt_clock_callback);
#endif
    cb = callback;
    systick_add_callback(mt_periodic, 100,PERIODIC);
    // TODO proper initialization of systick
    event_queue_flush();
    card_buffer[0] = 0;
}


#ifdef MT_DEBUG
void mt_enable_callback(void)
{
    static bool prev_enable = true; // start as enable high (active low)
    bool new_enable = enablepin;

    if (new_enable != prev_enable) { // only enable edges result in events
        mt_ev_t ev;
        ev.type = prev_enable ? MT_START : MT_STOP; // active low
        event_queue_add_event(ev);
        prev_enable = new_enable;
    }
}

#elif MT_SYSTICK == 1
void mt_enable_callback(void) {
    static bool prev_enable = true; // start as enable high (active low)
    bool new_enable = gpioRead(MT_ENABLEPIN);

    if (new_enable != prev_enable) { // only enable edges result in events
        mt_ev_t ev;
        ev.type = prev_enable ? MT_START : MT_STOP; // active low
        event_queue_add_event(ev);
        prev_enable = new_enable;
    }
}
#else // IRQ interrupt on edges
void mt_enable_callback() {
    mt_ev_t ev;
//    static int i = 0;
    if (gpioRead(MT_ENABLEPIN)) {
    	ev.type = MT_STOP; // active low
    	event_queue_add_event(ev);
    }
    else {
    	ev.type = MT_START;
    	event_queue_add_event_front(ev);
//    	if(i != 0)
//    		i++;
    }
    //i++;
}
#endif // MT_DEBUG

#ifndef MT_DEBUG
void mt_clock_callback(void) {
//	gpioWrite(MT_DEGUBPIN,true);
//	static int i = 0;
    mt_ev_t ev;
    ev.type = MT_DATA;
    ev.data = !gpioRead(MT_DATAPIN);
    event_queue_add_event(ev);
//    i++;
//	gpioWrite(MT_DEGUBPIN,false);
}
#else
void mt_clock_callback(void) {
    mt_ev_t ev;
    ev.type = MT_DATA;
    ev.data = datapin;
    event_queue_add_event(ev);
}
#endif //MT_DEBUG

void mt_periodic(void)
{
    static mt_state_t state = MT_IDLE;
    while (event_queue_get_length()) {
        mt_ev_t lastev = event_queue_pop_front();
        if (lastev.type == MT_SUCCESS) {
        	state = MT_IDLE;
        	mt_get_card(card_buffer);
        	cb(card_buffer);
        }
        else {
        	if (lastev.type == MT_STOP && state != MT_IDLE) {
        		card_buffer[0] = 0;
        		cb(NULL);
        	}

            handler_table[state][lastev.type](lastev);
            state = state_table[state][lastev.type];
        }
    }
}


