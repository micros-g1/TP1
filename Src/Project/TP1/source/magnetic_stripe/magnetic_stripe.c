/***************************************************************************//**
  @file     magnetic_stripe.c
  @brief    User identification via magnetic stripe, track 2
  @author   22.99 2019 Group 1: Alvarez, Gonzalez, Parra, Reina
 ******************************************************************************/

#include "magnetic_stripe.h"
#include <stdint.h>
#include <stdio.h>



typedef enum {MS_IDLE, MS_WAITING_SS, MS_READING, MS_LRC, MS_N_STATES} ms_state_t;

typedef struct {
    ms_state_t curr;
//    ms_queue_t queue;

}ms_fsm;

static uint8_t currword;
typedef char ev_queue_t; // placeholder


bool ms_init()
{
    // mt_init();
	// port config
	currword = 0;
}

void ms_add_to_queue(ms_ev_t ev) {
    ; // todo
}


void process_bit_ss(ms_ev_t ev);

void discard(ms_ev_t ev);

void nop(ms_ev_t ev);

void discard(ms_ev_t ev);

void reset_ss(ms_ev_t ev);

void discard(ms_ev_t ev);

void process_bit_ss(ms_ev_t ev)
{
	static uint32_t n = 0; // bits of start sentinel already received
    bool newbit = ev.data;
	/* The start sentinel bits are received as follows: 11010b
	 * An FSM detects this sequence for characters, where the state is equivalent to the number of correct bits
	 * received, and the only action taken is generating an event upon successful detection of the sequence
	 * */
    switch(n) {
        case 0: n = newbit;         break; // first bit == 1
        case 1: n = newbit? 2 : 0;  break;
        case 2: n = newbit? 2 : 3;  break;
        case 3: n = newbit? 4 : 0;  break;

        case 4: {
            if (newbit) {
                n = 2;
            }
            else {
                n = 0;
                printf("gol");
            }
        }
        default: n = 0; break; // this should not happen, default to empty
    }

}

