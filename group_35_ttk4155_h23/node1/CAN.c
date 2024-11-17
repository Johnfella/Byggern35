
#define F_CPU 4915200
#include <stdio.h>
#include <util/delay.h>
#include <avr/io.h>
#include"SPI.h"
#include"CAN.h"
#include"mcp2515.h"
#include "joystick.h"
#include "macros.h"


void mcp_reset() {
	spi_clear_ss(); 
	spi_write(MCP_RESET); 
	spi_set_ss(); 
}

uint8_t mcp_read(uint8_t address) {
	spi_clear_ss();
	spi_write(MCP_READ);
	spi_write(address); 
	uint8_t data = spi_read();
	spi_set_ss();

	return data;
}


void mcp_write(uint8_t address, uint8_t data) {
	spi_clear_ss();
	spi_write(MCP_WRITE);
	spi_write(address); 
	spi_write(data);
	spi_set_ss();
}



char mcp_read_status() {
	spi_clear_ss();
	spi_write(MCP_READ_STATUS);
	char data = spi_read();
	spi_set_ss();

	return data;
}

void mcp_bit_modify(uint8_t address, uint8_t mask, uint8_t data) {
	spi_clear_ss();
	spi_write(MCP_BITMOD);
	spi_write(address); 
	spi_write(mask); 
	spi_write(data); 
	spi_set_ss();
}

void mcp_init() {
	spi_master_init();
	mcp_reset(); 

	_delay_ms(1); 

	
	uint8_t value = mcp_read(MCP_CANSTAT); 
	if ((value & MODE_MASK) != MODE_CONFIG) {
	printf("MCP251 CANSTAT: %x \r\n", value);
	}
}


void mcp_request_to_send(int buffer_number) {
	
	
	spi_clear_ss();
	buffer_number = buffer_number % 3; // Mapper buffernummer til 0, 1, 2
	char data = MCP_RTS_TX0;
	if (buffer_number == 0) {
		data = MCP_RTS_TX0;
	} else if (buffer_number == 1) {
		data = MCP_RTS_TX1;
	} else if (buffer_number == 2) {
		data = MCP_RTS_TX2;
	}
	spi_write(data);
	spi_set_ss();
	mcp_clear_tx_buffer(buffer_number);
	
}


void mcp_set_mode(uint8_t mode) {
	mcp_bit_modify(MCP_TXB0CTRL+MCP_CANCTRL, MODE_MASK, mode);
}




int can_bus_read(CAN_Message *message) {
    uint8_t canintf = mcp_read(MCP_CANINTF);  // Read CAN Interrupt Flag register

    // Check RX Buffer 0
    if (canintf & MCP_RX0IF) {
        // Read the message ID
        message->id = (mcp_read(MCP_RXB0SIDH) << 3) | (mcp_read(MCP_RXB0SIDL) >> 5);

        uint8_t dlc = mcp_read(MCP_RXB0DLC);
		message->length = dlc & 0x0F;
        // Read the message data
        for (int i = 0; i < message->length; i++) {
            message->data[i] = mcp_read(MCP_RXB0D0 + i);
        }

        // Clear the RX0IF flag
        mcp_bit_modify(MCP_CANINTF, MCP_RX0IF, 0);
        return 1;  // Message from RXB0 read successfully
    }

    // Check RX Buffer 1
    if (canintf & MCP_RX1IF) {
        // Read the message ID
        message->id = (mcp_read(MCP_RXB1SIDH) << 3) | (mcp_read(MCP_RXB1SIDL) >> 5);

        uint8_t dlc = mcp_read(MCP_RXB1DLC);
		message->length = dlc & 0x0F;
        // Read the message data
        for (int i = 0; i < message->length; i++) {
            message->data[i] = mcp_read(MCP_RXB1D0 + i);
        }

        // Clear the RX1IF flag
        mcp_bit_modify(MCP_CANINTF, MCP_RX1IF, 0);
        return 2;  // Message from RXB1 read successfully
    }

    return 0;  // No new message in RXB0 or RXB1
}

