//
// Created by lisan on 6/9/2019.
//

#ifndef FSM_STATES_H_
#define FSM_STATES_H_

#include "fsm.h"
#include "Display/display_interface.h"
#include "Database/database.h"

#define ID_LENGTH 8
#define CARD_DATA_LENGTH 40
#define PIN_LENGTH 5
#define MAX_INPUT_LENGTH 10

/*
 * state information structure.
 * stores info about data beeing edited by user
 */

typedef enum{
	INVALID_ERROR,
	BLOCKED_ERROR,
	WRONG_PASS,
	LAST_ADMIN_ERROR,
	DONT_EXIST_ERROR,
	CARD_IN_USE_ERROR,
	ALREADY_EXISTS_ERROR,
	ERROR_CARD_READ,
	ERROR_TYPE_NUM,
}fsm_error_code_t;

char * error_msgs[] = {"id not found", "id bloc", "pin incorrect","error cant delete super user", "user not found", "error card in use","error repeated user","error reading"};
char * admin_options[] = {"add user","delete user","change pin","unbl user"};


typedef enum{
	ADD_USER_ENTRY,
	DELETE_USER_ENTRY,
	CHANGE_PIN_ENTRY,
	UNBLOCK_USER_ENTRY,
	N_ENTRIES
}fsm_admin_menu_entries;

typedef enum{
	USER_ACCESS,
	REMOVE_USER,
	ADD_USER,
}fsm_use_mode;

typedef struct{
	char data[MAX_INPUT_LENGTH];	// data beeing edited
	char * data_ptr;				// pointer to traverse data
	int cur_index;					// index indicating the char beeing edited
	int page_offset;				// page indicating the offset of the first digit displayed from the data[0]
	bool masked;					// indicates wether the data must be masked while being typed
	char mask_char;					// character to generate de mask
	int max_pages;					// if known to control led indicators.
	int data_length;					// data input length
	bool is_pin;
	fsm_error_code_t error_code;
}input_data_helper_t;

typedef struct{
	char id[ID_LENGTH + 1];
	char pin[PIN_LENGTH + 1];
	char card[CARD_DATA_LENGTH + 1];
	id_type_t input_type;
	bool admin;
}fsm_user_data_t;

typedef struct{
	fsm_admin_menu_entries option;
}admin_menu_helper_t;


admin_menu_helper_t admin_menu_helper;
fsm_user_data_t logged_usr;
fsm_user_data_t aux_user;
fsm_user_data_t user_data_helper;
input_data_helper_t data_helper;
fsm_use_mode use_mode;
bool config_mode;

// luego sacarlo de aca y meterlo todo en el driver de display. solo por debug
#define PIN_LED_RED 	PORTNUM2PIN (PB,22)
#define PIN_LED_GREEN   PORTNUM2PIN(PE,26)
#define PIN_LED_BLUE    PORTNUM2PIN(PB,21)

fsm_state_t const * fsm_get_init_state(void);

#endif
