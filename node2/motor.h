#ifndef MOTOR_H
#define MOTOR_H
#include "can.h"
#include "encoder.h"
#include "PWM.h"
#include "time.h"

void motor_init(void);
void motor_set_speed(uint8_t speed);
void motor_set_direction(uint8_t direction);
void motor_stop(void);
void motor_controller(can_data *d);

#endif