void external_clk();
void adc_init();
void adc_read();
void calibration();
void joystickPosition();
uint16_t adc_readgpt(uint8_t channel);

uint8_t offset_x;

struct multifunctionBoardTag;

typedef struct multifunctionBoardTag{
    uint8_t x;
    uint8_t y;
    uint8_t Offset_x;
    uint8_t Offset_y;
    int percent_x;
    int percent_y;

    }multifunctionBoard;


/*typedef enum direction{
    Left,
    Right,
    Up,
    Down;
}direction;*/
