
#define F_CPU 84000000
#include <stdio.h>
#include <stdarg.h>
#include "time.h"
#include "sam.h"
#include "uart.h"
#include "can.h"
#include "PWM.h"
#include "encoder.h"
//#include "motor.h"
#define BAUD 115200
#define CAN_BIT_RATE 250000
#define TQ_TOTAL 1/CAN_BIT_RATE
#define TQ TQ_TOTAL/16


int main()
{
    SystemInit();
    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer

    uart_init(F_CPU, BAUD);
    CanInit initConfig = {
        .brp = 21-1,
        .phase1 = 7-1,
        .phase2 = 7-1,
        .propag = 1-1,
        .sjw = 1-1,
        .smp = 0
    };
    can_init(initConfig, 0);
    pwm_clocks_init();
    setup_pwm_servo();
    encoder_init();
    motor_init();


    CanMsg receivedMsg;
    can_data data;
    uint64_t counted_time = count_time(&data);
    uint32_t encoder_pos;
    
    while (1)
    {   
        motor_set_speed(50);
        motor_set_direction(1);
        can_data_direct(&data, &receivedMsg);
        encoder_pos = encoder_get_pos(&data);
        //printf("Current position: %d\r\n",encoder_pos);
        set_servo_position(data.servo);
        printf("joystick vertical mapped: %d  |  Joysitck horizontal mapped: %d\r\n", data.motor_position);
        //can_printmsg(receivedMsg);
        //ata.game_status = 1;
        //counted_time = count_time(&data);
        //printf("%d\n", totalSeconds(counted_time));
    }

                    
    return 1;
}