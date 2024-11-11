
#define F_CPU 84000000
#include <stdio.h>
#include <stdarg.h>
#include "time.h"
#include "sam.h"
#include "uart.h"
#include "can.h"
#include "PWM.h"
#define BAUD 115200
#define CAN_BIT_RATE 250000
#define TQ_TOTAL 1/CAN_BIT_RATE
#define TQ TQ_TOTAL/16
/*
 * Remember to update the Makefile with the (relative) path to the uart.c file.
 * This starter code will not compile until the UART file has been included in the Makefile. 
 * If you get somewhat cryptic errors referencing functions such as _sbrk, 
 * _close_r, _write_r, _fstat etc, you have most likely not done that correctly.

 * If you get errors such as "arm-none-eabi-gcc: no such file", you may need to reinstall the arm gcc packages using
 * apt or your favorite package manager.
 */



int main()
{


    SystemInit();
    uart_init(F_CPU, BAUD);
    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer


    CanInit initConfig = {
        .brp = 21-1,
        .phase1 = 7-1,
        .phase2 = 7-1,
        .propag = 1-1,
        .sjw = 1-1,
        .smp = 0
    };

    can_init(initConfig, 0);
    /*uint32_t can_status = CAN0->CAN_SR;
    printf("SAM3X8E CAN Status Register: 0x%08X\n\r", can_status);
    delay_ms(10);
    CanMsg msg;
    msg.id = 1;
    msg.length = 2;
    msg.byte[0] = 0xAB;
    msg.byte[1] = 0xCD;
    printf("\n\r");*/
    CanMsg receivedMsg;
    //CanMsg new_message;
    can_data data;
    //setup_pwm_servo();
    while (1)
    {   

        can_data_direct(&data, &receivedMsg);
        printf("joyisck: %d\r\n", data.vert_map);
        //can_printmsg(receivedMsg);
        delay_ms(10);
    }

                    
    return 1;
}