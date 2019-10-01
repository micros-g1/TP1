//
// Created by Rocío Parra on 9/1/2019.
//


#define DB_TEST 0
#if DB_TEST == 1

#include <stdio.h>
#include "database.h"

int main (void) {
    u_init();

    char * p1 = "0010";
    id_t u1;
    u1.data = "12345678";
    u1.type = EIGHT_DIGIT_PIN;

    if (u_add(u1.type, u1.data, p1)) {
        printf("agrego");
        u_block(u1.type, u1.data);
        if (u_is_blocked(u1.type, u1.data)) {
            u_unblock(u1.type, u1.data);
            if (!u_is_blocked(u1.type, u1.data)) {
                u_make_admin(u1.type, u1.data);
                if (u_is_admin(u1.type, u1.data)) {
                    u_remove_as_admin(EIGHT_DIGIT_PIN, "00000000");
                    if (u_n_admins() == 1 && u_exists(EIGHT_DIGIT_PIN, "00000000")) {
                        p1 = "1234";
                        u_change_password(u1.type, u1.data, p1);
                        if (u_validate(u1.type, u1.data, p1)) {
                            p1 = "0000";
                            if (!u_validate(u1.type, u1.data, p1)) {
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
