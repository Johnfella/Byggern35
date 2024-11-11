#ifndef PWM_H
#define PWM_H

void setup_pwm_servo();
void set_servo_position(uint16_t pulse_width);
uint32_t mapValue(uint32_t value,uint32_t low, uint32_t high, uint32_t min, uint32_t max);


#endif