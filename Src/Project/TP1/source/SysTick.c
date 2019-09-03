/*
 * @file SysTick.c
 * @brief SysTick driver implementation
 * @author 22.99 Group 1: Alvarez, Gonzalez, Parra, Reina
 */
#include "SysTick.h"
#include "MK64F12.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "core_cm4.h"


#define FCLK	120000000U // clock frequency, Hz

#if FCLK % SYSTICK_ISR_FREQUENCY_HZ != 0
#warning SYSTICK cannot implement this exact frequency. Using floor(FCLK/SYSTICK_ISR_FREQUENCY_HZ) instead.
#elif SYSTICK_ISR_FREQUENCY_HZ <= 0
#error SYSTICK frequency must be positive
#endif /* FCLK % SYSTICK_ISR_FREQUENCY_HZ != 0 */


systick_callback_t callback;

typedef struct {
    systick_callback_t func;
    unsigned int counter;
    unsigned int reload;
    bool enabled;
} st_cb_data_t;

static st_cb_data_t st_callbacks[MAX_N_ST_CALLBACKS];

void SysTick_Handler(void);


void systick_init ()
{
	static bool initialized = false;

	if(initialized) return;

	NVIC_EnableIRQ(SysTick_IRQn);
	SysTick->CTRL = 0x00; 								// enable systick interrupts
	SysTick->LOAD = FCLK/SYSTICK_ISR_FREQUENCY_HZ - 1; 	// load value = pulses per period - 1
	SysTick->VAL=0x00;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk| SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;

	unsigned int i = 0;
	for (i = 0; i < MAX_N_ST_CALLBACKS; i++) {
	    st_callbacks[i].func = NULL;
	}
	initialized = true;
}

void SysTick_Handler(void) // DO NOT CHANGE THE NAME, overrides core_cm4.h weak definition
{
	// for SysTick, clearing the interrupt flag is not necessary
	// it is not an omission!
    unsigned int i;
    for (i = 0; i < MAX_N_ST_CALLBACKS; i++) {
        if (st_callbacks[i].func != NULL && st_callbacks[i].enabled) {
            if (st_callbacks[i].counter == st_callbacks[i].reload) {
            	st_callbacks[i].func();
                st_callbacks[i].counter = 0;
            }
            st_callbacks[i].counter++;
        }
    }
}



unsigned int systick_add_callback(systick_callback_t cb, unsigned int reload)
{
    unsigned int id = MAX_N_ST_CALLBACKS;
    if (cb != NULL) {
        unsigned int i = 0;
        while (i < MAX_N_ST_CALLBACKS && id == MAX_N_ST_CALLBACKS) {
            if (st_callbacks[i].func == NULL) {
                st_callbacks[i].func = cb;
                st_callbacks[i].enabled = true;
                st_callbacks[i].counter = 0;
                st_callbacks[i].reload = reload;

                id = i;
            }
            i++;
        }
    }

    return id;
}

void systick_enable_callback(unsigned int id) {
    if (id < MAX_N_ST_CALLBACKS) {
        st_callbacks[id].enabled = true;
    }
}

void systick_disable_callback(unsigned int id) {
    if (id < MAX_N_ST_CALLBACKS) {
        st_callbacks[id].enabled = false;
    }
}
