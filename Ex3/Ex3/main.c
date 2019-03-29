#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL
#include "util/delay.h"
#define LED0 PB0
#define LED1 PB1
#define WDT_RESET asm("WDR \n") // Inline assembly instr
void fWDTInit(void) {
	cli(); // Global intr disabled
	// Reset watchdog timer
	WDT_RESET;
	// Start timed sequence
	WDTCSR |= (1<<WDCE) | (1<<WDE);
	// Set WDT prescaler
	WDTCSR = (1<<WDE) | (0<<WDP3) | (1<<WDP2) | (1<<WDP0);
}
int main(void) {
	uint8_t i;
	DDRB |= (1<<LED1)|(1<<LED0);
	PORTB |= (1<<LED0);
	fWDTInit();
	while (1) {
		PORTB ^= (1<<LED1)|(1<<LED0); // Toggling LEDs
		_delay_ms(126);
		WDT_RESET; // Reset watchdog timer
		_delay_ms(126);
		WDT_RESET; // Reset watchdog timer
		_delay_ms(126);
		WDT_RESET; // Reset watchdog timer
		_delay_ms(126);
		WDT_RESET; // Reset watchdog timer
	}
}