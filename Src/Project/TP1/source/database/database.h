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

#define ID_LEN      8    // only valid ID len
#define MAX_ID_NUM  99999999


// valid length ranges for validator string, by type
#define PIN_MIN_LEN 4
#define PIN_MAX_LEN 5

#define CARD_LEN        37
#define CARD_MIN_LEN    CARD_LEN
#define CARD_MAX_LEN    CARD_LEN

/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/
typedef enum {MAGNETIC_CARD,    FIVE_DIGIT_PIN, N_VALIDATORS} id_validator_type_t;

typedef struct {
    id_validator_type_t type;
    char * data; // string, 0 terminated
} id_validator_t;

typedef struct {    // id left as whole int for efficiency, two ints would be necessary either way
    uint32_t id;            // 2^26-1=67,108,864; 2^27-1=134,217,727 -> 27 bits necessary for 8 digit id
    uint8_t admin : 1;     // booleans
    uint8_t blocked : 1;
    uint8_t free : 1;      // true if valid user
    uint8_t index : 5;     // 5 bits necessary for 31 users
    id_validator_t validators[N_VALIDATORS];
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
bool u_exists(char * id);

/**
 * @brief Validates access
 * @param id: user id, 0 terminated
 * @param validator: access code given
 * @return true if user is not blocked and validator matches, false otherwise
 */
bool u_validate(char * id, id_validator_t validator);

/**
 * @brief Block user from accessing
 * @param id: user id, 0 terminated
 */
void u_block(char * id);

/**
 * @brief Allow user access
 * @param id: user id, 0 terminated
 */
void u_unblock(char * id);

/**
 * @brief Give user admin privileges
 * @param id: user id, 0 terminated
 */
void u_make_admin(char * id);

/**
 * @brief Remove admin privileges from user
 * @param id: user id, 0 terminated
 * @return operation success. Will fail if user does not exists or is the sole admin
 */
bool u_remove_as_admin(char * id);

/**
 * @brief Checks whether user has admin privileges
 * @param id: user id, 0 terminated
 * @return true if user has admin privileges
 */
bool u_is_admin(char * id);

/**
 * @brief Checks whether user is blocked
 * @param id: user id, 0 terminated
 * @return true if user is blocked
 */
bool u_is_blocked(char * id);

/**
 * @brief Change or add specific validator for user
 * @param id: user id, 0 terminated
 * @param validator: new validator
 * @return exit success
 */
bool u_change_validator(char * id, id_validator_t validator);

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
bool u_add(char * id, id_validator_t validator);

/**
 * @brief Delete user
 * @param id: user id, 0 terminated
 */
void u_remove(char * id);
/*******************************************************************************
 ******************************************************************************/


#endif //SOURCE_DATABASE_H
