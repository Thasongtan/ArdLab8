.include "m328pdef.inc"
.device atmega328p
.equ LED = PB5
.equ SW = PD2
.org 0x0000 ; Reset intr vector
jmp Main
.org 0x0002 ; INT0 intr vector
jmp Int0_ISR
;-------------------------------------
Main:
sbi ddrb,LED ; PB5 is output (LED)
sbi portb,LED ; LED is on
cbi ddrd,SW ; PD2 is input (SW)
sbi portd,SW ; SW is pulled up
;- System clock prescaler
ldi r16,0b10000000 ;
sts CLKPR,r16 ; Step 1
ldi r16,0b00000100 ;
sts CLKPR,r16 ; Step 2
;- INT0 enabled, active when low
sbi EIMSK,INT0
;- Enter sleep mode -----------
ldi r16,0x00
out SMCR,r16 ;
ori r16,(1<<SE)
out SMCR,r16 ; Enable sleep
sei ; Global intr enabled
sleep ; Sleep now
andi r16,~(1<<SE) ; Now wake up
out SMCR,r16 ; Disable sleep
cli ; Global intr disabled
Loop:
rcall TogLED ; Call TogLed
rcall Delay ; Call Delay
rjmp Loop
;- Subroutine TogLED -----------------
TogLED:
in r16, PORTB ; Read portB
ldi r17,(1<<LED) ; To toggle LED
eor r16,r17 ; To toggle LED
out PORTB,r16 ; Toggle LED
ret
;- Subroutine Delay ------------------
Delay:
ldi r20, 50
ldi r21, 255
delay_dec:
dec r21
brne delay_dec
dec r20
brne delay_dec
ret
;- INT0's ISR ------------------------
Int0_ISR:
reti