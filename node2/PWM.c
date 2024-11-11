#include "sam.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include "PWM.h"
#include "uart.h"

void setup_pwm_servo() {
    // Enable the PWM peripheral clock
    PMC->PMC_PCER1 |= (1 << (ID_PWM - 32));  // Enable the clock for the PWM controller

    // Configure PB13 as PWM output using Peripheral B
    PIOB->PIO_ABSR |= PIO_ABSR_P13;          // Select Peripheral B for PB13 (D21)
    PIOB->PIO_PDR |= PIO_PDR_P13;            // Disable PIO control, enable peripheral control

    // Set the PWM clock to achieve a 50 Hz signal on Channel 1
    PWM->PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(84);  // Set prescaler A to 42 (1 MHz PWM clock)

    // Configure PWM Channel 1 for 50 Hz frequency (20 ms period)
    PWM->PWM_CH_NUM[1].PWM_CMR = PWM_CMR_CPRE_CLKA;     // Use Clock A as the source for Channel 1
    PWM->PWM_CH_NUM[1].PWM_CPRD = 20000;                // Set period to 20000 (20 ms at 1 MHz)
    PWM->PWM_CH_NUM[1].PWM_CMR &= ~(1<<8);
    PWM->PWM_CH_NUM[1].PWM_CMR |= PWM_CMR_CPOL;

    // Set an initial duty cycle (e.g., midpoint for servo at 1.5 ms)
    PWM->PWM_CH_NUM[1].PWM_CDTY = 1500;                 // 1.5 ms pulse width

    // Enable the PWM channel
    PWM->PWM_ENA = PWM_ENA_CHID1;                       // Enable PWM Channel 1
}

void set_servo_position(uint16_t pulse_width) {
    if(pulse_width>2100 || pulse_width<900){
       printf("Out of range");
    }
    else {
    PWM->PWM_CH_NUM[1].PWM_CDTYUPD = pulse_width;
    }
}


uint32_t mapValue(uint32_t value, uint32_t low, uint32_t high, uint32_t min, uint32_t max) {
    // Calculate range for scaling
    uint32_t range_input = high - low;           // Expected to be 255
    uint32_t range_output = max - min;           // Expected to be 1200

    // Scale value to the output range
    uint64_t scaled_value = (uint64_t)(value - low) * range_output;
    uint64_t mapped_value = min + (scaled_value / range_input);  // Division happens last for accuracy

    return (uint32_t)mapped_value;
}



/*
// insert in main

        setup_pwm_servo();
        delay_ms(5000);
        
        set_servo_position(10000);   // Set servo to midpoint (1.5 ms pulse)
        delay_ms(5000);

        set_servo_position(2000);   // Set servo to minimum position (1 ms pulse)
        delay_ms(5000);

        set_servo_position(200);   // Set servo to maximum position (2 ms pulse)
        delay_ms(5000);


*/
