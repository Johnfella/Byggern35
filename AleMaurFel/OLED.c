#define F_CPU 4915200
#include <avr/io.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>
#include "OLED.h"

// Recommended functions: oled init(), oled reset(), oled home(), oled goto line(line), oled goto column(column),
// oled clear line(line), oled pos(row,column), oled print(char*);
// FOR COMMANDS SEE PAGE 27 IN "OLED Controller SSD1308.pdf"


void write_c(uint8_t command) {
volatile char *ext_oledC = (char *) 0x1000;
ext_oledC[0]=command;
}

void write_d(uint8_t data) {
volatile char *ext_oledD = (char *) 0x1200;
ext_oledD[0]=data;
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
    write_c(0x41);
    write_c(0xd9); //set pre-charge period
    write_c(0x21);
    write_c(0x20); //Set Memory Addressing Mode
    write_c(0x10);
    write_c(0xdb); //VCOM deselect level mode
    write_c(0x30);
    write_c(0xad); //master configuration
    write_c(0x00);
    write_c(0xa4); //out follows RAM content
    write_c(0xa6); //set normal display
    write_c(0xaf); // display on
}

void oled_goto_line(uint8_t line) {
    uint8_t command = 0xB0 + line;
    write_c(command);
}

void oled_goto_column(uint8_t column) {
	  write_c(0x00 + (column % 16));
	  write_c(0x10 + (column / 16));
}

void oled_pos(uint8_t row, uint8_t column) {
    oled_goto_line(row);
    oled_goto_column(column);
}

void oled_home() {
    oled_pos(0,0);
}

void oled_fill_line(uint8_t line, uint8_t fill) {
    oled_pos(line,0);

    for (uint8_t j = 0; j < 128; j++) {
            write_d(fill);
            oled_pos(line,j);
            _delay_us(10);
    }
}

void oled_clear_line(uint8_t line) {
    oled_fill_line(line, 0x00);
}

void oled_reset() {
    oled_home();
    for (uint8_t i = 0; i < 9; i++) {
        oled_clear_line(i);
    }
    oled_home();
}

void oled_set() {
    oled_home();
    for (uint8_t i = 0; i < 9; i++) {
        oled_fill_line(i, 0xFF);
    }
    oled_home();
}
