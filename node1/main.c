#define F_CPU 4915200
#include <stdio.h>
#include <util/delay.h>
#include <avr/io.h>

#include "UART.h"
#include "SRAM_test.h"

#include "joystick.h"
#include "OLED.h"
#include "menu.h"

#include"SPI.h"
#include"CAN.h"
#include"CAN_MCP2515.h"

#define BAUD 9600
#define MYUBRR (F_CPU/16/BAUD-1)
#define CAN_CLOCK 16000000UL
#define CAN_BIT_RATE 250000UL
#define CAN_BIT_TIME 1/CAN_BIT_RATE
#define TQ CAN_BIT_TIME/16




void main(void) {
    USART_init(MYUBRR);
    SRAM_init();
    external_clk();
    oled_init();
    adc_init();
    spi_master_init();
    mcp_init();

    CanInit initConfig = {
        .brp = 2,
        .sjw = 1,
        .propag = 1,
        .phase1 = 7,
        .sam = 0,
        .bltmode = 1,
        .phase2 = 7
    };
    


    can_init(initConfig, 0);

    _delay_ms(1000);
    oled_set();

    CAN_Message sensor_data = {
        .id = 20,
        .length = 5
    };

    CAN_Message game_status = {
        .id = 10,
        .data = 0,
        .length = 1
    };

    CAN_Message received_msg;
    uint8_t ir_status;
    while (1) {
        //adc_read();
        //printf("%x\r\n", joystickPosition);
        //sensor_data.id = 0x280;
        
        //printf("%x\r\n",message->data[0]);
        printf("Current breakline: %d\r\n", ir_status);
        //can_bus_write(0, sensor_data);
        //can_read_print(&received_msg);
        can_sensor_data(&sensor_data);
        
        _delay_ms(10);
        
    }
}
/*
    while(1) {
        for (int i=0x00; i<=0xFF; i++){
            interrupt = mcp_read(MCP_CANINTF);
                if (interrupt != 0) {
                    mcp_write(MCP_CANINTF, 0);
                }
            _delay_ms(1000);
            mcp_write(MCP_TXB0SIDH, i); 
            mcp_request_to_send(0); 
            interrupt = mcp_read(MCP_CANINTF);
                if (interrupt != 0) {
                    mcp_write(MCP_CANINTF, 0);
                }
            byte = mcp_read(MCP_RXB1SIDH);
            printf("Receiving byte %x\r\n", (byte));
            //display_receiving_byte(byte);
        }
    }
}*/

