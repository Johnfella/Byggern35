
//NEW CODE

#define F_CPU 4915200
#include <avr/io.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <math.h>
#include"mcp2515.h"
#include"SPI.h"

#define DDR_SPI DDRB
#define DD_SS DDB4
#define DD_MOSI DDB5
#define DD_MISO DDB6
#define DD_SCK DDB7


void spi_set_ss() {
	PORTB |= (1 << DD_SS);
}


void spi_clear_ss() {
	PORTB &= ~(1 << DD_SS);
}

void test_mosi_clear() {
	PORTB &= ~(1 << DD_MOSI);
}


void spi_master_init(void) {
	
	DDR_SPI = (1<<DD_MOSI)|(1<<DD_SCK)|(1<<DD_SS);
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);

	spi_set_ss();
}

void spi_write(char cData) {
	SPDR = cData;
	while(!(SPSR & (1<<SPIF))) 
	;
}


uint8_t spi_read() {
	SPDR = 0xFF;
	while(!(SPSR & (1<<SPIF))) 
	;
	return SPDR;
}

//OLD CODE


/*
#define F_CPU 4915200
#include <stdio.h>
#include <util/delay.h>
#include <avr/io.h>
#include "UART.h"
#include "SRAM_test.h"
#include "joystick.h"
#include "OLED.h"
#include "SPI.h"
// ADD TO MAKEFILE ! SOURCE_FILES := $(wildcard source_files/*.c)

void SPI_MasterInit(void)
{
DDRB = (1<<DDB4)|(1<<DDB5)|(1<<DDB7);
SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
}

void SPI_MasterTransmit(char cData)
{
SPDR = cData;
while(!(SPSR & (1<<SPIF)));
}

void SPI_SlaveInit(void)
{
DDRB = (1<<DDB6);
SPCR = (1<<SPE);
}
char SPI_SlaveReceive(void)
{
while(!(SPSR & (1<<SPIF)));
return SPDR;
}*/