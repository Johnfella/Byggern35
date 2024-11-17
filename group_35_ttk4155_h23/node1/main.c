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

    CAN_Message sensor_data = {
        .id = 20,
        .length = 6
    };

    uint8_t ir_status;

    CAN_Message received_msg;
    
    oled_reset();
    start_menu(&main_menu);


    while (1) {
        can_sensor_data(&sensor_data);
        menu_update_line(joystickPosition(), *main_menu);
    }
}