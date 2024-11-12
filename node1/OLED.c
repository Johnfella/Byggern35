#define F_CPU 4915200
#include <avr/io.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <string.h>
#include "OLED.h"
#include "font_8x8.h"

// Recommended functions: oled init(), oled reset(), oled home(), oled goto line(line), oled goto column(column),
// oled clear line(line), oled pos(row,column), oled print(char*);
// FOR COMMANDS SEE PAGE 27 IN "OLED Controller SSD1308.pdf"

volatile oled_position_t oled_position;

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
    oled_home();
}

void oled_goto_line(uint8_t line) {
    uint8_t command = 0xB0 + line;
    oled_position.row = line;
    write_c(command);
}

void oled_goto_column(uint8_t column) {
	  write_c(0x00 + (column % 16));
	  write_c(0x10 + (column / 16));
      oled_position.col = column;
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

    for (uint8_t j = 0; j <= 128; j++) {
            write_d(fill);
            oled_pos(line,j);
            _delay_us(10);
    }
    oled_pos(line,0);
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

void oled_out_of_bounds() {
	if (oled_position.col > 127) {
		oled_position.col -= 128;
		oled_position.row += 1;
		if (oled_position.row > 7) {
			oled_position.row = 0;
		}
	}
}


void oled_print(char c) {
    oled_pos(oled_position.row,oled_position.col);
    uint8_t printChar = c-32;
	
	for (int i=0; i < 8; i++) {
		write_d(pgm_read_word(&font_8x8[printChar][i]));
		oled_position.col += 8;
		oled_out_of_bounds();
	}
    
}

void oled_clear_screen() {
    for (uint8_t i = 0; i < 8; i++) {  // Assuming 8 lines for a 64-pixel high display
        oled_goto_line(i);
        for (uint8_t j = 0; j < 128; j++) {  // Assuming 128 columns for a 128-pixel wide display
            write_d(0x00);  // Clear each pixel
        }
    }
    oled_home();  // Reset cursor to home position
}


void oled_print_string(char *str) {
    oled_pos(oled_position.row,oled_position.col);
    while (*str) {
        uint8_t printChar = *str - 32;  // Adjust for the ASCII offset
        for (int i = 0; i < 8; i++) {  // Assuming each character is 8 bytes wide
            write_d(pgm_read_byte(&font_8x8[printChar][i]));
        }
        oled_position.col += 8;  // Move to the next character position
        if (oled_position.col > 128) {  // If at the end of the line, move to next line
            oled_position.col = 0;
            oled_position.row++;
        }
        oled_out_of_bounds();
        str++;
    }
    //FILE oled_stdout = FDEV_SETUP_STREAM(oled_print, NULL, _FDEV_SETUP_WRITE);
    //stdout = &oled_stdout;  // Set stdout to the OLED stream
}

void display_receiving_byte(uint32_t byte) {
    
    char buffer[32];  // Create a buffer with enough space for your formatted string
    char byte_char = (char)byte;
    sprintf(buffer, "Receiving: %02X", byte_char);  // Format the string; %02X for hexadecimal display
    oled_print_string(buffer);  // Pass the formatted string to oled_print_string
}


