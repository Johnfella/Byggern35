// Program to Generate waveform using AVR Microcontroller (Atmega16) Timers
#define FOSC 4915200UL
#include <avr/io.h>
#include <util/delay.h>

int main(void){

    DDRB |=(1<<PB0);

    while(1){
    PORTB ^=(1<<PB0);
    _delay_ms(200);
    
    }

return 0;
}
    