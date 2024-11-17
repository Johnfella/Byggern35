#define F_CPU 4915200
#include <avr/io.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>
#include "OLED.h"
#include "joystick.h"
#include "menu.h"
#include "CAN.h"



volatile oled_navigation_t oled_navigation;
menu_struct* main_menu = NULL;

menu_struct* create_menu(int length, int cursor_positions[], int next_menu[], int next_menu_size) {
    oled_reset();
    menu_struct* menu = (menu_struct*)malloc(sizeof(menu_struct));
    if (menu == NULL) {
        // Handle memory allocation failure
        return NULL;
    }
    menu->length = length;
    
    menu->cursor_positions = (int *)malloc(length * sizeof(int));
    if (menu->cursor_positions == NULL) {
        // Handle memory allocation failure
        free(menu);
        return NULL;
    }
    for (int i = 0; i < length; i++) {
        menu->cursor_positions[i] = cursor_positions[i];
    }
    

    menu->next_menu = (int *)malloc(next_menu_size * sizeof(int));
    if (menu->next_menu == NULL) {
        free(menu->cursor_positions);
        free(menu);
        return NULL; // Memory allocation failure
    }
    for (int i = 0; i < next_menu_size; i++) {
        menu->next_menu[i] = next_menu[i];
    }
    menu->next_menu_size = next_menu_size;

    return menu;
}

void start_menu(menu_struct** main_menu_ptr) {
    // Definir las opciones del menú
    int cursor_positions[] = {2, 5};
    int next_menu[] = {1,2};
    int next_menu_size = sizeof(next_menu) / sizeof(next_menu[0]);

    *main_menu_ptr = create_menu(2, cursor_positions, next_menu, next_menu_size);

    // Imprimir el menú
    oled_fill_line(0,0xFF);
    oled_pos(2,10);
    oled_print_string("NEW GAME");
    oled_pos(5,10);
    oled_print_string("HIGH SCORES");
    oled_fill_line(7,0xFF);
    oled_navigation.current_menu = 0;

    // Liberar la memoria asignada
}

void new_game(menu_struct** main_menu_ptr) {
    // Definir las opciones del menú
    int cursor_positions[] = {2, 5};
    int next_menu[] = {3,0};
    int next_menu_size = sizeof(next_menu) / sizeof(next_menu[0]);
    // Crear el menú
    *main_menu_ptr = create_menu(2,cursor_positions,next_menu,next_menu_size);

    // Imprimir el menú
    oled_fill_line(0,0xFF);
    oled_pos(2,10);
    oled_print_string("START NEW GAME");
    oled_pos(5,10);
    oled_print_string("RETURN");
    oled_fill_line(7,0xFF);
    oled_navigation.current_menu = 1;
    // Liberar la memoria asignada
}


void high_score(menu_struct** main_menu_ptr) {
    // Definir las opciones del menú
    int cursor_positions[] = {7};
    int next_menu[] = {0};
    int next_menu_size = sizeof(next_menu) / sizeof(next_menu[0]);
    // Crear el menú
    
    *main_menu_ptr = create_menu(1,cursor_positions,next_menu,next_menu_size);

    oled_pos(0,10);
    oled_print_string("HIGH SCORES");
    oled_pos(1,10);
    oled_print_string("0");
    oled_pos(2,10);
    oled_print_string("0");
    oled_pos(3,10);
    oled_print_string("0");
    oled_pos(4,10);
    oled_print_string("0");
    oled_pos(5,10);
    oled_print_string("0");
    oled_pos(6,10);
    oled_print_string("0");
    oled_pos(7,10);
    oled_print_string("RETURN");
    oled_navigation.current_menu = 2;
}

void cursor_set() {
    oled_pos(oled_position.row,0);
    write_d(0xFF);
    write_d(0x7E);
    write_d(0x3C);
    write_d(0x18);
    oled_pos(oled_position.row,0);
}

void cursor_reset() {
    oled_pos(oled_position.row,0);
    write_d(0x00);
    write_d(0x00);
    write_d(0x00);
    write_d(0x00);
    oled_pos(oled_position.row,0);
}


void menu_update_line(position pos, menu_struct menu) {
    static uint8_t last_pos;
    if (menu.next_menu[oled_navigation.cursor_position] == 4) {
        adc_read(&joystick);
    } else {
        if (pos != last_pos) {
        switch (pos)
        {
        case DOWN:
            if (oled_navigation.cursor_position == menu.length-1) {
                oled_navigation.cursor_position = 0;
            } else {
                oled_navigation.cursor_position++;
            }
            break;
        case UP:
            if (oled_navigation.cursor_position == 0) {
                oled_navigation.cursor_position = menu.length-1;
            } else {
                oled_navigation.cursor_position--;
            }
            break;
        case RIGHT:
                if (menu.next_menu[oled_navigation.cursor_position] == 0) {
                    start_menu(&main_menu);

                } else if (menu.next_menu[oled_navigation.cursor_position] == 1) {
                    new_game(&main_menu);
                } else if (menu.next_menu[oled_navigation.cursor_position] == 2) {
                    high_score(&main_menu);
                }
                oled_navigation.cursor_position = 0;
                break;
        default:
            break;
        }
        last_pos = pos;
        }
    }
    cursor_reset();
    oled_pos(menu.cursor_positions[oled_navigation.cursor_position],0);
    cursor_set();
}

void free_menu(menu_struct* menu) {
    if (menu != NULL) {
        if (menu->cursor_positions != NULL) {
            free(menu->cursor_positions);
        }
        if (menu->next_menu != NULL) {
            free(menu->next_menu);
        }
        free(menu);
    }
}