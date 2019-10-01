//
// Created by lisan on 6/9/2019.
//

#ifndef FSM_H_
#define FSM_H_

#include "events.h"
#include "Magnetic_stripe/magnetic_stripe.h"

typedef struct state_table fsm_state_t;

void ms_callback(ms_ev_t ev);

struct state_table{
    fsm_event_t event;
    fsm_state_t * next_state;
    void (*transition)(void);
};

fsm_state_t * fsm_run(fsm_state_t * cur_state, fsm_event_t cur_event);

#endif //FSM_H_
