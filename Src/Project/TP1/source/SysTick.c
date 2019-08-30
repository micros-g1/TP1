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


typedef void (*systick_callback_t)(void);
systick_callback_t callback;



void SysTick_Handler(void);

bool SysTick_Init (void (*funcallback)(void))
{
	NVIC_EnableIRQ(SysTick_IRQn);
	if (callback == NULL) {
		SysTick->CTRL = 0x00; 								// enable systick interrupts
		SysTick->LOAD = FCLK/SYSTICK_ISR_FREQUENCY_HZ - 1; 	// load value = pulses per period - 1
		SysTick->VAL=0x00;
		SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk| SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;
		callback = funcallback;								// set ISR

		return true;
	}
	else {
		return false;
	}
}

void SysTick_Handler(void)
{
	// for SysTick, clearing the interrupt flag is not necessary
	// it is not an omission!
	callback();
}
