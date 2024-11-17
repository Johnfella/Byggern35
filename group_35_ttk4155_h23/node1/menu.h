#ifndef MENU_H
#define MENU_H

#include <stdint.h>
#include <util/delay.h>
#include "OLED.h"

typedef struct {
    int length;
    int *cursor_positions;
	int *next_menu;
	int next_menu_size;
} menu_struct;

menu_struct* create_menu(int length, int cursor_positions[], int next_menu[], int next_menu_size);

void start_menu(menu_struct** main_menu_ptr);
void new_game(menu_struct** main_menu_ptr);

void high_score(menu_struct** main_menu_ptr);
void menu_update_line(position pos, menu_struct menu);

void cursor_set();
void cursor_reset();
void free_menu(menu_struct* menu) ;


typedef struct {
    int cursor_position;
    int current_menu;
} oled_navigation_t;

extern volatile oled_navigation_t oled_navigation;
extern menu_struct* main_menu;


#endif // MENU_H