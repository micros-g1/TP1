/***************************************************************************//**
  @file     database.c
  @brief    User database for security system
  @author   22.99 2019 Group 1: Alvarez, Gonzalez, Parra, Reina
 ******************************************************************************/


/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "database.h"
#include <string.h>
#include <stdlib.h>


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/


/*******************************************************************************
 * ENUMERATIONS AND STRUCTURES AND TYPEDEFS
 ******************************************************************************/

typedef struct {
    uint32_t n_users;   // current number of users in the database
    uint32_t blocked;
    uint32_t admins;
} database_metadata_t;


/*******************************************************************************
 * VARIABLES WITH FILE SCOPE
 ******************************************************************************/
static database_metadata_t metadata;
static user_t database[MAX_USER_N];

static char last_queried_id[ID_LEN+1]; // reduce need for iteration
static unsigned char last_queried_user;

static char cards[CARD_MAX_LEN+1][MAX_USER_N];
static char pins[PIN_MAX_LEN+1][MAX_USER_N];
static uint32_t max_lens[N_VALIDATORS];
static uint32_t min_lens[N_VALIDATORS];


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH FILE SCOPE
 ******************************************************************************/

/**
 * @brief Look for id in database. Updates last query.
 * @param id: user id, 0 terminated
 * @return Index for user in database. MAX_USER_N if not found
 */
uint32_t query(char * id);

/**
 * @brief Validate access. Same functionality as "u_validate"
 * @param u: user
 * @param v: access code to check
 * @return true if user is not blocked and validator matches, false otherwise
 */
bool check_validator(user_t u, id_validator_t v);

/**
 * @brief Change validator for user. Same functionality as "u_change_validator".
 * @param u: user
 * @param v: new validator
 * @return Exit success
 */
bool update_validator(user_t u, id_validator_t v);

/**
 * @brief Converts id string to id number
 * @param id: user id, 0 terminated
 * @return id number. MAX_ID_NUMBER+1 if input not valid
 */
unsigned int id_to_num(char * id);


/*******************************************************************************
 * FUNCTIONS WITH GLOBAL SCOPE
 ******************************************************************************/

void u_init()
{
    metadata.n_users = 1; // initialize with one default admin user
    metadata.blocked =  0;
    metadata.admins =   1;

    memset(last_queried_id, '0', sizeof(last_queried_id));
    last_queried_id[ID_LEN] = 0;
    last_queried_user = 0;

    unsigned int i, j;
    for(i = 0; i < MAX_USER_N; i++) {
        database[i].free = true;
        database[i].blocked = false;
        database[i].admin = false;
        database[i].index = i;

        for (j = 0; j < N_VALIDATORS; j++) {
            database[i].validators[j].type = j;
        }
        database[i].validators[FIVE_DIGIT_PIN].data = pins[i];
        pins[i][0] = 0;
        database[i].validators[MAGNETIC_CARD].data = cards[i];
        cards[i][0] = 0;
    }

    max_lens[FIVE_DIGIT_PIN] = PIN_MAX_LEN;
    min_lens[FIVE_DIGIT_PIN] = PIN_MIN_LEN;

    max_lens[MAGNETIC_CARD] = CARD_MAX_LEN;
    min_lens[MAGNETIC_CARD] = CARD_MIN_LEN;

    database[0].id = 0;  // default admin data
    database[0].blocked = false;
    database[0].admin = true;
    database[0].free = false;
    strcpy(database[0].validators[FIVE_DIGIT_PIN].data, "0000");
}


bool u_validate(char * id, id_validator_t validator)
{
    bool valid = false;
    uint32_t index = query(id);

    if (index < MAX_USER_N && !database[index].blocked) {
        valid = check_validator(database[index], validator);
    }

    return valid;
}


void u_block(char * id) {
    uint32_t index = query(id);

    if (index < MAX_USER_N && !database[index].blocked) {
        database[index].blocked = true;
        metadata.blocked++;
    }
}


void u_unblock(char * id)
{
    uint32_t index = query(id);

    if (index < MAX_USER_N && database[index].blocked) {
        database[index].blocked = false;
        metadata.blocked--;
    }
}


void u_make_admin(char * id)
{
    uint32_t index = query(id);

    if (index < MAX_USER_N && !database[index].admin) {
        database[index].admin = true;
        metadata.admins++;
    }
}


