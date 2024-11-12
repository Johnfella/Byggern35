#define RESET_CMD 0xC0
#define READ_CMD 0x03
//#define READ_RX_BUFFER 
//...
#define WRITE_CMD 0x02
//#define LOAD_TX_BUFFER
//...
//#define RTS_CMD
//...
#define READ_STATUS_CMD 0xA0
#define RX_STATUS_CMD 0xB0
#define BIT_MODIFY_CMD 0x05

char MCP2515_Read(char address) {
    PORTB &= ~(1 << PORTB4);  // Set CS low
    SPI_MasterTransmit(READ_CMD);  // Send read command
    SPI_MasterTransmit(address);   // Send the address to read from
    char result = SPI_SlaveReceive();  // Receive the data
    PORTB |= (1 << PORTB4);   // Set CS high
    return result;
}

char MCP2515_write(char address) {
    PORTB &= ~(1 << PORTB4);
    SPI_MasterTransmit(WRITE_CMD);
    SPI_MasterTransmit(address);
    char 
}