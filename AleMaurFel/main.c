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
    USART_Init(MYUBRR);
    SRAM_init();
   // SRAM_test();
   //external_clk();
   // oled_init_ports();
    oled_init();

    while (1) {    
        write_c(0xaf); // display on
        //_delay_ms(500);
        write_c(0xae);
        //_delay_ms(500);
    }
       
}