//
// Created by lisan on 6/9/2019.
//

#include <fsm_states.h>
#include "string.h"
#include "Display/display_interface.h"
#include "Display/freedom_leds.h"
#include "Magnetic_stripe/magnetic_stripe.h"
#include <stdbool.h>

ms_ev_t aux_ms_ev;

//amount of display positions.
#define DISPLAY_DIGITS 4

void ms_callback(ms_ev_t ev){
	fsm_event_t event;
	if(ev.type == MS_SUCCESS){
		aux_ms_ev.data = ev.data;
		event.code = CARD_EV;
	}
	else
	{
		event.code = CARD_EV;
		aux_ms_ev.data = NULL;
	}
	push_event(event);
}

//FSM states
fsm_state_t waiting_id_state[];
fsm_state_t initial_state[];
fsm_state_t waiting_db_id_confirmation_state[];
fsm_state_t waiting_u_id_confirmation_state[];
fsm_state_t waiting_pin_state[];
fsm_state_t admin_menu_state[];
fsm_state_t waiting_db_pin_confirmation_state[];
fsm_state_t door_open_state[];
fsm_state_t admin_menu_state[];
fsm_state_t change_pin_state[];
fsm_state_t remove_user_state[];
fsm_state_t add_id_user_state[];
fsm_state_t delete_succes_state_msg[];
fsm_state_t show_error_msg_state_adminmenu[];
fsm_state_t add_card_user_state[];
fsm_state_t add_pin_user_state[];
fsm_state_t add_success_state[];
fsm_state_t change_pin_msg_state[];
fsm_state_t show_error_msg_state_inputid[];
fsm_state_t user_menu_state[];
fsm_state_t unblock_succes_state_msg[];
fsm_state_t unblock_user_state[];
/*****************************************
*************do_nothing**************
******************************************
* do_nothing refer to title for behaviour.
* 	INPUT:
*		void.
*	OUTPUT:
*		void
*/
static void do_nothing(void){}

/*****************************************
*************setup_waiting_id**************
******************************************
* setup_waiting_id sets the conditions for the user to start inputting an id.
* This includes changing the display, setting the inital id to 00000000
* and the current input digit to 0.
* 	INPUT:
*		void.
*	OUTPUT:
*		void
*/
static void setup_waiting_id(void);

/*****************************************
*************increase_digit**************
******************************************
* increase_digit increments the digit on display and logically when
* inputting an id or a pin.
* 	INPUT:
*		void.
*	OUTPUT:
*		void
*/
static void increase_digit(void);

/*****************************************
*************decrease_digit**************
******************************************
* decrease_digit decreases the digit on display and logically when
* inputting an id or a pin.
* 	INPUT:
*		void.
*	OUTPUT:
*		void
*/
static void decrease_digit(void);

/*****************************************
*************next_digit**************
******************************************
* next_digit changes the input conditions (logical and graphical)
* to the next corresponding digit, changing indexes and blinking digit.
* 	INPUT:
*		void.
*	OUTPUT:
*		void
*/
static void next_digit(void);

/*****************************************
*************previous_digit**************
******************************************
* previous_digit changes the input conditions (logical and graphical)
* to the previous corresponding digit, changing indexes and blinking digit.
* 	INPUT:
*		void.
*	OUTPUT:
*		void
*/
static void previous_digit(void);

/*****************************************
*************re_print_data**************
******************************************
* re_print_data prints on display the new input configuration whenever a
* new page has been set (as a consequence of having inputted a new digit or
* successfully entered an id).
* 	INPUT:
*		void.
*	OUTPUT:
*		void
*/
static void re_print_data(void);

/*****************************************
*************set_page_indicator**************
******************************************
* set_page_indicator changes the leds of the display according to the
* current page the user is in so that he/she/it/apache helicopter may have a reference of
* his current input status even though the amount of display positions is limited.
* 	INPUT:
*		void.
*	OUTPUT:
*		void
*/
static void set_page_indicator(void);

/*****************************************
*************check_id*********************
******************************************
* check_id checks if the given id exists and is not blocked. Otherwise, an invalid id event
* is appended to fsm event queue.
* 	INPUT:
*		void.
*	OUTPUT:
*		void
*/
static void check_id(void);

/*****************************************
*************show_id**************
******************************************
* show_id shows the entered id on screen, marqueeing it due to the limited amount
* of display positions.
* 	INPUT:
*		void.
*	OUTPUT:
*		void
*/
static void show_id(void);

/*****************************************
*************show_error_msg**************
******************************************
* show_error_msg prints an error message on the display in case of error.
* 	INPUT:
*		void.
*	OUTPUT:
*		void
*/
static void show_error_msg(void);

/*****************************************
*************setup_waiting_pin**************
******************************************
* setup_waiting_pin sets tlhe graphical parts to the 'waiting for pin' state
* 	INPUT:
*		void.
*	OUTPUT:
*		void
*/
static void setup_waiting_pin(void);

/*****************************************
*************toggle_config_mode**************
******************************************
* toggle_config_mode
* 	INPUT:
*		void.
*	OUTPUT:
*		void
*/
static void toggle_config_mode(void);

/*****************************************
*************open_door**************
******************************************
* open_door opens the door after successfully validating the identity of the user.
* 	INPUT:
*		void.
*	OUTPUT:
*		void
*/
static void open_door(void);

