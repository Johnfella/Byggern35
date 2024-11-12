#include "sam.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include "PWM.h"
#include "uart.h"

void pwm_clocks_init(void) {
    // Configure PWM_CLK register only once to avoid interference
    // Set CLKA to MCK/42 for 2 MHz (for motor control)
    // Set CLKB to MCK/1680 for ~50 Hz (for servo control)
    PWM->PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(84) | // 
                   PWM_CLK_PREB(0) | PWM_CLK_DIVB(1680); // ~50 Hz for servo
}


void setup_pwm_servo() {
    // Enable the PWM peripheral clock
    PMC->PMC_PCER1 |= (1 << (ID_PWM - 32));  // Enable the clock for the PWM controller
    

    // Configure PB13 as PWM output using Peripheral B
    PIOB->PIO_ABSR |= PIO_ABSR_P13;          // Select Peripheral B for PB13 (D21)
    PIOB->PIO_PDR |= PIO_PDR_P13;            // Disable PIO control, enable peripheral control

    //PIOB->PIO_ABSR |= PIO_ABSR_P12;          // Select Peripheral B for PB13 (D21)
    //PIOB->PIO_PDR |= PIO_PDR_P12; 

    // Set the PWM clock to achieve a 50 Hz signal on Channel 1
    //PWM->PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(84);  // Set prescaler A to 42 (1 MHz PWM clock)

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
    static uint16_t last_pulse_width;
    if (abs(last_pulse_width - pulse_width) > 40) {
        if(pulse_width>2100 || pulse_width<900){
        printf("Out of range");
        }
        else {
        PWM->PWM_CH_NUM[1].PWM_CDTYUPD = pulse_width;
        }
    //printf("pulse_width: %d  |  last_pulse_width: %d\r\n", pulse_width, last_pulse_width);
    last_pulse_width = pulse_width;
    }
}


int32_t mapValue(int32_t value, int32_t low, int32_t high, int32_t min, int32_t max) {
    // Calculate range for scaling
    int32_t range_input = high - low;           // Expected to be 255
    int32_t range_output = max - min;           // Expected to be 1200

    // Scale value to the output range
    int64_t scaled_value = (int64_t)(value - low) * range_output;
    int64_t mapped_value = min + (scaled_value / range_input);  // Division happens last for accuracy
    if (mapped_value > max) {
        mapped_value = max;
    } else if (mapped_value < min) {
        mapped_value = min;
    }
    return (int32_t)mapped_value;
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
