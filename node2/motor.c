#include "sam.h"
#include <stdint.h> 
#include "motor.h"

#define MOTOR_ENABLE_PIN    PIO_PB12      // PWM pin for speed control
#define MOTOR_DIR_PIN       PIO_PC23      // Direction pin

// PWM Channel and Configuration
#define MOTOR_CHANNEL         0             // PWM channel for motor control
#define PWM_FREQUENCY       20000         // PWM frequency in Hz (20kHz, under 50kHz)
#define MCK                 84000000     // System clock frequency (84 MHz)

void motor_init(void) {
    // Enable clocks for PIOB, PIOC (for direction control) and PWM controller
    PMC->PMC_PCER0 |= (1 << ID_PIOB) | (1 << ID_PIOC);
    PMC->PMC_PCER1 |= (1 << (ID_PWM - 32));

    // Configure MOTOR_ENABLE_PIN (PB12) for PWM (Peripheral B) and MOTOR_DIR_PIN (PC23) as GPIO output
    //PIOB->PIO_PDR |= MOTOR_ENABLE_PIN;          // Disable GPIO control on PB12 (Enable pin)
    //PIOB->PIO_ABSR |= MOTOR_ENABLE_PIN;         // Select Peripheral B for PWM on PB12

    PIOB->PIO_ABSR |= PIO_ABSR_P12;          // Select Peripheral B for PB13 (D21)
    PIOB->PIO_PDR |= PIO_PDR_P12;            // Disable PIO control, enable peripheral control

    PIOC->PIO_PER |= MOTOR_DIR_PIN;             // Enable GPIO control on PC23 (Dir pin)
    PIOC->PIO_OER |= MOTOR_DIR_PIN;             // Set PC23 as output for direction control

    // Configure the PWM channel for MOTOR_ENABLE_PIN (PWM Channel 0)
    PWM->PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(84);
    PWM->PWM_CH_NUM[MOTOR_CHANNEL].PWM_CMR = PWM_CMR_CPRE_CLKA;     // Use Clock A as the source for Channel 1
    PWM->PWM_CH_NUM[MOTOR_CHANNEL].PWM_CPRD = 25;                // Set period to 20000 (20 ms at 1 MHz)
    PWM->PWM_CH_NUM[MOTOR_CHANNEL].PWM_CMR &= ~(1<<8);
    PWM->PWM_CH_NUM[MOTOR_CHANNEL].PWM_CMR |= PWM_CMR_CPOL;

    PWM->PWM_CH_NUM[MOTOR_CHANNEL].PWM_CDTY = 0;                       // Start with 0% duty cycle (motor off)
    
    // Enable the PWM channel
    PWM->PWM_ENA = PWM_ENA_CHID0;    
}

void motor_set_speed(uint8_t speed) {
    // Set the motor speed (0-100%)
    if (speed > 100) speed = 100;  // Clamp speed to max 100%
    uint32_t duty = (PWM->PWM_CH_NUM[MOTOR_CHANNEL].PWM_CPRD * speed) / 100;
    PWM->PWM_CH_NUM[MOTOR_CHANNEL].PWM_CDTY = duty;
}

void motor_set_direction(uint8_t direction) {
    // Set the motor direction
    if (direction) {
        PIOC->PIO_SODR = MOTOR_DIR_PIN;  // Set direction pin high
    } else {
        PIOC->PIO_CODR = MOTOR_DIR_PIN;  // Set direction pin low
    }
}

void motor_stop(void) {
    // Stop the motor by setting speed to 0
    motor_set_speed(0);
}