/*****************************************
*************close_door**************
******************************************
* close_door closes the door (after a timeout has been reached) for security reasons.
* 	INPUT:
*		void.
*	OUTPUT:
*		void
*/
static void close_door(void);
/*****************************************
*************setup_unblock_user***********
******************************************
* setup_unblock_user
* 	INPUT:
*		void.
*	OUTPUT:
*		void
*/
static void setup_unblock_user(void);
/*****************************************
*************pin_submited**************
******************************************
* pin_submited refer to title for behaviour.
* 	INPUT:
*		void.
*	OUTPUT:
*		void
*/
static void pin_submited(void);

/*****************************************
*************setup_initial_state**************
******************************************
* setup_initial_state
* 	INPUT:
*		void.
*	OUTPUT:
*		void
*/
static void setup_initial_state(void);

/*****************************************
*************setup_admin_menu**************
******************************************
* setup_admin_menu
* 	INPUT:
*		void.
*	OUTPUT:
*		void
*/
static void setup_admin_menu(void);


/*****************************************
*************next_option**************
******************************************
* next_option
* 	INPUT:
*		void.
*	OUTPUT:
*		void
*/
static void next_option(void);

/*****************************************
*************previous_option**************
******************************************
* previous_option
* 	INPUT:
*		void.
*	OUTPUT:
*		void
*/
static void previous_option(void);

/*****************************************
*************select_admin_menu_option**************
******************************************
* select_admin_menu_option
* 	INPUT:
*		void.
*	OUTPUT:
*		void
*/
static void select_admin_menu_option(void);

/*****************************************
*************set_led_page**************
******************************************
* set_led_page
* 	INPUT:
*		void.
*	OUTPUT:
*		void
*/
static void set_led_page(int);

/*****************************************
*************print_admin_menu**************
******************************************
* print_admin_menu
* 	INPUT:
*		void.
*	OUTPUT:
*		void
*/
static void print_admin_menu(void);

/*****************************************
*************setup_remove_user**************
******************************************
* setup_remove_user
* 	INPUT:
*		void.
*	OUTPUT:
*		void
*/
static void setup_remove_user(void);

/*****************************************
*************setup_add_user**************
******************************************
* setup_add_user
* 	INPUT:
*		void.
*	OUTPUT:
*		void
*/
static void setup_add_user(void);

/*****************************************
*************setup_change_pin**************
******************************************
* setup_change_pin sets the display to the changing pin configuration.
* 	INPUT:
*		void.
*	OUTPUT:
*		void
*/
static void setup_change_pin(void);

/*****************************************
*************check_id_to_remove**************
******************************************
* check_id_to_remove checks if a given id may be removed whenever an admin
* tries to remove an id using his config option.
* 	INPUT:
*		void.
*	OUTPUT:
*		void
*/
static void check_id_to_remove(void);

/*****************************************
*************print_initial_msg**************
******************************************
* print_initial_msg prints config message or access message on the display
* for the two menu options, according to the current control status.
* 	INPUT:
*		void.
*	OUTPUT:
*		void
*/
static void print_initial_msg(void);

/*****************************************
*************show_delete_success_msg**************
******************************************
* show_delete_success_msg prints on display a message informing the user that
* a delete user operation has been successful.
* 	INPUT:
*		void.
*	OUTPUT:
*		void
*/
static void show_delete_success_msg(void);

/*****************************************
*************check_id_to_add**************
******************************************
* check_id_to_add checks if the id entered and about to be added to the database is a valid id
* (the id doesn't exist on the current database) and appends an error to the event queue in case it is invalid.
* 	INPUT:
*		void.
*	OUTPUT:
*		void
*/
static void check_id_to_add(void);

/*****************************************
*************show_request_card_msg**************
******************************************
* show_request_card_msg refer to title for behaviour.
* 	INPUT:
*		void.
*	OUTPUT:
*		void
*/
static void show_request_card_msg(void);

/*****************************************
*************check_card_to_add**************
******************************************
* check_card_to_add checks if a given card may be added to the database.
* 	INPUT:
*		void.
*	OUTPUT:
*		void
*/
static void check_card_to_add(void);

/*****************************************
*************check_id_card****************
******************************************
* check_id_card checks if a given card exists.
* 	INPUT:
*		void.
*	OUTPUT:
*		void
*/
static void check_id_card(void);
static void submit_unblock(void);
static void show_add_success_msg(void);
static void commit_add(void);
static void submit_remove(void);
static void commit_change_pin(void);
static void show_pin_change_msg(void);
static void show_unblock_success_msg(void);
static void check_id_to_unblock(void);

fsm_state_t initial_state[] = {
		{.event = CANCEL_EV, .next_state = initial_state, .transition = toggle_config_mode},
		{.event = BACK_EV, .next_state = initial_state, .transition = do_nothing},
		{.event = ENTER_EV, .next_state = waiting_id_state, .transition = setup_waiting_id},
		{.event = MARQUEE_END_EV, .next_state = initial_state, .transition = print_initial_msg},
		{.event = GND_EV, .next_state = initial_state,.transition = do_nothing}
};

