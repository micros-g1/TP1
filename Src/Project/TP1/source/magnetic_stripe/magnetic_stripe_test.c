//
// Created by Rocío Parra on 8/31/2019.
//

#include "magnetic_stripe.h"
#include <string.h>
#include <stdio.h>

#define MS_TEST 0

#if MS_TEST == 1
int main(void)
{
    char last_queried_id [5];
    memset(last_queried_id, '-', sizeof(last_queried_id)-1);
    last_queried_id[5-1];
    printf(last_queried_id);

    //    char s [] = "11100101111010000011010";
//    unsigned int i;
//
//    for (i = 0; i< sizeof(s); i++) {
//        process_bit_ss(s[i] - '0');
//    }
}
#endif // MS_TEST == 1
