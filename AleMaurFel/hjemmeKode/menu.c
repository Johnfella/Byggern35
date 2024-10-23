#define F_CPU 4915200
#include <avr/io.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>
#include "OLED.h"
#include "joystick.h"

uint8_t currentMenu;
uint8_t cursorPosition;

void menuSelect() {
    switch (currentMenu) {
        case 0:
            if (cursorPosition == 0) {
                newGame();
            } else if (cursorPosition == 1) {
                highScore();
            }
            break;
        case 1:
            if (cursorPosition == 0) {
                startNewGame();
            } else if (cursorPosition == 1) {
                oledHome();
            }
            break;
        case 2:
            if (cursorPosition == 0) {
                oledHome();
            }
            break;
    }
}

void 