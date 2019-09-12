//
// Created by Rocío Parra on 8/31/2019.
//



#ifdef MT_DEBUG

#include <string.h>
#include <stdio.h>
#include "magtek_driver.h"
#include "magnetic_stripe.h"


void senddata(char * data, unsigned int n);

int main(void)
{
    ms_init();
    mt_periodic();
    enablepin = true;
    mt_enable_callback();
    mt_periodic();
    enablepin = false;
    mt_enable_callback();
    mt_periodic();
    char word[] = "0011010111110000110101";
    senddata(word, sizeof(word));
}

void senddata(char * data, unsigned int n)
{
    unsigned int i;
    for (i = 0; i < n; i++) {
        datapin = (data[i] == '1');
        mt_clock_callback();
        mt_periodic();
    }
}



//int main(void)
//{
//    char last_queried_id [5];
//    memset(last_queried_id, '-', sizeof(last_queried_id)-1);
//    last_queried_id[5-1];
//    printf(last_queried_id);
//
//    //    char s [] = "11100101111010000011010";
////    unsigned int i;
////
////    for (i = 0; i< sizeof(s); i++) {
////        process_bit_ss(s[i] - '0');
////    }
//}
#endif // MS_TEST == 1