void can_bus_write(uint8_t tx_buffer, CAN_Message m) {
    uint8_t addr;

    // Determine base address for the chosen transmit buffer (TXB0, TXB1, TXB2)
    if (tx_buffer == 0) addr = 0x31;  // TXB0 base address
    else if (tx_buffer == 1) addr = 0x41;  // TXB1 base address
    else if (tx_buffer == 2) addr = 0x51;  // TXB2 base address
    else return;  // Invalid buffer number

    // Start SPI communication and write sequence
    spi_clear_ss();
    spi_write(MCP_WRITE);    // Write command
    spi_write(addr);          // Start address for TX buffer

    // Write the ID (Standard 11-bit ID example)
    spi_write((m.id >> 3) & 0xFF);         // TXBnSIDH
    spi_write((m.id << 5) & 0xE0);         // TXBnSIDL (standard ID only)

	spi_write(0x00);
	spi_write(0x00);

    // Write Data Length Code (DLC)
    spi_write(m.length & 0x0F);            // TXBnDLC, using lower 4 bits for data length

    // Write Data Bytes
    for (uint8_t i = 0; i < m.length; i++) {
        spi_write(m.data[i]);              // TXBnDm (data bytes)
    }
    spi_set_ss();  // End SPI communication

    // Request to send (RTS) for the chosen buffer
    mcp_request_to_send(tx_buffer);  // Signal transmission for the selected buffer
}


void can_read_print(CAN_Message *message) {
	int result = can_bus_read(message);
	
	if (result == 1) {
		// Message read from RXB0
		printf("Message from RXB0: ID = %x, Length = %d, Data: ", message->id, message->length);
		for (int i = 0; i < message->length; i++) {
			printf("%02X ", message->data[i]);
		}
		printf("\n");

	} else if (result == 2) {
		// Message read from RXB1
		printf("Message from RXB1: ID = %x, Length = %d, Data: ", message->id, message->length);
		for (int i = 0; i < message->length; i++) {
			printf("%02X ", message->data[i]);
		}
		printf("\r\n");

	} else {
		// No new message available; you can add a small delay here if needed
	}
}

void can_init(CanInit init, uint8_t loopback) {
    mcp_set_mode(MODE_CONFIG); // Enter configuration mode
    
    // Set SJW in CNF1 (bits 1-0)
    mcp_bit_modify(MCP_CNF1, 0b11000000, (init.sjw-1) << 6);

    // Set the BRP in CNF1 (bits 7-2)
    mcp_bit_modify(MCP_CNF1, 0b00111111, (init.brp-1));

    // Set BLTMODE in CNF2 (bit 7)
    mcp_bit_modify(MCP_CNF2, 0b1000000, init.bltmode << 7);

    // Set SAM in CNF2 (bit 6)
    mcp_bit_modify(MCP_CNF2, 0b01000000, init.sam << 6);

    // Set PHSEG1 in CNF2 (bits 2-0)
    mcp_bit_modify(MCP_CNF2, 0b00111000, (init.phase1-1) << 3);

    // Set PROPSEG in CNF2 (bits 5-3)
    mcp_bit_modify(MCP_CNF2, 0b00000111, (init.propag-1));


    // Set PHSEG2 in CNF3 (bits 2-0)
    mcp_bit_modify(MCP_CNF3, 0b00000111, (init.phase2-1));

	if (loopback) {
		mcp_set_mode(MODE_LOOPBACK);
	} else {
		mcp_set_mode(MODE_NORMAL);
	}

	
}

void mcp_clear_tx_buffer(int buffer_number) {
    // Ensure buffer_number is within valid range (0, 1, or 2)
    buffer_number = buffer_number % 3;

    // Determine the TXBnCTRL and TXBnSIDH addresses based on buffer_number
    char tx_ctrl;
    char tx_sidh;
    if (buffer_number == 0) {
        tx_ctrl = MCP_TXB0CTRL;
        tx_sidh = MCP_TXB0SIDH;
    } else if (buffer_number == 1) {
        tx_ctrl = MCP_TXB1CTRL;
        tx_sidh = MCP_TXB1SIDH;
    } else {
        tx_ctrl = MCP_TXB2CTRL;
        tx_sidh = MCP_TXB2SIDH;
    }

    // Step 1: Abort any ongoing transmission by clearing TXREQ
    spi_clear_ss();
    spi_write(MCP_BITMOD);
    spi_write(tx_ctrl);                  // Control register for selected buffer
    spi_write(0x08);                     // Mask for TXREQ bit
    spi_write(0x00);                     // Clear TXREQ bit to abort
    spi_set_ss();

    // Step 2: Clear buffer data, identifier, and control registers
    spi_clear_ss();
    spi_write(MCP_WRITE);
    spi_write(tx_sidh);                  // Start address for selected TX buffer
    for (int i = 0; i < 13; i++) {
        spi_write(0x00);                 // Write 0 to clear all fields in buffer
    }
    spi_set_ss();
}

void can_sensor_data (CAN_Message *m) {
    adc_read(&joystick);
    m->data[0] = joystick.y;
    m->data[1] = joystick.x;
    m->data[2] = joystick.L;
    m->data[3] = joystick.R;
    m->data[4] = test_bit(PINB, PINB0);
    m->data[5] = test_bit(PINB, PINB1);
    can_bus_write(0, *m);
}

