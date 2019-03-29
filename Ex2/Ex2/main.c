#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL
#include "util/delay.h"
#define SW PD2
#define LED0 PB0
#define LED1 PB1
#define LED2 PB5
void fSleep(void) {
	// Enter sleep mode -------------
	cli(); // Global intr disabled
	SMCR |= (1<<SM1); // Set Power down mode
	SMCR |= (1<<SE); // Enable sleep
	sei(); // Global intr enabled
	asm("sleep \n"); // Sleep now (inline sleep instr)
	// Now wake up -----------------
	SMCR &= ~(1<<SE); // Disable sleep
}
void fBlink(void) { // Blinking LEDs
	uint8_t i;
	for (i=0;i<5;i++) {
		PORTB ^= (1<<LED1)|(1<<LED0);
		_delay_ms(500);
	}
}
int main(void) {
	// PORTs -----------------------
	DDRB |= (1<<LED2)|(1<<LED1)|(1<<LED0);
	PORTB |= (1<<LED1);
	PORTD |= (1<<SW);
	// INT0 ------------------------
	EICRA |= (1<<ISC01); // Falling edge for INT0
	EIMSK |= (1<<INT0); // Enable INT0
	while (1) {
		fBlink(); // Blinking LED1 & LED0
		fSleep(); // Sleeping
	}
}
ISR(INT0_vect) { // INT0's ISR
	PORTB ^= (1<<LED2);
}