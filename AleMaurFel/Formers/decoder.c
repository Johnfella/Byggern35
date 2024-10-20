#define FOSC 4915200UL
#include <avr/io.h>
#include <util/delay.h>

int main(void){

    DDRA = 1;
    PORTA= 0;
    DDRC =(1<<PC0);
    DDRC =(1<<PC1);
    DDRC =(1<<PC2);
    DDRC =(1<<PC3);

while(1){

  
    PORTC =(0<<PC0);

    PORTC =(0<<PC1);

    PORTC =(1<<PC2);

    PORTC =(0<<PC3);

    _delay_ms(200);
    
    PORTC =(0<<PC2);
    PORTC=(1<<PC3);

    _delay_ms(200);

    }

return 0;
}