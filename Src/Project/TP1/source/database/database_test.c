//
// Created by Rocío Parra on 9/1/2019.
//


#define DB_TEST 1
#if DB_TEST == 1

#include <stdio.h>
#include "database.h"

int main (void) {
    u_init();

    char * u1 = "12345678";
    id_validator_t v;
    v.type = FIVE_DIGIT_PIN;
    v.data = "0010";
    if (u_add(u1, v)) {
        printf("agrego");
        u_block(u1);
        if (u_is_blocked(u1)) {
            u_unblock(u1);
            if (!u_is_blocked(u1)) {
                u_make_admin(u1);
                if (u_is_admin(u1)) {
                    u_remove_as_admin("00000000");
                    if (u_n_admins() == 1 && u_exists("00000000")) {
                        v.data = "1234";
                        u_change_validator(u1, v);
                        if (u_validate(u1, v)) {
                            v.data = "0000";
                            if (!u_validate(u1, v)) {
                                printf("si");
                            }
                        }
                    }
                }
            }
        }
    }
}



#endif // DB_TEST == 1