//
// Created by Rocío Parra on 9/4/2019.
//

#include "magtek_driver_fsm.h"
#include <stdint.h>

#define CARD_SIZE   40
#define WORD_SIZE   5

#define MT_SS_SYM   0X1A
#define MT_ES_SYM   0X16

static uint32_t nbit = 0;
static uint32_t nword = 0;
static bool curr_parity = true;
static bool expected_lrc [WORD_SIZE-1];

static unsigned char card_buffer[CARD_SIZE];
static unsigned char curr_word;


void mt_raise_error(void);

void mt_cb_noaction(mt_ev_t ev)
{
}


void mt_cb_ssbit(mt_ev_t ev)
{
    bool newbit = ev.data;
    /* The start sentinel bits are received as follows: 11010b
     * An FSM detects this sequence for characters, where the state is equivalent to the number of correct bits
     * received, and the only action taken is generating an event upon successful detection of the sequence
     * */
    switch(nbit) {// n: bits of start sentinel already received
        case 0: nbit = newbit;         break; // first bit == 1
        case 1: nbit = newbit? 2 : 0;  break;
        case 2: nbit = newbit? 2 : 3;  break;
        case 3: nbit = newbit? 4 : 0;  break;

        case 4:
            if (newbit) {
                nbit = 2;
            }
            else {
                nbit = 0;   //leave things ready to process next section
                nword++;
                curr_word = 0;
                curr_parity = true;
                expected_lrc[0] = true;
                expected_lrc[1] = true;
                expected_lrc[2] = false;
                expected_lrc[3] = true;

                mt_ev_t newev;
                newev.type = MT_SS;
                newev.data = 0;
                event_queue_add_event(newev);

            }

            break;

            default: break;
    }

}

void mt_cb_restart(mt_ev_t ev) {
    nbit = 0;
    nword = 0;
    card_buffer[0] = MT_SS_SYM;
    curr_word = 0;
    curr_parity = true;
}

void mt_cb_databit(mt_ev_t ev)
{
    if (nword < CARD_SIZE - 1) { // leave one byte for LRC
        curr_word <<= 1; // add new bit to current word
        curr_word += (unsigned int)ev.data;

        if (nbit == WORD_SIZE - 1) { // last bit (parity) received: check parity, check if ES and wait for next word
            if (ev.data == curr_parity) {   // word completed!
                card_buffer[nword] = curr_word; // save word in buffer

                if (curr_word == MT_ES_SYM) {   //check if end sentinel was found
                    mt_ev_t newev;
                    newev.type = MT_ES; // now we wait for lrc bit, raise event to change state
                    event_queue_add_event(newev);
                }

                nword++;            // wait for next word
                curr_word = 0;      // set initial word values
                nbit = 0;
                curr_parity = true;
            }
            else { // wrong parity bit in word
                mt_raise_error();
            }

        }
        else { // data bit received: update parity for lrc and wait for next bit
            expected_lrc[nbit] ^= ev.data;
            curr_parity ^= ev.data;
            nbit++;
        }
    }
    else { // end sentinel not found
        mt_raise_error();
    }
}

void mt_cb_lrcbit(mt_ev_t ev)
{
    if (nbit < WORD_SIZE - 1) { // parity with previous words
        if (expected_lrc[nbit] == ev.data) {
            nbit++; //good lrc bit
        }
        else {
            mt_raise_error();
        }
    }
    else {  // parity with lrc
        bool lrc_parity = true;
        unsigned int i;
        for (i = 0; i < WORD_SIZE -1; i++) {
            lrc_parity ^= expected_lrc[i];
        }

        if (lrc_parity == ev.data) {
            mt_ev_t newev;
            newev.type = MT_SUCCESS;
            event_queue_add_event(newev);
        }
        else {
            mt_raise_error();
        }
    }
}


void mt_raise_error(void)
{
    nbit = 0;   // this data should not be accessed before restart, but in case of error this prevents buffer overflow
    nword = 0;
    mt_ev_t newev;
    newev.type = MT_STOP;
    event_queue_add_event(newev);
}
