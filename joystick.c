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


multifunctionBoard joystick;


void adc_read(multifunctionBoard * p)
{      
    volatile char *ext_adc = (char *) 0x1400; 
  
    for(int i=1; i<=4; i++){
    ext_adc[0x00]=0x01;
    }
    

    for(int i=1; i<=4; i++){
    uint8_t data = ext_adc[0x00];
    
        switch(i){
            case 1: 
                joystick.y = data-joystick.Offset_y;
                printf("Data %s: %02X\n", "y",joystick.y);
                
                break;
            case 2: 
                joystick.x =data-joystick.Offset_x;
                printf("Data %s: %02X\n", "x",joystick.x); 
                break;

            case 3: 
                printf("Data %s: %d\n", "sliderR",data);
                break;
            case 4: 
                printf("Data %s: %02X\n", "sliderL",data);
                break;
        }

    }

}

void joystickPosition(multifunctionBoard * p) {
    uint8_t xValue = joystick.x;
    uint8_t yValue = joystick.y;
    
    joystick.percent_x=round(100*((xValue)-127)/127);
 
    joystick.percent_y=round(100*((yValue)-127)/127);

    printf("%d",joystick.percent_x);
    printf("%d",joystick.percent_y);
}



void calibration(multifunctionBoard * p){

    if(joystick.x >127){
        joystick.Offset_x = joystick.x - 127;
    }  
    
    if(joystick.y >127){
        joystick.Offset_y = joystick.y - 127;
    }  
    
    printf("Offsetx= %d\n", joystick.Offset_x);
    printf("Offsety= %d\n", joystick.Offset_y);
    
}

