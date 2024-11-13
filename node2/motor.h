#ifndef MOTOR_H
#define MOTOR_H

void motor_init(void);
void motor_set_speed(uint8_t speed);
void motor_set_direction(uint8_t direction);
void motor_stop(void);

#endif