//
// Created by Rocío Parra on 9/1/2019.
//

#ifndef SOURCE_MAGNETIC_STRIPE_FSM_H
#define SOURCE_MAGNETIC_STRIPE_FSM_H

typedef enum {MS_START, MS_STOP, MS_SS, MS_ES, MS_DATA, MS_N_EVS} ms_ev_id_t;

typedef struct {
    ms_ev_id_t type;
    void * data;
} ms_ev_t;

#endif //SOURCE_MAGNETIC_STRIPE_FSM_H
