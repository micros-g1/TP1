/***************************************************************************//**
  @file     database.h
  @brief    User database for security system
  @author   22.99 2019 Group 1: Alvarez, Gonzalez, Parra, Reina
 ******************************************************************************/

#ifndef SOURCE_DATABASE_H
#define SOURCE_DATABASE_H

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/
#include <stdint.h>
#include <stdbool.h>


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define MAX_USER_N  32

#define MAX_ID_NUM  99999999


// valid length ranges for validator string, by type
#define PASSWORD_MIN_LEN 4
#define PASSWORD_MAX_LEN 5

#define CARD_LEN        40// only valid card len
#define PIN_LEN      8    // only valid pin ID len

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef enum {MAGNETIC_CARD,    EIGHT_DIGIT_PIN, N_IDS} id_type_t;

typedef struct {
    id_type_t type;
    char * data; // string, 0 terminated
} id_t;


typedef struct {    // id left as whole int for efficiency, two ints would be necessary either way
    char password [PASSWORD_MAX_LEN + 1];  // 2^26-1=67,108,864; 2^27-1=134,217,727 -> 27 bits necessary for 8 digit id
    uint8_t admin : 1;     // booleans
    uint8_t blocked : 1;
    uint8_t free : 1;      // true if valid user
    uint8_t index : 5;     // 5 bits necessary for 32 users
    uint8_t n_tries;
    id_t ids[N_IDS];
} user_t;


/*******************************************************************************
 * VARIABLE PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/*******************************************************************************
 * FUNCTION PROTOTYPES WITH GLOBAL SCOPE
 ******************************************************************************/

/**
 * @brief Initialize user database
 */
void u_init();

/**
 * @brief Checks whether any user in the database matches the given ID
 * @param id: user id, 0 terminated
 * @return true if user exists
 */
bool u_exists(id_type_t id_type, char * id);

/**
 * @brief Validates access
 * @param id: user id, 0 terminated
 * @param validator: access code given
 * @return true if user is not blocked and validator matches, false otherwise
 */

bool u_validate(id_type_t id_type, char * id, char * password);

/**
 * @brief Block user from accessing
 * @param id: user id, 0 terminated
 */
void u_block(id_type_t id_type, char * id);

/**
 * @brief Allow user access
 * @param id: user id, 0 terminated
 */
void u_unblock(id_type_t id_type, char * id);

/**
 * @brief Give user admin privileges
 * @param id: user id, 0 terminated
 */
void u_make_admin(id_type_t id_type, char * id);

/**
 * @brief Remove admin privileges from user
 * @param id: user id, 0 terminated
 * @return operation success. Will fail if user does not exists or is the sole admin
 */
bool u_remove_as_admin(id_type_t id_type, char * id);

/**
 * @brief Checks whether user has admin privileges
 * @param id: user id, 0 terminated
 * @return true if user has admin privileges
 */
bool u_is_admin(id_type_t id_type, char * id);

/**
 * @brief Checks whether user is blocked
 * @param id: user id, 0 terminated
 * @return true if user is blocked
 */
bool u_is_blocked(id_type_t id_type, char * id);

uint32_t u_get_n_tries(id_type_t id_type, char * id);
void u_reset_n_tries(id_type_t id_type, char * id);

/**
 * @brief Change or add specific validator for user
 * @param id: user id, 0 terminated
 * @param validator: new validator
 * @return exit success
 */
bool u_change_password(id_type_t id_type, char * id, char * password);

/**
 * @brief Total number of users currently in the database, including blocked users and admins
 * @return Current number of users
 */
uint32_t u_n_users();

/**
 * @brief Total number of admins currently in the database, including blocked users
 * @return Current number of admins
 */
uint32_t u_n_admins();

/**
 * @brief Total number of blocked users currently in the database, including admins
 * @return Current number of blocked users
 */
uint32_t u_n_blocked();

/**
 * @brief Add new user to the data base, with one validator associated
 * @param id: new user id, 0 terminated
 * @param validator: access validator associated to new user
 * @return Operation success. Will fail if ID is already in use
 */
bool u_add(id_type_t id_type, char * id, char * password);


bool u_new_id(id_type_t id_type, char * id, id_type_t new_id_type, char * new_id);
/**
 * @brief Delete user
 * @param id: user id, 0 terminated
 */
void u_remove(id_type_t id_type, char * id);
/*******************************************************************************
 ******************************************************************************/

uint8_t u_get_tries(id_type_t id_type, char * id);

void u_get_pin_from_card(char * card, char * pin);

#endif //SOURCE_DATABASE_H
