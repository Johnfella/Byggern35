
#ifndef __OLED_H
#define __OLED_H

void oled_init();
void write_c(uint8_t command);
void write_d(uint8_t data);

void oled_goto_line(uint8_t line);
void oled_goto_column(uint8_t column);
void oled_pos(uint8_t row, uint8_t column);

void oled_fill_line(uint8_t line, uint8_t fill);
void oled_clear_line(uint8_t line);
void oled_clear_screen();
void oled_clear_column(uint8_t column);

void oled_home();
void oled_reset();
void oled_set();

void oled_print_string(char *str);
void display_receiving_byte(uint32_t byte);

typedef struct {
	int row, col;
} oled_position_t;

extern volatile oled_position_t oled_position;

#endif