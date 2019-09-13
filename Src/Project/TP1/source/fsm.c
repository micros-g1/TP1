//
// Created by lisan on 6/9/2019.
//
#include "fsm.h"
#include "events.h"

fsm_state_t * fsm_run(fsm_state_t * cur_state, fsm_event_t cur_event){
    while(cur_state->event.code != cur_event.code && cur_state->event.code != GND_EV)
        ++cur_state;
    (*cur_state->transition)();
    cur_state = cur_state->next_state;
    return cur_state;
}
