/***************************************************************************//**
  @file     database.c
  @brief    User database for security system
  @author   22.99 2019 Group 1: Alvarez, Gonzalez, Parra, Reina
 ******************************************************************************/


/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include <string.h>
#include <stdlib.h>
#include <Database/database.h>


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

// reduce need for iteration when quering multiple times for the same user
static unsigned char last_queried_user;

static char cards[MAX_USER_N][CARD_LEN+1]; // leave one space for terminator
static char pins[MAX_USER_N][PIN_LEN+1];


/*******************************************************************************
 * FUNCTION PROTOTYPES WITH FILE SCOPE
 ******************************************************************************/

/**
 * @brief Look for id in database. Updates last query.
 * @param id: user id, 0 terminated
 * @return Index for user in database. MAX_USER_N if not found
 */
uint32_t query(id_type_t id_type, char * id);

/**
 * @brief Validate access. Same functionality as "u_validate"
 * @param u: user
 * @param password: access code to check
 * @return true if user is not blocked and validator matches, false otherwise
 */
bool check_password(user_t * u, char * password);

/**
 * @brief Change validator for user. Same functionality as "u_change_validator".
 * @param u: user
 * @param password: new validator
 * @return Exit success
 */
bool update_password(user_t * u, char * password);

/**
 * @brief Converts id string to id number
 * @param id: user id, 0 terminated
 * @return id number. MAX_ID_NUMBER+1 if input not valid
 */
bool is_id_valid(id_type_t id_type, char * id);


/*******************************************************************************
 * FUNCTIONS WITH GLOBAL SCOPE
 ******************************************************************************/

void u_init()
{
    static bool is_init = false;
    if (is_init)
        return;
    is_init = true;

    metadata.n_users = 1; // initialize with one default admin user
    metadata.blocked =  0;
    metadata.admins =   1;

    last_queried_user = 0;

    unsigned int i, j;
    for(i = 0; i < MAX_USER_N; i++) {
        database[i].free = true;
        database[i].blocked = false;
        database[i].admin = false;
        database[i].index = i;
        database[i].n_tries = 0;

        for (j = 0; j < N_IDS; j++) {
            database[i].ids[j].type = j;
        }
        database[i].ids[EIGHT_DIGIT_PIN].data = pins[i];
        memset(pins[i], 0, sizeof(pins[i]));
        database[i].ids[MAGNETIC_CARD].data = cards[i];
        memset(cards[i], 0, sizeof(cards[i]));
    }

    strcpy(database[0].ids[EIGHT_DIGIT_PIN].data, "00000000");  // default admin data
    database[0].admin = true;
    database[0].free = false;
    strcpy(database[0].password, "00000");

}


bool u_validate(id_type_t id_type, char * id, char * password)
{
    bool valid = false;
    uint32_t index = query(id_type, id);

    if (index < MAX_USER_N && !database[index].blocked) {
        valid = check_password(&database[index], password);

        if (valid) {
            database[index].n_tries = 0;
        }
        else {
            database[index].n_tries++;
        }
    }

    return valid;
}


void u_block(id_type_t id_type, char * id)
{
    uint32_t index = query(id_type, id);

    if (index < MAX_USER_N && !database[index].blocked) {
        database[index].blocked = true;
        metadata.blocked++;
    }
}


void u_unblock(id_type_t id_type, char * id)
{
    uint32_t index = query(id_type, id);

    if (index < MAX_USER_N && database[index].blocked) {
        database[index].blocked = false;
        metadata.blocked--;
        database[index].n_tries = 0;
    }
}


void u_make_admin(id_type_t id_type, char * id)
{
    uint32_t index = query(id_type, id);

    if (index < MAX_USER_N && !database[index].admin) {
        database[index].admin = true;
        metadata.admins++;
    }
}


bool u_remove_as_admin(id_type_t id_type, char * id)
{
    bool success = false;

    if (metadata.admins > 1) {
        uint32_t index = query(id_type, id);

        if (index < MAX_USER_N && database[index].admin) {
            database[index].admin = false;
            metadata.admins--;
            success = true;
        }
    }

    return success;
}


bool u_change_password(id_type_t id_type, char * id, char * password)
{
    bool success = false;
    uint32_t index = query(id_type, id);
    if (index < MAX_USER_N) {
        success = update_password(&database[index], password);
    }

    return success;
}