bool u_remove_as_admin(char * id)
{
    bool success = false;
    if (metadata.admins > 1) {
        uint32_t index = query(id);

        if (index < MAX_USER_N && database[index].admin) {
            database[index].admin = false;
            metadata.admins--;
        }
    }
}


bool u_change_validator(char * id, id_validator_t validator)
{
    bool success = false;
    uint32_t index = query(id);
    if (index < MAX_USER_N) {
        success = update_validator(database[index], validator);
    }

    return success;
}


bool u_add(char * id, id_validator_t validator)
{
    bool success = false;
    if (metadata.n_users < MAX_USER_N) {
        // room for new user available
        unsigned int id_number = id_to_num(id);
        if (id_number <= MAX_ID_NUM) {
            // valid id received
            if (query(id) ==  MAX_USER_N) {
                // id not repeated
                unsigned int i = 0, end = false;
                while (i < MAX_USER_N && !end) {
                    if (database[i].free) {
                        end = true;
                        if (update_validator(database[i], validator)) {
                            // valid validator received. new user clear
                            database[i].free = false;
                            database[i].admin = false;
                            database[i].blocked = false;
                            database[i].id = id_number;

                            metadata.n_users++;
                            success = true;
                        }
                    }
                    i++;
                }
            }
        }
    }

    return success;
}


void u_remove(char * id)
{
    uint32_t index = query(id);
    if (index < MAX_USER_N && !(database[index].admin && metadata.admins == 1)) {
        database[index].free = true;
        if (database[index].blocked) {
            metadata.blocked--;
        }
        if (database[index].admin) {
            metadata.admins--;
        }
        metadata.n_users--;
    }
}

/*******************************************************************************
 * GETTERS AND SIMPLE CHECKS
 ******************************************************************************/

uint32_t u_n_users()
{
    return metadata.n_users;
}


uint32_t u_n_admins()
{
    return metadata.admins;
}


uint32_t u_n_blocked()
{
    return metadata.blocked;
}


bool u_is_admin(char * id)
{
    bool ans = false;
    uint32_t index = query(id);
    if (index < MAX_USER_N) {
        ans = database[index].admin;
    }
    return ans;
}


bool u_is_blocked(char * id)
{
    bool ans = false;
    uint32_t index = query(id);
    if (index < MAX_USER_N) {
        ans = database[index].blocked;
    }
    return ans;
}


bool u_exists(char * id)
{
    uint32_t index = query(id);
    return index < MAX_USER_N;
}


/*******************************************************************************
 * AUXILIARY FUNCTIONS
 ******************************************************************************/

uint32_t query(char * id)
{
    uint32_t index = MAX_USER_N;
    if (id != NULL && strlen(id) == ID_LEN) {
        unsigned int id_number = id_to_num(id);
        if (id_number <= MAX_ID_NUM) {
            // valid id number
            if (strcmp(id, last_queried_id) == 0 && last_queried_user < MAX_USER_N &&
                !database[last_queried_user].free
                && database[last_queried_user].id == id_number) {
                index = last_queried_user; // same result as last query
            } else {
                last_queried_user = index;
                // new query, or last query result no longer valid
                unsigned int i = 0;
                while (i < MAX_USER_N && index == MAX_USER_N) {
                    if (!database[i].free && database[i].id == id_number) {
                        // user found
                        index = last_queried_user = i;
                        strcpy(last_queried_id, id);
                    }
                    i++;
                }
            }
        }
    }

    return index;
}


unsigned int id_to_num(char * id)
{
    unsigned int number = MAX_ID_NUM + 1;

    if (id != NULL && strlen(id) == ID_LEN) {
        char * str_end; // convert id number
        long id_number = strtol(id, &str_end, 0);
        if (*str_end == '\0' && id_number <= MAX_ID_NUM) {
            // valid number
            number = id_number;
        }
    }

    return number;
}


bool check_validator(user_t u, id_validator_t v) {
    char * check =  u.validators[v.type].data;
    return v.data != NULL && strcmp(check, v.data) == 0;
}


bool update_validator(user_t u, id_validator_t v)
{
    bool success = false;
    if (v.data != NULL && u.validators[v.type].data != NULL) {
        unsigned int len = strlen(v.data);
        if (len >= min_lens[v.type] && len <= max_lens[v.type]) {
            strcpy(u.validators[v.type].data, v.data);
            success = true;
        }
    }
    return success;
}



