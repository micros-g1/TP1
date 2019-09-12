//
// Created by lisan on 6/9/2019.
//

#include "fsm_sates.h"
#include "string.h"
#include <stdbool.h>

#define DISPLAY_DIGITS 4
fsm_state_t waiting_id_state[];
fsm_state_t initial_state[];
fsm_state_t waiting_db_id_confirmation_state[];
fsm_state_t waiting_u_id_confirmation_state[];
fsm_state_t show_error_msg_init[];
fsm_state_t set_pin_state[];
fsm_state_t waiting_pin_state[];
fsm_state_t admin_menu_state[];
fsm_state_t waiting_db_pin_confirmation_state[];
fsm_state_t door_open_state[];
fsm_state_t admin_menu_state[];
fsm_state_t change_pin_state[];
fsm_state_t remove_user_state[];
fsm_state_t add_user_state[];

void do_nothing(void){}
void setup_waiting_id(void);
void increase_digit(void);
void decrease_digit(void);
void next_digit(void);
void previous_digit(void);
void re_print_data(void);
void set_page_indicator(void);
void check_id(void);
void show_id(void);
void show_error_msg(void);
void check_initial_db_status(void);
void setup_waiting_pin(void);
void toggle_config_mode(void);
void open_door(void);
void close_door(void);
void pin_submited(void);
void setup_initial_state(void);
void setup_admin_menu(void);
void setup_set_pin(void);
void next_option(void);
void previous_option(void);
void select_admin_menu_option(void);
void set_led_page(int);
void print_admin_menu(void);
void setup_remove_user(void);
void setup_add_user(void);
void setup_change_pin(void);

fsm_state_t initial_state[] = {
		{.event = CANCEL_EV, .next_state = initial_state, .transition = toggle_config_mode},
		{.event = BACK_EV, .next_state = initial_state, .transition = do_nothing},
		{.event = ENTER_EV, .next_state = waiting_id_state, .transition = setup_waiting_id}
};

fsm_state_t waiting_u_id_confirmation_state[] = {
		{.event = ENTER_EV, .next_state = waiting_db_id_confirmation_state, .transition = check_id},
		{.event = BACK_EV, .next_state = waiting_id_state, .transition = setup_waiting_id},
		{.event = MARQUEE_END_EV, .next_state = waiting_u_id_confirmation_state, .transition = show_id},
		{.event = VALID_ID_EV, .next_state = waiting_pin_state, .transition = setup_waiting_pin}
};

fsm_state_t waiting_id_state[] = {
        {.event = UP_EV, .next_state = waiting_id_state, .transition = increase_digit},
        {.event = DOWN_EV, .next_state = waiting_id_state, .transition = decrease_digit},
        {.event = ENTER_EV, .next_state = waiting_id_state, .transition = next_digit},
        {.event = BACK_EV, .next_state = waiting_id_state, .transition = previous_digit},
		{.event = CANCEL_EV, .next_state = initial_state, .transition = setup_initial_state},
		{.event = SUBMIT_DATA_EV, .next_state = waiting_u_id_confirmation_state, .transition = show_id}
};

fsm_state_t show_error_msg_state_init[] = {
		{.event = ENTER_EV, .next_state = initial_state, .transition = setup_initial_state},
		{.event = MARQUEE_END_EV, .next_state = show_error_msg_state_init, .transition = show_error_msg}
};


fsm_state_t waiting_db_id_confirmation_state[] = {
		{.event = VALID_ID_EV, .next_state = waiting_pin_state, .transition = setup_waiting_pin},
		{.event = INVALID_ID_EV, .next_state = show_error_msg_state_init, .transition = show_error_msg},
		{.event = USER_BLOCKED_EV, .next_state = show_error_msg_state_init, .transition = show_error_msg},
};

fsm_state_t waiting_pin_state[] = {
        {.event = UP_EV, .next_state = waiting_pin_state, .transition = increase_digit},
        {.event = DOWN_EV, .next_state = waiting_pin_state, .transition = decrease_digit},
        {.event = ENTER_EV, .next_state = waiting_pin_state, .transition = next_digit},
        {.event = BACK_EV, .next_state = waiting_pin_state, .transition = previous_digit},
		{.event = SUBMIT_DATA_EV, .next_state = waiting_db_pin_confirmation_state, .transition = pin_submited}
};

