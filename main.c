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
    external_clk();
    adc_read();
    calibration();
    _delay_ms(2000);


    while(1){

    adc_read();
    joystickPosition();
    _delay_ms(2000);
    }
}