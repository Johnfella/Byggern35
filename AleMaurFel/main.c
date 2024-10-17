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


    

    while(1){
        adc_read();
        position Position = joystickPosition();

    switch (Position) {
        case LEFT: printf("\n LEFT \n"); break;
        case RIGHT: printf("\n RIGHT \n"); break;
        case NEUTRAL: printf("\n NEUTRAL \n"); break;
        case UP: printf("\n UP \n"); break;
        case DOWN: printf("\n DOWN \n"); break;
    }
        _delay_ms(100);
    }
}