fsm_state_t waiting_db_pin_confirmation_state[] = {
		{.event = USER_LOGIN, .next_state = door_open_state, .transition = open_door},
		{.event = ADMIN_LOGIN_CONF, .next_state = admin_menu_state, .transition = setup_admin_menu},
		{.event = USER_LOGIN_CONF, .next_state = initial_state, .transition = setup_set_pin},
		{.event = INVALID_PIN_EV, .next_state = show_error_msg_state_init, .transition = show_error_msg}
};

fsm_state_t door_open_state[] = {
		{.event = ENTER_EV, .next_state = initial_state, .transition = close_door},
		{.event = MARQUEE_END_EV, .next_state = door_open_state, .transition = do_nothing}
};

fsm_state_t admin_menu_state[] = {
		{.event = UP_EV, .next_state = admin_menu_state, .transition = next_option},
		{.event = DOWN_EV, .next_state = admin_menu_state, .transition = previous_option},
		{.event = ENTER_EV, .next_state = admin_menu_state, .transition = select_admin_menu_option},
		{.event = CANCEL_EV, .next_state = initial_state, .transition = setup_initial_state},
		{.event = MARQUEE_END_EV, .next_state = admin_menu_state, .transition = print_admin_menu},
		{.event = REMOVE_USER_OPT_EV, .next_state = remove_user_state, .transition = setup_remove_user},
		{.event = ADD_USER_OPT_EV, .next_state = add_user_state, .transition = setup_add_user},
		{.event = CHANGE_PIN_OPT_EV, .next_state = change_pin_state, .transition = setup_change_pin}
};

