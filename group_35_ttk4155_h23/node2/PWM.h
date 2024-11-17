#ifndef PWM_H
#define PWM_H
#include "can.h"
#include "motor.h"

void servo_init();
void pwm_clocks_init(void);
void set_servo_position(uint16_t pulse_width);
int32_t mapValue(int32_t value,int32_t low, int32_t high, int32_t min, int32_t max);
void solenoid_init();
void solenoid_control(can_data d);
void actuators(can_data d);
#endif