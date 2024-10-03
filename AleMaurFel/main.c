#define F_CPU 4915200
#include <stdio.h>
#include <util/delay.h>
#include "UART.h"
#include "SRAM_test.h"
#include "joystick.h"

#define BAUD 9600
#define MYUBRR (F_CPU/16/BAUD-1)

void main(void){
    USART_Init(MYUBRR);
    SRAM_init();
    adc_init();
    SRAM_test();

    while(1){
        //uint16_t x_axis = adc_read(0);
        //uint16_t y_axis = adc_read(1);
        //printf(x_axis);
        _delay_ms(100);
    }
}