#define F_CPU 4915200
#include <stdio.h>
#include "UART.h"
#include "SRAM_test.h"

#define BAUD 9600
#define MYUBRR (F_CPU/16/BAUD-1)

void main(void){
    USART_Init(MYUBRR);
    SRAM_init();
    SRAM_test();

    while(1){
        //printf
        //delay_ms(100);
    }
}