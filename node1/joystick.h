void external_clk();
void adc_init();
void adc_read();
void calibration();
void joystick_print();




struct multifunctionBoardTag;
uint8_t adc_initted;

typedef struct multifunctionBoardTag{
    uint8_t x;
    uint8_t y;
    uint8_t L;
    uint8_t R;
    int8_t x_percent;
    int8_t y_percent;
    int8_t x_offset;
    int8_t y_offset;
    int32_t x_offset_percent;
    int32_t y_offset_percent;

    }multifunctionBoard;

multifunctionBoard joystick;
typedef enum {
    LEFT,
    RIGHT,
    NEUTRAL,
    UP,
    DOWN
}position;

position joystickPosition();

typedef enum{ 
    JBUTTON,
    LBUTTON,
    RBUTTON 

}button_enum;

int get_button();
int get_ir_status();

/*struct Joystick {
    enum JoystickPosition position;
};

enum JoystickPosition get_joystick_position(struct Joystick c) {
    return c.position;
}

void print_joystick_position(position position) {
    switch (position) {
        case LEFT: printf("LEFT \n"); break;
        case RIGHT: printf("RIGHT \n"); break;
        case NEUTRAL: printf("NEUTRAL \n"); break;
        case UP: printf("UP \n"); break;
        case DOWN: printf("DOWN \n"); break;
    }
}*/