bool u_add(id_type_t id_type, char * id, char * password)
{
    bool success = false;
    if (metadata.n_users < MAX_USER_N) {
        // room for new user available

        if (is_id_valid(id_type, id)) {
            // valid id received

            if (query(id_type, id) ==  MAX_USER_N) {
                // id not repeated

                unsigned int i = 0;
                bool end = false;
                while (i < MAX_USER_N && !end) {
                    if (database[i].free) {
                        end = true; // free spot found!
                        if (update_password(&database[i], password)) {
                            // valid password received. new user clear
                            database[i].free = false;
                            database[i].admin = false;
                            database[i].blocked = false;
                            database[i].n_tries = 0;
                            strcpy(database[i].ids[id_type].data, id);


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

bool u_new_id(id_type_t id_type, char * id, id_type_t new_id_type, char * new_id)
{
    bool valid = false;
    uint32_t index = query(id_type, id);
    if (index < MAX_USER_N && is_id_valid(new_id_type, new_id)) {
        strcpy(database[index].ids[new_id_type].data, new_id);
        valid = true;
    }

    return valid;
}



void u_remove(id_type_t id_type, char * id)
{
    uint32_t index = query(id_type, id);
    if (index < MAX_USER_N && !(database[index].admin && metadata.admins == 1)) { // cant remove last remaining admin!
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


void u_reset_n_tries(id_type_t id_type, char * id)
{
    uint32_t index = query(id_type, id);
    if (index < MAX_USER_N) {
        database[index].n_tries = 0;
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


bool u_is_admin(id_type_t id_type, char * id)
{
    bool ans = false;
    uint32_t index = query(id_type, id);
    if (index < MAX_USER_N) {
        ans = database[index].admin;
    }
    return ans;
}


bool u_is_blocked(id_type_t id_type, char * id)
{
    bool ans = false;
    uint32_t index = query(id_type, id);
    if (index < MAX_USER_N) {
        ans = database[index].blocked;
    }
    return ans;
}


bool u_exists(id_type_t id_type, char * id)
{
    uint32_t index = query(id_type, id);
    return index < MAX_USER_N;
}


uint32_t u_get_n_tries(id_type_t id_type, char * id)
{
    uint32_t ans = UINT32_MAX;
    uint32_t index = query(id_type, id);
    if (index < MAX_USER_N) {
        ans = database[index].n_tries;
    }
    return ans;
}

/*******************************************************************************
 * AUXILIARY FUNCTIONS
 ******************************************************************************/

uint32_t query(id_type_t id_type, char * id)
{
    uint32_t index = MAX_USER_N;
    if (is_id_valid(id_type, id)) {
        // pin is right lenght and consists solely of numbers

        // first we check if this is last query's result is still valid
        if (last_queried_user < MAX_USER_N  // was last query valid?
                && !database[last_queried_user].free  // is the user still there? (maybe last query was remove)
                && strcmp(id, database[last_queried_user].ids[id_type].data) == 0 ) { // is this the same query?
            index = last_queried_user; // same result as last query
        } else {
            // new query, or last query result no longer valid
            last_queried_user = MAX_USER_N; // in case i dont find the user: no last queried user

            unsigned int i = 0;
            while (i < MAX_USER_N && index == MAX_USER_N) {
                if (!database[i].free && strcmp(database[i].ids[id_type].data, id) == 0) {
                    // user found
                    index = last_queried_user = i;
                }
                i++;
            }
        }
    }

    return index;
}


bool is_id_valid(id_type_t id_type, char * id)
{
    bool valid = false;

    if (id != NULL) {
        switch (id_type) {
            case EIGHT_DIGIT_PIN: {
                if (strlen(id) == PIN_LEN) {
                    unsigned int i = 0;
                    valid = true;
                    while (i < EIGHT_DIGIT_PIN && valid) {
                    	i++;
                    	valid = (id[i] >= '0' && id[i] <='9') ? 1 : 0;
                    }
                }
            } break;

            case MAGNETIC_CARD: {
                valid = (strlen(id) <= CARD_LEN);
            } break;

            default: {
            } break;

        }
    }

    return valid;
}


bool check_password(user_t * u, char * password)
{
    return password != NULL && strcmp(password, u->password) == 0;
}


bool update_password(user_t * u, char * password)
{
    bool success = false;
    if (password != NULL) {
        unsigned int len = strlen(password);
        if (len >= PASSWORD_MIN_LEN && len <= PASSWORD_MAX_LEN) {
            unsigned int i = 0;
            success = true;
            while (i < len && success) {
				success = (password[i] >= '0' && password[i] <='9') ? 1 : 0;
				i++;
            }

            if (success) {
                strcpy(u->password, password);
            }
        }
    }
    return success;
}


void u_get_pin_from_card(char * card, char * pin)
{
	if (card != NULL) {
		unsigned int index = query(MAGNETIC_CARD, card);
		if (index < MAX_USER_N) {
			strcpy(pin, database[index].ids[EIGHT_DIGIT_PIN].data);
		}
	}
}



