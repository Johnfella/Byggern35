//X-axis == orange cable, AIN1. Y-axis == red cable, AIN0
//Right slider == red cable AIN3. Left slider == blue cable AIN2
#define F_CPU 4915200
#include <avr/io.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include "macros.h"
#include "UART.h"
#include "joystick.h"


void external_clk(void) {
    set_bit(DDRD,PD5);
    set_bit(TCCR1B,WGM12);
    clear_bit(TCCR1A,WGM10);
    clear_bit(TCCR1A,WGM11);
    set_bit(TCCR1A,COM1A0);
    OCR1A=0;
    set_bit(TCCR1B,CS10);

}

void adc_init(void) {
    
}

void adc_read(void)
{
    MCUCR |= (1<<SRE);
    SFIOR = (1<<XMM2);

    volatile char *ext_adc = (char *) 0x1400; 
    volatile char data = ext_adc[0x00];
    printf("Data: %c\n", data);

}