fsm_state_t remove_user_state[] = {

};
void increase_digit(void){
    char c = data_helper.data[data_helper.cur_index + data_helper.page_offset];
    if(c < '9')
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

void decrease_digit(void){
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

void next_digit(void){
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

void previous_digit(void){
    if(data_helper.cur_index + data_helper.page_offset > 0){ // Si no estoy editando el primer digito del ID
        if(data_helper.cur_index == 0){
        	data_helper.page_offset--;
        }else
        	data_helper.cur_index--;
    }
	re_print_data();
}

void setup_waiting_id(void){
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

void setup_initial_state(void){
	bool state = true;
	display_frdm_led_write(GREEN, state);
	display_frdm_led_write(BLUE, state);
	display_frdm_led_write(RED, state);

	display_write_to_led(1, true);
	display_write_to_led(0, true);
	display_write_to_led(2, true);

	display_stop_marquee();
	display_clear_all();

	display_write_char(' ', 0);
	display_write_char(' ', 1);
	display_write_char(' ', 2);
	display_write_char(' ', 3);
	display_set_blinking_all(false);
	config_mode = false;
}

fsm_state_t * fsm_get_init_state(){
	setup_initial_state();
	return initial_state;
}

void re_print_data(void){
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

void set_page_indicator(void){

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

void set_led_page(int page){
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

void check_id(void){
	fsm_event_t ev;
	char data[100];
	strcpy(user_data_helper.id, data_helper.data);
	if(user_data_helper.input_type == EIGHT_DIGIT_PIN)
		strcpy(data, user_data_helper.id);
	else if (user_data_helper.input_type == MAGNETIC_CARD)
		strcpy(data, user_data_helper.card);


	switch(use_mode){
	case USER_ACCESS:
		if(u_exists(user_data_helper.input_type, data)){
			if(u_is_blocked(user_data_helper.input_type, data)){
				ev.code = USER_BLOCKED_EV;
				data_helper.error_code = BLOCKED_ERROR;
			}else
				ev.code = VALID_ID_EV;
		}else{
			ev.code = INVALID_ID_EV;
			data_helper.error_code = INVALID_ERROR;
		}
		break;
	case REMOVE_USER:

		if(u_exists(user_data_helper.input_type, data)){
			if(u_n_admins() == 1){
				ev.code = INVALID_ID_EV;
				data_helper.error_code = LAST_ADMIN_ERROR;
			}else
				ev.code = VALID_ID_EV;
		}else{
			ev.code = INVALID_ID_EV;
			data_helper.error_code = INVALID_ERROR;
		}
		break;
	case ADD_USER:
		if(u_exists(user_data_helper.input_type, data)){
			ev.code = INVALID_ID_EV;
			data_helper.error_code = ALREADY_EXISTS_ERROR;
		}
	}
	push_event(ev);
}

void show_id(void){
	display_clear_all();
	display_set_blinking_all(false);
	display_marquee(data_helper.data, DISPLAY_INT_LEFT);


}

void show_error_msg(void){
	display_stop_marquee();
	display_clear_all();
	display_set_blinking_all(false);

	display_write_or_marquee(error_msgs[data_helper.error_code],DISPLAY_INT_LEFT);
}

void check_initial_db_status(void){
}

void toggle_config_mode(void){
	config_mode = !config_mode;
	if (config_mode){
		display_set_blinking_leds_all(true);
	}else
		display_set_blinking_leds_all(false);
}

void setup_waiting_pin(void){
	// definir que tiene mask
	display_stop_marquee();
	data_helper.masked = true;
	data_helper.is_pin = true;
	strcpy(data_helper.data, "00000000");
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

void pin_submited(void){
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
				if(is_admin)
					ev.code = ADMIN_LOGIN_CONF;
				else
					ev.code = USER_LOGIN_CONF;
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

void open_door(void){
	display_stop_marquee();
	display_clear_all();
	display_set_blinking_all(false);
	display_marquee("door Open",DISPLAY_INT_LEFT);
	bool blink = true;
	display_frdm_led_blink(GREEN, blink);
	display_frdm_led_blink(BLUE, blink);
	display_frdm_led_blink(RED, blink);

	display_frdm_led_write(BLUE, true);
	display_frdm_led_write(GREEN, false);
	display_frdm_led_write(RED, true);

}

void close_door(void){
	setup_initial_state();
}

void next_option(void){
	admin_menu_helper.option = (admin_menu_helper.option + 1) % N_ENTRIES;
	print_admin_menu();

}

void previous_option(void){
	admin_menu_helper.option = abs((admin_menu_helper.option - 1) % N_ENTRIES);
	print_admin_menu();
}

void select_admin_menu_option(void){
	fsm_event_t ev;
	if(admin_menu_helper.option == ADD_USER_ENTRY)
		ev.code = ADD_USER_OPT_EV;
	else if(admin_menu_helper.option == DELETE_USER_ENTRY)
		ev.code = REMOVE_USER_OPT_EV;
	else if(admin_menu_helper.option == CHANGE_PIN_ENTRY)
		ev.code = CHANGE_PIN_OPT_EV;
	push_event(ev);
}

void setup_set_pin(void){
	// Copie y pegue setup_waiting_pin. fijarse si esta bien
	display_stop_marquee();
	data_helper.masked = true;
	strcpy(data_helper.data, "00000000");
	data_helper.mask_char = 'P';
	data_helper.max_pages = 2;
	data_helper.page_offset = 0;
	data_helper.cur_index = 0;
	data_helper.data_length = 5;
	if(data_helper.masked){
		char c = data_helper.mask_char;
		char mask[DISPLAY_DIGITS + 1] = {c,c,c,c,'\0'};
		display_write_word(mask);
	}
	display_set_blinking_all(false);
	display_set_blinking_one(true, 0);
	set_page_indicator();
}

void setup_admin_menu(void){
	admin_menu_helper.option = 0;
	print_admin_menu();
}

void print_admin_menu(void){
	display_set_blinking_all(false);
	display_stop_marquee();
	display_clear_all();
	display_write_or_marquee(admin_options[admin_menu_helper.option], DISPLAY_INT_LEFT);
}

void setup_remove_user(void){
	//pedir que ingrese el id a eliminar o pase la tarjeta
	setup_waiting_id();
	//verificar si es valido. actuar en consecuencia
}

void setup_add_user(void){
	//si ingresa tarjeta luego pedir id.
	//si ingresa id luego pedir tarjeta
	setup_waiting_id();
	//verificar que no exista ni tarjeta ni id ingresados
	//ingresar pin
}

void setup_change_pin(void){
	//ingresar nuevo pin 2 veces, verificar que sean iguales y verificar que sea distinto al anterior
	setup_waiting_pin();
}
