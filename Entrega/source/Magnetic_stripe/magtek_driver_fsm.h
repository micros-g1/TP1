//
// Created by Rocío Parra on 9/4/2019.
//

#ifndef SOURCE_MAGTEK_DRIVER_FSM_H
#define SOURCE_MAGTEK_DRIVER_FSM_H


#include <Magnetic_stripe/mt_evqueue.h>


typedef enum {MT_IDLE, MT_WAITING_SS, MT_READING_DATA, MT_READING_LRC, MT_N_STATES} mt_state_t;

typedef void (*mt_fsm_callback_t)(mt_ev_t);

void mt_fsm_init();

void mt_cb_noaction(mt_ev_t ev);
void mt_cb_restart(mt_ev_t ev);
void mt_cb_ssbit(mt_ev_t ev);
void mt_cb_databit(mt_ev_t ev);
void mt_cb_lrcbit(mt_ev_t ev);

void mt_get_card(unsigned char * buffer);

#endif //SOURCE_MAGTEK_DRIVER_FSM_H