fsm_state_t waiting_u_id_confirmation_state[] = {
		{.event = TIMEOUT_EV, .next_state = initial_state, .transition = setup_initial_state},
		{.event = ENTER_EV, .next_state = waiting_db_id_confirmation_state, .transition = check_id},
		{.event = BACK_EV, .next_state = waiting_id_state, .transition = setup_waiting_id},
		{.event = CANCEL_EV, .next_state = initial_state, .transition = setup_initial_state},
		{.event = MARQUEE_END_EV, .next_state = waiting_u_id_confirmation_state, .transition = show_id},
		{.event = VALID_ID_EV, .next_state = waiting_pin_state, .transition = setup_waiting_pin},
		{.event = VALID_CARD_EV, .next_state = waiting_db_id_confirmation_state, .transition = check_id},
		{.event = INVALID_CARD_EV, .next_state = show_error_msg_state_inputid, .transition = show_error_msg},
		{.event = GND_EV, .next_state = waiting_u_id_confirmation_state,.transition = do_nothing}
};

fsm_state_t waiting_id_state[] = {
		{.event = TIMEOUT_EV, .next_state = initial_state, .transition = setup_initial_state},
        {.event = UP_EV, .next_state = waiting_id_state, .transition = increase_digit},
        {.event = DOWN_EV, .next_state = waiting_id_state, .transition = decrease_digit},
        {.event = ENTER_EV, .next_state = waiting_id_state, .transition = next_digit},
        {.event = BACK_EV, .next_state = waiting_id_state, .transition = previous_digit},
		{.event = CANCEL_EV, .next_state = initial_state, .transition = setup_initial_state},
		{.event = SUBMIT_DATA_EV, .next_state = waiting_u_id_confirmation_state, .transition = show_id},
		{.event = CARD_EV, .next_state = waiting_u_id_confirmation_state, .transition = check_id_card},
		{.event = GND_EV, .next_state = waiting_u_id_confirmation_state,.transition = do_nothing}
};

fsm_state_t show_error_msg_state_inputid[] = {
		{.event = TIMEOUT_EV, .next_state = initial_state, .transition = setup_initial_state},
		{.event = ENTER_EV, .next_state = waiting_id_state, .transition = setup_waiting_id},
		{.event = MARQUEE_END_EV, .next_state = show_error_msg_state_inputid, .transition = show_error_msg},
		{.event = GND_EV, .next_state = show_error_msg_state_inputid,.transition = do_nothing}
};

fsm_state_t show_error_msg_state_init[] = {
		{.event = TIMEOUT_EV, .next_state = initial_state, .transition = setup_initial_state},
		{.event = ENTER_EV, .next_state = initial_state, .transition = setup_initial_state},
		{.event = MARQUEE_END_EV, .next_state = show_error_msg_state_init, .transition = show_error_msg},
		{.event = GND_EV, .next_state = show_error_msg_state_init,.transition = do_nothing}
};

fsm_state_t show_error_msg_state_adminmenu[] = {
		{.event = TIMEOUT_EV, .next_state = initial_state, .transition = setup_initial_state},
		{.event = ENTER_EV, .next_state = admin_menu_state, .transition = setup_admin_menu},
		{.event = MARQUEE_END_EV, .next_state = show_error_msg_state_adminmenu, .transition = show_error_msg},
		{.event = GND_EV, .next_state = show_error_msg_state_adminmenu,.transition = do_nothing}
};

fsm_state_t waiting_db_id_confirmation_state[] = {
		{.event = TIMEOUT_EV, .next_state = initial_state, .transition = setup_initial_state},
		{.event = VALID_ID_EV, .next_state = waiting_pin_state, .transition = setup_waiting_pin},
		{.event = INVALID_ID_EV, .next_state = show_error_msg_state_inputid, .transition = show_error_msg},
		{.event = USER_BLOCKED_EV, .next_state = show_error_msg_state_init, .transition = show_error_msg},
		{.event = GND_EV, .next_state = waiting_db_id_confirmation_state,.transition = do_nothing}
};

fsm_state_t waiting_pin_state[] = {
		{.event = TIMEOUT_EV, .next_state = initial_state, .transition = setup_initial_state},
        {.event = UP_EV, .next_state = waiting_pin_state, .transition = increase_digit},
        {.event = DOWN_EV, .next_state = waiting_pin_state, .transition = decrease_digit},
        {.event = ENTER_EV, .next_state = waiting_pin_state, .transition = next_digit},
        {.event = BACK_EV, .next_state = waiting_pin_state, .transition = previous_digit},
		{.event = SUBMIT_DATA_EV, .next_state = waiting_db_pin_confirmation_state, .transition = pin_submited},
		{.event = CANCEL_EV, .next_state = initial_state, .transition = setup_initial_state},
		{.event = GND_EV, .next_state = waiting_pin_state,.transition = do_nothing}
};

fsm_state_t waiting_db_pin_confirmation_state[] = {
		{.event = TIMEOUT_EV, .next_state = initial_state, .transition = setup_initial_state},
		{.event = USER_LOGIN, .next_state = door_open_state, .transition = open_door},
		{.event = ADMIN_LOGIN_CONF, .next_state = admin_menu_state, .transition = setup_admin_menu},
		{.event = USER_LOGIN_CONF, .next_state = user_menu_state, .transition = print_admin_menu},
		{.event = INVALID_PIN_EV, .next_state = show_error_msg_state_init, .transition = show_error_msg},
		{.event = GND_EV, .next_state = waiting_db_pin_confirmation_state,.transition = do_nothing}
};

