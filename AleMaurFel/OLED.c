#define F_CPU 4915200
#include <avr/io.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <math.h>
#include "OLED.h"

// Recommended functions: oled init(), oled reset(), oled home(), oled goto line(line), oled goto column(column),
// oled clear line(line), oled pos(row,column), oled print(char*);


void write_c(uint8_t command) {
volatile char *ext_oledC = (char *) 0x1000;
ext_oledC[0x00]=command;
}

void write_d(uint8_t data) {
volatile char *ext_oledD = (char *) 0x1200;
ext_oledD[0x00]=data;
}



void oled_init() {
    write_c(0xae); // display off
    write_c(0xa1); //segment remap
    write_c(0xda); //common pads hardware: alternative
    write_c(0x12);
    write_c(0xc8); //common output scan direction:com63~com0
    write_c(0xa8); //multiplex ration mode:63
    write_c(0x3f);
    write_c(0xd5); //display divide ratio/osc. freq. mode
    write_c(0x80);
    write_c(0x81); //contrast control
    write_c(0x50);
    write_c(0xd9); //set pre-charge period
    write_c(0x21);
    write_c(0x20); //Set Memory Addressing Mode
    write_c(0x02);
    write_c(0xdb); //VCOM deselect level mode
    write_c(0x30);
    write_c(0xad); //master configuration
    write_c(0x00);
    write_c(0xa4); //out follows RAM content
    write_c(0xa6); //set normal display
    write_c(0xaf); // display on
}


