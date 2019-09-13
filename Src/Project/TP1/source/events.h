//
// Created by lisan on 6/9/2019.
//

#ifndef EVENTS_H_
#define EVENTS_H_

#define MAX_QUEUE_LENGTH    10
#define MAX_DATA_LENGTH 100
#include <stdbool.h>

typedef enum{
    ENTER_EV,
    BACK_EV,
    CANCEL_EV,
    BRIGHT_UP_EV,
    BRIGHT_DOWN_EV,
    QUIT_EV,
    NO_EV,
    UP_EV,
    DOWN_EV,
    SUBMIT_DATA_EV,
	INVALID_ID_EV,
	VALID_ID_EV,
	VALID_ID_CONF_EV,
	VALID_ID_CONF_ADM_EV,
	VALID_CARD_EV,
	INVALID_CARD_EV,
	MARQUEE_END_EV,
	EMPTY_DB_EV,
	NOT_EMPTY_DB_EV,
	USER_BLOCKED_EV,
	USER_DONT_EXISTS_EV,
	VALID_PIN_EV,
	INVALID_PIN_EV,
	USER_LOGIN,
	USER_LOGIN_CONF,
	ADMIN_LOGIN_CONF,
	REMOVE_USER_OPT_EV,
	ADD_USER_OPT_EV,
	CHANGE_PIN_OPT_EV,
	VALID_REMOVE_USER_EV,
	UNBLOCK_USER_OPT_EV,
	VALID_UNBLOCK_USER_EV,
	CARD_EV,
	TIMEOUT_EV,
	GND_EV

}fsm_event_code;


typedef struct{
    fsm_event_code code;
    char data[MAX_DATA_LENGTH];
}fsm_event_t;

typedef struct {
    fsm_event_t buffer[MAX_QUEUE_LENGTH];
    int head;
    int tail;
    int count;
}fsm_event_queue;

bool is_there_event();
void init_event_queue();
void push_event(fsm_event_t event);
void pop_event(fsm_event_t * event);

#endif //EVENTS_H_