fsm_state_t door_open_state[] = {
		{.event = TIMEOUT_EV, .next_state = initial_state, .transition = close_door},
		{.event = ENTER_EV, .next_state = initial_state, .transition = close_door},
		{.event = MARQUEE_END_EV, .next_state = door_open_state, .transition = do_nothing},
		{.event = GND_EV, .next_state = door_open_state,.transition = do_nothing}
};

fsm_state_t admin_menu_state[] = {
		{.event = TIMEOUT_EV, .next_state = initial_state, .transition = close_door},
		{.event = UP_EV, .next_state = admin_menu_state, .transition = next_option},
		{.event = DOWN_EV, .next_state = admin_menu_state, .transition = previous_option},
		{.event = ENTER_EV, .next_state = admin_menu_state, .transition = select_admin_menu_option},
		{.event = CANCEL_EV, .next_state = initial_state, .transition = setup_initial_state},
		{.event = MARQUEE_END_EV, .next_state = admin_menu_state, .transition = print_admin_menu},
		{.event = REMOVE_USER_OPT_EV, .next_state = remove_user_state, .transition = setup_remove_user},
		{.event = UNBLOCK_USER_OPT_EV, .next_state = unblock_user_state, .transition = setup_unblock_user},
		{.event = ADD_USER_OPT_EV, .next_state = add_id_user_state, .transition = setup_add_user},
		{.event = CHANGE_PIN_OPT_EV, .next_state = change_pin_state, .transition = setup_change_pin},
		{.event = GND_EV, .next_state = admin_menu_state,.transition = do_nothing}
};

fsm_state_t user_menu_state[] = {
		{.event = TIMEOUT_EV, .next_state = initial_state, .transition = close_door},
		{.event = ENTER_EV, .next_state = change_pin_state, .transition = setup_change_pin},
		{.event = CANCEL_EV, .next_state = initial_state, .transition = setup_initial_state},
		{.event = MARQUEE_END_EV, .next_state = user_menu_state, .transition = print_admin_menu},
		{.event = GND_EV, .next_state = user_menu_state,.transition = do_nothing}
};

fsm_state_t remove_user_state[] = {
		{.event = TIMEOUT_EV, .next_state = initial_state, .transition = close_door},
		{.event = SUBMIT_DATA_EV, .next_state = remove_user_state, .transition = check_id_to_remove},
		{.event = VALID_REMOVE_USER_EV, .next_state = delete_succes_state_msg, .transition = submit_remove},
		{.event = INVALID_ID_EV, .next_state = show_error_msg_state_adminmenu, .transition = show_error_msg},
		{.event = UP_EV, .next_state = remove_user_state, .transition = increase_digit},
		{.event = DOWN_EV, .next_state = remove_user_state, .transition = decrease_digit},
		{.event = ENTER_EV, .next_state = remove_user_state, .transition = next_digit},
		{.event = BACK_EV, .next_state = remove_user_state, .transition = previous_digit},
		{.event = GND_EV, .next_state = remove_user_state,.transition = do_nothing}
};

fsm_state_t delete_succes_state_msg[] = {
		{.event = TIMEOUT_EV, .next_state = initial_state, .transition = close_door},
		{.event = ENTER_EV, .next_state = admin_menu_state, .transition = setup_admin_menu},
		{.event = GND_EV, .next_state = delete_succes_state_msg,.transition = do_nothing}
};

fsm_state_t add_id_user_state[] = {
		{.event = TIMEOUT_EV, .next_state = initial_state, .transition = close_door},
		{.event = SUBMIT_DATA_EV, .next_state = add_id_user_state, .transition = check_id_to_add},
		{.event = VALID_ID_EV, .next_state = add_card_user_state, .transition = show_request_card_msg},
		{.event = INVALID_ID_EV, .next_state = show_error_msg_state_adminmenu, .transition = show_error_msg},
		{.event = UP_EV, .next_state = add_id_user_state, .transition = increase_digit},
		{.event = DOWN_EV, .next_state = add_id_user_state, .transition = decrease_digit},
		{.event = ENTER_EV, .next_state = add_id_user_state, .transition = next_digit},
		{.event = BACK_EV, .next_state = add_id_user_state, .transition = previous_digit},
		{.event = CANCEL_EV, .next_state = admin_menu_state, .transition = setup_admin_menu},
		{.event = GND_EV, .next_state = add_id_user_state,.transition = do_nothing}
};

fsm_state_t add_card_user_state[] = {
		{.event = TIMEOUT_EV, .next_state = initial_state, .transition = close_door},
		{.event = CARD_EV, .next_state = add_card_user_state, .transition = check_card_to_add},
		{.event = VALID_CARD_EV, .next_state = add_pin_user_state, .transition = setup_waiting_pin},
		{.event = INVALID_CARD_EV, .next_state = show_error_msg_state_adminmenu, .transition = show_error_msg},
		{.event = CANCEL_EV, .next_state = admin_menu_state, .transition = setup_admin_menu},
		{.event = MARQUEE_END_EV, .next_state = add_card_user_state, .transition = show_request_card_msg},
		{.event = GND_EV, .next_state = add_card_user_state,.transition = do_nothing}
};

