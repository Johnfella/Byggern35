//X-axis == orange cable, AIN1. Y-axis == red cable, AIN0
//Right slider == red cable AIN3. Left slider == blue cable AIN2
#include "joystick.h"

multifunctionBoard joystick;



void external_clk(void) {
    set_bit(DDRD,PD5);
    set_bit(TCCR1B,WGM12);
    clear_bit(TCCR1A,WGM10);
    clear_bit(TCCR1A,WGM11);
    set_bit(TCCR1A,COM1A0);
    OCR1A=0;
    set_bit(TCCR1B,CS10);

}



void adc_init(void) {
    clear_bit(DDRB, PB0);
    set_bit(PORTB, PB0);
    clear_bit(DDRB, PB1);
    set_bit(DDRB, PB3);
    set_bit(PORTB, PB3);


    adc_read();
    adc_initted = 1;
}





void adc_read(multifunctionBoard * p)
{   

    volatile char *ext_adc = (char *) 0x1400; 
  
    for(int i=1; i<=4; i++){
    ext_adc[0x00]=0x01;
    }
    

    for(int i=1; i<=4; i++){
    uint8_t data = ext_adc[0x00];
    
        switch(i){
            case 1: 
                if (!adc_initted) {
                    joystick.y_offset = data - 126;
                }
                if (data - joystick.y_offset > 126) {
                    joystick.y_offset_percent = 2 - (data/126);
                } else {
                    joystick.y_offset_percent = (data/126);
                }
                joystick.y = data - (joystick.y_offset*joystick.y_offset_percent);
                
                break;
            case 2: 
                if (!adc_initted) {
                    joystick.x_offset = data - 126;
                }
                if (data - joystick.x_offset > 126) {
                    joystick.x_offset_percent = 2 - (data/126);
                } else {
                    joystick.x_offset_percent = (data/126);
                }
                joystick.x = data - joystick.x_offset*joystick.x_offset_percent;
                break;

            case 3: 
                joystick.R = data;
                break;
            case 4: 
                joystick.L = data;
                break;
        }

    }

}

position joystickPosition (multifunctionBoard * p) {
    uint8_t xValue = joystick.x;
    uint8_t yValue = joystick.y;


    joystick.x_percent=round(100*(xValue- 127)/127);
    joystick.y_percent=round(100*(yValue-127)/127);

    if ((joystick.x_percent >= 50) & (joystick.x_percent > abs(joystick.y_percent)) ){
        return RIGHT;
    }

    else if ((joystick.x_percent <= -50) & (abs(joystick.x_percent) > abs(joystick.y_percent)) ){
        return LEFT;
    }

    else if ((joystick.y_percent >= 50) & (joystick.y_percent > abs(joystick.x_percent)) ){
        return UP;
    }

    else if ((joystick.y_percent <= -50) & (abs(joystick.y_percent) > abs(joystick.x_percent)) ){
        return DOWN;oled_reset();
    }

    else {
        return NEUTRAL;
    }
}


const char* position_to_string(position pos) {
    switch (pos) {
        case LEFT: return "LEFT";
        case RIGHT: return "RIGHT";
        case NEUTRAL: return "NEUTRAL";
        case UP: return "UP";
        case DOWN: return "DOWN";
        default: return "UNKNOWN";
    }
}


int get_button(button_enum button){

    switch(button){
        case JBUTTON:
             return !test_bit(PINB, PINB1);
        case LBUTTON:
             return test_bit(PINB, PINB0);
        case RBUTTON:
             return test_bit(PINB0, PINB2);
        default:
             return EXIT_FAILURE;
    }

}

int get_ir_status() {
    return test_bit(PINB, PINB1);
}

void game_status(CAN_Message *m) {
    static uint8_t ir_status;
    if (m->data == 1) {
        ir_status = get_ir_status();
        if (!ir_status) {
            *m->data = 0;
        }
    }
}

void solenoid_pin() {
    static uint8_t solenoid_pin;
    solenoid_pin = get_button(LBUTTON);
    if (solenoid_pin) {
        clear_bit(PORTB, PB3);
    } else {
        set_bit(PORTB, PB3);
    }
}