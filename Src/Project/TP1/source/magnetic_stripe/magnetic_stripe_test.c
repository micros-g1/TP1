//
// Created by Rocío Parra on 8/31/2019.
//

#include "magnetic_stripe.h"


int main(void)
{
    char s [] = "11100101111010000011010";
    unsigned int i;

    for (i = 0; i< sizeof(s); i++) {
        process_bit_ss(s[i] - '0');
    }
}