fsm_state_t add_pin_user_state[] = {
		{.event = TIMEOUT_EV, .next_state = initial_state, .transition = close_door},
        {.event = UP_EV, .next_state = add_pin_user_state, .transition = increase_digit},
        {.event = DOWN_EV, .next_state = add_pin_user_state, .transition = decrease_digit},
        {.event = ENTER_EV, .next_state = add_pin_user_state, .transition = next_digit},
        {.event = BACK_EV, .next_state = add_pin_user_state, .transition = previous_digit},
		{.event = SUBMIT_DATA_EV, .next_state = add_success_state, .transition = commit_add},
		{.event = CANCEL_EV, .next_state = admin_menu_state, .transition = setup_admin_menu},
		{.event = GND_EV, .next_state = waiting_pin_state,.transition = do_nothing}
};

fsm_state_t add_success_state[] = {
		{.event = TIMEOUT_EV, .next_state = initial_state, .transition = close_door},
		{.event = MARQUEE_END_EV, .next_state = add_success_state, .transition = show_add_success_msg},
		{.event = ENTER_EV, .next_state = admin_menu_state, .transition = setup_admin_menu},
		{.event = GND_EV, .next_state = add_success_state,.transition = do_nothing}
};

fsm_state_t change_pin_state[] = {
		{.event = TIMEOUT_EV, .next_state = initial_state, .transition = close_door},
		{.event = UP_EV, .next_state = change_pin_state, .transition = increase_digit},
		{.event = DOWN_EV, .next_state = change_pin_state, .transition = decrease_digit},
		{.event = ENTER_EV, .next_state = change_pin_state, .transition = next_digit},
		{.event = BACK_EV, .next_state = change_pin_state, .transition = previous_digit},
		{.event = SUBMIT_DATA_EV, .next_state = change_pin_msg_state, .transition = commit_change_pin},
		{.event = CANCEL_EV, .next_state = initial_state, .transition = setup_initial_state},
		{.event = GND_EV, .next_state = waiting_pin_state,.transition = do_nothing}
};

fsm_state_t change_pin_msg_state[] = {
		{.event = TIMEOUT_EV, .next_state = initial_state, .transition = close_door},
		{.event = MARQUEE_END_EV, .next_state = change_pin_msg_state, .transition = show_add_success_msg},
		{.event = ENTER_EV, .next_state = initial_state, .transition = setup_initial_state},
		{.event = GND_EV, .next_state = change_pin_msg_state,.transition = do_nothing}
};

fsm_state_t unblock_user_state[] = {
		{.event = TIMEOUT_EV, .next_state = initial_state, .transition = setup_initial_state},
		{.event = SUBMIT_DATA_EV, .next_state = unblock_user_state, .transition = check_id_to_unblock},
		{.event = VALID_UNBLOCK_USER_EV, .next_state = unblock_succes_state_msg, .transition = submit_unblock},
		{.event = INVALID_ID_EV, .next_state = show_error_msg_state_adminmenu, .transition = show_error_msg},
		{.event = UP_EV, .next_state = unblock_user_state, .transition = increase_digit},
		{.event = DOWN_EV, .next_state = unblock_user_state, .transition = decrease_digit},
		{.event = ENTER_EV, .next_state = unblock_user_state, .transition = next_digit},
		{.event = BACK_EV, .next_state = unblock_user_state, .transition = previous_digit},
		{.event = GND_EV, .next_state = unblock_user_state,.transition = do_nothing}
};

fsm_state_t unblock_succes_state_msg[] = {
		{.event = TIMEOUT_EV, .next_state = initial_state, .transition = setup_initial_state},
		{.event = ENTER_EV, .next_state = admin_menu_state, .transition = setup_admin_menu},
		{.event = GND_EV, .next_state =unblock_succes_state_msg,.transition = do_nothing}
};



static void increase_digit(void){
    char c = data_helper.data[data_helper.cur_index + data_helper.page_offset];
    if(c < '9' && c > '\0')
        c++;
    else if (c == '9'){
    	if (data_helper.is_pin){
    		if (data_helper.cur_index + data_helper.page_offset == 4)
    			c = '\0';
    	}else
			c = '0';
    }else if(c == '\0' && data_helper.is_pin)
    	c = '0';

    data_helper.data[data_helper.cur_index + data_helper.page_offset] = c;
    display_write_char(c, data_helper.cur_index);
}

static void decrease_digit(void){
	char c = data_helper.data[data_helper.cur_index + data_helper.page_offset];
	    if(c > '0')
	        c--;
	    else if (c == '0'){
	        if(data_helper.is_pin){
	        	if(data_helper.cur_index + data_helper.page_offset == 4)
	        		c = '\0';
	        }else
	        	c = '9';
	    }else if(c == '\0' && data_helper.is_pin)
	    	c = '9';

	    data_helper.data[data_helper.cur_index + data_helper.page_offset] = c;
	    display_write_char(c, data_helper.cur_index);
}

