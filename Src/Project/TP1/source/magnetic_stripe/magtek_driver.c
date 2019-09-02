/***************************************************************************//**
  @file     magtek_driver.c
  @brief    Driver for track 2 card reader
  @author   22.99 2019 Group 1: Alvarez, Gonzalez, Parra, Reina
 ******************************************************************************/

#include "magtek_driver.h"
#include "magnetic_stripe.h"
#include "interrupts.h"

#define MT_ENABLE   (PORTNUM2PIN(PD, 0u))
#define MT_CLOCK    (PORTNUM2PIN(PD, 2u))
#define MT_DATA     (PORTNUM2PIN(PD, 3u))

#define MT_SYSTICK 1


bool mt_init()
{
    gpioMode(MT_ENABLE, INPUT); // configure data, enable and clock as input
    gpioMode(MT_CLOCK, INPUT);
    gpioMode(MT_DATA, INPUT);

    gpioIRQ(MT_ENABLE, GPIO_IRQ_MODE_BOTH_EDGES, mt_enable_callback); // configure enable and clock interrupts
    // TODO make mt_enable a systick interrupt rather than an IRQ interrupt
    gpioIRQ(MT_CLOCK, GPIO_IRQ_MODE_FALLING_EDGE, mt_clock_callback);
}

#if MT_SYSTICK == 1
void mt_enable_callback() {
    static bool prev_enable = true; // start as enable high (active low)
    bool new_enable = gpioRead(MT_ENABLE);

    if (new_enable != prev_enable) { // only enable edges result in events
        ms_ev_t ev;
        ev.type = prev_enable ? MS_START : MS_STOP; // active low
        ms_add_to_queue(ev);
        prev_enable = new_enable;
    }
}
#else // IRQ interrupt on edges
void mt_enable_callback() {
    ms_ev_t ev;
    ev.type = gpioRead(MT_ENABLE) ? MS_STOP : MS_START; // active low
    ms_add_to_queue(ev);
}
#endif // MT_SYSTICK == 1

void mt_clock_callback() {
    ms_ev_t ev;
    ev.type = MS_DATA;
    ev.data = gpioRead(MT_DATA);
    ms_add_to_queue(ev);
}
