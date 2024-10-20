#define F_CPU 4915200
#include <stdio.h>
#include <util/delay.h>
#include "UART.h"
#include "SRAM_test.h"
#include "joystick.h"
#include "OLED.h"

#define BAUD 9600
#define MYUBRR (F_CPU/16/BAUD-1)

void main(void) {
    USART_init(MYUBRR);
    SRAM_init();
    external_clk();
    oled_init();
    SRAM_test();


    while (1) { 
        oled_reset();
        _delay_ms(400);
        oled_set();
        _delay_ms(400);
    }
       
}