static void next_digit(void){
    if(data_helper.cur_index + data_helper.page_offset < data_helper.data_length - 1){// Si no estoy en el ultimo digito del ID
    	if (data_helper.cur_index == DISPLAY_DIGITS - 1){
    		data_helper.page_offset++;

    	}else
    		data_helper.cur_index++;
    }else if(data_helper.cur_index + data_helper.page_offset == data_helper.data_length - 1){
    	fsm_event_t ev;
    	ev.code = SUBMIT_DATA_EV;
    	push_event(ev);
    }
	re_print_data();
}

static void previous_digit(void){
    if(data_helper.cur_index + data_helper.page_offset > 0){ // Si no estoy editando el primer digito del ID
        if(data_helper.cur_index == 0){
        	data_helper.page_offset--;
        }else
        	data_helper.cur_index--;
    }
	re_print_data();
}

static void setup_waiting_id(void){

	display_stop_marquee();
	strcpy(data_helper.data, "00000000");
	user_data_helper.input_type = EIGHT_DIGIT_PIN;
	data_helper.is_pin = false;
	data_helper.cur_index = 0;
	data_helper.page_offset = 0;
	data_helper.max_pages = 5;
	data_helper.data_length = 8;
	data_helper.masked = false;
	for(int i = 0; i < 4; i++)
		display_write_char('0', i);
	display_set_blinking_all(false);
	display_set_blinking_one(true, 0);
	set_page_indicator();
}

static void setup_initial_state(void){
	bool state = false;
	display_frdm_led_write(GREEN, state);
	display_frdm_led_write(BLUE, state);
	display_frdm_led_write(RED, state);

	display_write_to_led(1, true);
	display_write_to_led(0, true);
	display_write_to_led(2, true);

	display_stop_marquee();
	display_clear_all();
	display_set_blinking_all(false);

	print_initial_msg();
}

fsm_state_t const * fsm_get_init_state(){
	setup_initial_state();
	return initial_state;
}

static void re_print_data(void){
	if(!data_helper.masked){
		for(int i = 0; i < DISPLAY_DIGITS; i++)
			display_write_char(data_helper.data[i + data_helper.page_offset], i);
	} else{
		for(int i = 0; i < DISPLAY_DIGITS; i++)
			if (i != data_helper.cur_index)
				display_write_char(data_helper.mask_char, i);
			else
				display_write_char(data_helper.data[i + data_helper.page_offset], i);
	}
	set_page_indicator();
    display_set_blinking_all(false);
    display_set_blinking_one(true, data_helper.cur_index);
}

static void set_page_indicator(void){

	if(data_helper.max_pages > 1 && data_helper.max_pages < 6){
		switch(data_helper.max_pages){
		case 2:
			if(data_helper.page_offset == 0)
				set_led_page(1);
			else if(data_helper.page_offset == 1)
				set_led_page(5);
			break;
		case 3:
			if(data_helper.page_offset == 0)
				set_led_page(1);
			else if(data_helper.page_offset == 1)
				set_led_page(3);
			else if(data_helper.page_offset == 2)
				set_led_page(5);
			break;
		case 4:
			if(data_helper.page_offset == 0)
				set_led_page(1);
			else if(data_helper.page_offset == 1)
				set_led_page(2);
			else if(data_helper.page_offset == 2)
				set_led_page(4);
			else if(data_helper.page_offset == 3)
				set_led_page(5);
			break;
		case 5:
			if(data_helper.page_offset == 0)
				set_led_page(1);
			else if(data_helper.page_offset == 1)
				set_led_page(2);
			else if(data_helper.page_offset == 2)
				set_led_page(3);
			else if(data_helper.page_offset == 3)
				set_led_page(4);
			else if(data_helper.page_offset == 4)
				set_led_page(5);
			break;
		default:
			set_led_page(0);
			break;
		}
	}
}

static void set_led_page(int page){
	switch(page){
	case 0:
		display_write_to_led(2, false);
		display_write_to_led(1, false);
		display_write_to_led(0, false);
		break;
	case 1:
		display_write_to_led(2, true);
		display_write_to_led(1, false);
		display_write_to_led(0, false);
		break;
	case 2:
		display_write_to_led(2, true);
		display_write_to_led(1, true);
		display_write_to_led(0, false);
		break;
	case 3:
		display_write_to_led(2, false);
		display_write_to_led(1, true);
		display_write_to_led(0, false);
		break;
	case 4:
		display_write_to_led(2, false);
		display_write_to_led(1, true);
		display_write_to_led(0, true);
		break;
	case 5:
		display_write_to_led(2, false);
		display_write_to_led(1, false);
		display_write_to_led(0, true);
		break;
	default:
		display_write_to_led(2, false);
		display_write_to_led(1, false);
		display_write_to_led(0, false);
		break;
	}
}

static void check_id(void){
	fsm_event_t ev;
	char data[100];
	strcpy(user_data_helper.id, data_helper.data);
	if(user_data_helper.input_type == EIGHT_DIGIT_PIN)
		strcpy(data, user_data_helper.id);
	else if (user_data_helper.input_type == MAGNETIC_CARD)
	{
		strcpy(data, user_data_helper.card);
		u_get_pin_from_card(data, user_data_helper.id);
	}

	if(u_exists(user_data_helper.input_type, data)){
		if(u_is_blocked(user_data_helper.input_type, data)){
			ev.code = USER_BLOCKED_EV;
			data_helper.error_code = BLOCKED_ERROR;
		}else{
			if( (u_get_n_tries(EIGHT_DIGIT_PIN, user_data_helper.id) >=3) && !u_is_admin(EIGHT_DIGIT_PIN, user_data_helper.id)){
				u_block(EIGHT_DIGIT_PIN, user_data_helper.id);
				ev.code = USER_BLOCKED_EV;
				data_helper.error_code = BLOCKED_ERROR;
			}
			else
				ev.code = VALID_ID_EV;
		}

	}else{
		ev.code = INVALID_ID_EV;
		data_helper.error_code = INVALID_ERROR;
	}
	push_event(ev);
}

