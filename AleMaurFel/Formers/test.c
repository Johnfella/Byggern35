#define FOSC 4915200UL
#include <avr/io.h>
#include <util/delay.h>

int main(void){

    DDRA |=(1<<PA0);
    DDRE |=(1<<PE1);
    
    PORTA |=(1<<PA0);
    PORTE |=(1<<PE1);

    while(1){

    }

return 0;
}
    