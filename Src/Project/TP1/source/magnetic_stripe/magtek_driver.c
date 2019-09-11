/***************************************************************************//**
  @file     magtek_driver.c
  @brief    Driver for track 2 card reader
  @author   22.99 2019 Group 1: Alvarez, Gonzalez, Parra, Reina
 ******************************************************************************/

#include "magtek_driver.h"
#include "magnetic_stripe.h"
#include "interrupts.h"
#include "magtek_driver_fsm.h"
#ifdef MT_DEBUG
#include <stdio.h>
#endif

#define MT_SYSTICK 1



#define MT_ENABLEPIN   (PORTNUM2PIN(PD, 0u))
#define MT_CLOCKPIN    (PORTNUM2PIN(PD, 2u))
#define MT_DATAPIN     (PORTNUM2PIN(PD, 3u))

static on_new_card_t newcard_callback;

#ifdef MT_DEBUG
bool datapin = true;
bool enablepin = true;
#endif


static mt_fsm_callback_t const handler_table[MT_N_STATES][MT_N_EVS] = {
                    /* START ******     STOP *******    SS ********     ES *******      DATA */
/* IDLE         */  {mt_cb_restart,     mt_cb_noaction, mt_cb_noaction, mt_cb_noaction, mt_cb_noaction},
/* WAITING SS   */  {mt_cb_noaction,    mt_cb_noaction, mt_cb_noaction, mt_cb_noaction, mt_cb_ssbit},
/* READING DATA */  {mt_cb_noaction,    mt_cb_noaction, mt_cb_noaction, mt_cb_noaction, mt_cb_databit},
/* READING LRC  */  {mt_cb_noaction,    mt_cb_noaction, mt_cb_noaction, mt_cb_noaction, mt_cb_lrcbit}
};

static mt_state_t const state_table[MT_N_STATES][MT_N_EVS] = {
                /* START ******     STOP ****   SS ******           ES *******      DATA */
/* IDLE         */  {MT_WAITING_SS,     MT_IDLE,    MT_IDLE,            MT_IDLE,        MT_IDLE},
/* WAITING SS   */  {MT_IDLE,           MT_IDLE,    MT_READING_DATA,    MT_IDLE,        MT_WAITING_SS},
/* READING DATA */  {MT_IDLE,           MT_IDLE,    MT_IDLE,            MT_READING_LRC, MT_READING_DATA  },
/* READING LRC  */  {MT_IDLE,           MT_IDLE,    MT_IDLE,            MT_IDLE,        MT_READING_LRC}
};



bool mt_init(void(on_new_card(char*)))
{
#ifndef MT_DEBUG
    gpioMode(MT_ENABLEPIN, INPUT); // configure data, enable and clock as input
    gpioMode(MT_CLOCKPIN, INPUT);
    gpioMode(MT_DATAPIN, INPUT);

    gpioIRQ(MT_ENABLEPIN, GPIO_IRQ_MODE_BOTH_EDGES, mt_enable_callback); // configure enable and clock interrupts
    // TODO make mt_enable a systick interrupt rather than an IRQ interrupt
    gpioIRQ(MT_CLOCKPIN, GPIO_IRQ_MODE_FALLING_EDGE, mt_clock_callback);
#endif
    newcard_callback = on_new_card;
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
    ms_ev_t ev;
    ev.type = gpioRead(MT_ENABLE) ? MS_STOP : MS_START; // active low
    ms_add_to_queue(ev);
}
#endif // MT_DEBUG

#ifndef MT_DEBUG
void mt_clock_callback(void) {
    mt_ev_t ev;
    ev.type = MT_DATA;
    ev.data = gpioRead(MT_DATAPIN);
    event_queue_add_event(ev);
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
    if (event_queue_get_length()) {
        mt_ev_t lastev = event_queue_pop_front();
        if (lastev.type == MT_SUCCESS) {
#ifdef MT_DEBUG
            printf("gol");
#endif
        }
        else {
            handler_table[state][lastev.type](lastev);
            state = state_table[state][lastev.type];
        }
    }
}