static void show_id(void){
	display_clear_all();
	display_set_blinking_all(false);
	display_marquee(data_helper.data, DISPLAY_INT_LEFT);


}
static void setup_unblock_user(void){
	//pedir que ingrese el id a eliminar o pase la tarjeta
	setup_waiting_id();
}
static void show_error_msg(void){
	display_stop_marquee();
	display_clear_all();
	display_set_blinking_all(false);

	display_write_or_marquee(error_msgs[data_helper.error_code],DISPLAY_INT_LEFT);
}


static void toggle_config_mode(void){
	config_mode = !config_mode;
	if (config_mode){
		display_set_blinking_leds_all(true);
	}else
		display_set_blinking_leds_all(false);
	print_initial_msg();
}

static void setup_waiting_pin(void){
	// definir que tiene mask
	display_stop_marquee();
	data_helper.masked = true;
	data_helper.is_pin = true;
	strcpy(data_helper.data, "00000\0");
	data_helper.mask_char = 'o';
	data_helper.max_pages = 2;
	data_helper.page_offset = 0;
	data_helper.cur_index = 0;
	data_helper.data_length = 5;
	if(data_helper.masked){
		char c = data_helper.mask_char;
		char mask[DISPLAY_DIGITS + 1] = {c,c,c,c,'\0'};
		display_write_word(mask);
	}
	re_print_data();
	display_set_blinking_all(false);
	display_set_blinking_one(true, 0);
	set_page_indicator();
}

static void pin_submited(void){
	strcpy(user_data_helper.pin, data_helper.data);
	fsm_event_t ev;
	char data[100];
	bool valid = false;
	bool blocked = true;
	bool is_admin = false;

	if(user_data_helper.input_type == EIGHT_DIGIT_PIN)
		strcpy(data, user_data_helper.id);
	else
		strcpy(data, user_data_helper.card);

	valid = u_validate(user_data_helper.input_type, data, user_data_helper.pin);
	blocked = u_is_blocked(user_data_helper.input_type, data);

	if(valid){
		if(!blocked){
			if(config_mode){
				is_admin = u_is_admin(user_data_helper.input_type, data);
				if(is_admin){
					ev.code = ADMIN_LOGIN_CONF;
					strcpy(logged_usr.id, user_data_helper.id);
					strcpy(logged_usr.pin, user_data_helper.pin);
				}
				else{
					ev.code = USER_LOGIN_CONF;
					strcpy(logged_usr.id, user_data_helper.id);
					strcpy(logged_usr.pin, user_data_helper.pin);
					admin_menu_helper.option = CHANGE_PIN_ENTRY;
				}
			}else
				ev.code = USER_LOGIN;
		}
	}else{
		if(blocked){
			ev.code = USER_BLOCKED_EV;
			data_helper.error_code = USER_BLOCKED_EV;
		}else{
			ev.code = INVALID_PIN_EV;
			data_helper.error_code = WRONG_PASS;
		}
	}
	push_event(ev);
}

static void open_door(void){
	display_stop_marquee();
	display_clear_all();
	display_set_blinking_all(false);
	display_marquee("door Open",DISPLAY_INT_LEFT);

	display_frdm_led_write(BLUE, false);
	display_frdm_led_write(GREEN, true);
	display_frdm_led_write(RED, false);

}

static void close_door(void){
	setup_initial_state();
}

static void next_option(void){
	admin_menu_helper.option = admin_menu_helper.option + 1;
	if(admin_menu_helper.option == N_ENTRIES)
		admin_menu_helper.option = 0;
	print_admin_menu();

}

static void previous_option(void){
	if(admin_menu_helper.option == 0)
		admin_menu_helper.option = N_ENTRIES;
	admin_menu_helper.option--;
	print_admin_menu();
}

static void select_admin_menu_option(void){
	fsm_event_t ev;
	if(admin_menu_helper.option == ADD_USER_ENTRY)
		ev.code = ADD_USER_OPT_EV;
	else if(admin_menu_helper.option == DELETE_USER_ENTRY)
		ev.code = REMOVE_USER_OPT_EV;
	else if(admin_menu_helper.option == CHANGE_PIN_ENTRY)
		ev.code = CHANGE_PIN_OPT_EV;
	else if(admin_menu_helper.option == UNBLOCK_USER_ENTRY)
		ev.code = UNBLOCK_USER_OPT_EV;
	push_event(ev);
}


static void setup_admin_menu(void){
	admin_menu_helper.option = 0;
	print_admin_menu();
}

static void print_admin_menu(void){
	display_set_blinking_all(false);
	display_stop_marquee();
	display_clear_all();
	display_write_or_marquee(admin_options[admin_menu_helper.option], DISPLAY_INT_LEFT);
}

