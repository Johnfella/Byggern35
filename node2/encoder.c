#include "sam.h"
#include <stdio.h>
#include "encoder.h"


void encoder_init(void) {
    // Enable clock for the TC2 peripheral (check the Peripheral ID in ATSAM3X8E documentation)
    PMC->PMC_PCER1 |= (1 << (ID_TC6 - 32)); // TC6 corresponds to TC2 channel 0

    // Disable PIO control on PC25 and PC26 to use as peripheral inputs
    PIOC->PIO_PDR |= (PIO_PC25 | PIO_PC26);

    // Set the pins to Peripheral B mode (for TC2 quadrature input)
    PIOC->PIO_ABSR |= (PIO_PC25 | PIO_PC26);

    

    // Configure TC2 channel 0 for quadrature encoder mode
    TC2->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_XC0 |   // Set XC0 as clock
                                 TC_CMR_ABETRG;        // Use TIOA and TIOB inputs

    // Set up quadrature decoding mode
    TC2->TC_BMR = TC_BMR_QDEN |      // Enable quadrature decoder
                  TC_BMR_POSEN |     // Position enable
                  TC_BMR_EDGPHA |    // Use TIOA as Phase A and TIOB as Phase B
                  TC_BMR_MAXFILT(1); // Set minimal filter (try adjusting if needed)


    //encoder_set_pos(5000);
    // Enable the counter and reset it to 0
    TC2->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
}

int32_t encoder_get_pos(can_data *d) {
    // Read the current counter value (position)
    static int32_t pos;
    pos = (int32_t)TC2->TC_CHANNEL[0].TC_CV;

    d->motor_position = mapValue(pos,0,4800,0,100);
    return pos;
}

void encoder_reset(void) {
    // Reset the counter to zero
    TC2->TC_CHANNEL[0].TC_CCR = TC_CCR_SWTRG;
}
