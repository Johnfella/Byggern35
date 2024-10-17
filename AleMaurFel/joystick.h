void external_clk();
void adc_init();
void adc_read();
void calibration();

uint16_t adc_readgpt(uint8_t channel);

uint8_t offset_x;

struct multifunctionBoardTag;

typedef struct multifunctionBoardTag{
    uint8_t x;
    uint8_t y;
    uint8_t L;
    uint8_t R;
    int8_t percent_x;
    int8_t percent_y;
    
    //   uint8_t Offset_x;
    //   uint8_t Offset_y;

    }multifunctionBoard;


typedef enum {
    LEFT,
    RIGHT,
    NEUTRAL,
    UP,
    DOWN
}position;

position joystickPosition();

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