static void setup_remove_user(void){
	//pedir que ingrese el id a eliminar o pase la tarjeta
	setup_waiting_id();

}

void setup_add_user(void){
	setup_waiting_id();
}

void setup_change_pin(void){
	setup_waiting_pin();
}

static void check_id_to_remove(void){
	strcpy(aux_user.id, data_helper.data);
	fsm_event_t ev;
	if(u_exists(EIGHT_DIGIT_PIN, aux_user.id) && !u_is_admin(EIGHT_DIGIT_PIN, aux_user.id)){
		ev.code = VALID_REMOVE_USER_EV;
		strcpy(aux_user.id, data_helper.data);
	}else if(!u_exists(EIGHT_DIGIT_PIN, aux_user.id)){
		ev.code = INVALID_ID_EV;
		data_helper.error_code = DONT_EXIST_ERROR;
	}else if(u_is_admin(EIGHT_DIGIT_PIN, aux_user.id)){
		ev.code = INVALID_ID_EV;
		data_helper.error_code = LAST_ADMIN_ERROR;
	}
	push_event(ev);
}

static void print_initial_msg(void){
	display_clear_all();
	display_stop_marquee();
	if(config_mode)
		display_write_or_marquee("config", DISPLAY_INT_LEFT);
	else
		display_write_or_marquee("access", DISPLAY_INT_LEFT);
}

static void submit_remove(void){
	u_remove(EIGHT_DIGIT_PIN, aux_user.id);
	show_delete_success_msg();
}

static void show_delete_success_msg(void){
	display_clear_all();
	display_stop_marquee();
	display_set_blinking_all(false);
	display_write_or_marquee("User deleted" , DISPLAY_INT_LEFT);
}

static void check_id_to_add(void){
	strcpy(aux_user.id, data_helper.data);
	fsm_event_t ev;
	if(!u_exists(EIGHT_DIGIT_PIN, aux_user.id)){
		strcpy(aux_user.id, data_helper.data);
		ev.code = VALID_ID_EV;
	}else{
		ev.code = INVALID_ID_EV;
		data_helper.error_code = ALREADY_EXISTS_ERROR;
	}
	push_event(ev);
}

static void show_request_card_msg(void){
	display_clear_all();
	display_set_blinking_all(false);
	display_write_or_marquee("slide card", DISPLAY_INT_LEFT);
}

static void check_card_to_add(void){
	fsm_event_t ev;
	if(aux_ms_ev.data != NULL)
	{
		if(!u_exists(MAGNETIC_CARD, (char *)  aux_ms_ev.data)){
			strcpy((char *) aux_user.card, (char *)  aux_ms_ev.data);
			ev.code = VALID_CARD_EV;
		}else{
			ev.code = INVALID_CARD_EV;
			data_helper.error_code = CARD_IN_USE_ERROR;
		}
	}
	else
	{
		ev.code = INVALID_CARD_EV;
		data_helper.error_code = ERROR_CARD_READ;
	}
	push_event(ev);
}

static void check_id_card(void){
	fsm_event_t ev;
	if(aux_ms_ev.data != NULL){
		ev.code = VALID_CARD_EV;
		user_data_helper.input_type = MAGNETIC_CARD;
		strcpy((char *) user_data_helper.card,(char *) aux_ms_ev.data);

	}else{
		ev.code = INVALID_CARD_EV;
		data_helper.error_code = ERROR_CARD_READ;

	}
	push_event(ev);
}

static void commit_add(void){
	strcpy(aux_user.pin, data_helper.data);
	u_add(EIGHT_DIGIT_PIN, aux_user.id, aux_user.pin);
	u_new_id(EIGHT_DIGIT_PIN, aux_user.id, MAGNETIC_CARD, aux_user.card);
	show_add_success_msg();

}

static void show_add_success_msg(void){
	display_clear_all();
	display_set_blinking_all(false);
	display_write_or_marquee("User added", DISPLAY_INT_LEFT);
}

static void commit_change_pin(void){
	u_change_password(EIGHT_DIGIT_PIN, logged_usr.id, data_helper.data);
	show_pin_change_msg();
}

static void show_pin_change_msg(void){
	display_clear_all();
	display_set_blinking_all(false);
	display_write_or_marquee("pin changed", DISPLAY_INT_LEFT);
}




static void check_id_to_unblock(void){
	strcpy(aux_user.id, data_helper.data);
	fsm_event_t ev;
	if(u_exists(EIGHT_DIGIT_PIN, aux_user.id)){
		ev.code = VALID_UNBLOCK_USER_EV;
		strcpy(aux_user.id, data_helper.data);
	}else if(!u_exists(EIGHT_DIGIT_PIN, aux_user.id)){
		ev.code = INVALID_ID_EV;
		data_helper.error_code = DONT_EXIST_ERROR;
	}
	push_event(ev);
}



static void submit_unblock(void){
	u_unblock(EIGHT_DIGIT_PIN, aux_user.id);
	show_unblock_success_msg();
}


static void show_unblock_success_msg(void){
	display_clear_all();
	display_stop_marquee();
	display_set_blinking_all(false);
	display_write_or_marquee("User unblocEd" , DISPLAY_INT_LEFT);
}
