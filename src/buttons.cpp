#include <Arduino.h>
#include "buttons.h"

#define BTN_UP 32
#define BTN_DOWN 33
#define BTN_LEFT 25
#define BTN_RIGHT 26
#define BTN_SELECT 27
#define BTN_MENU 14

void initButtons() {
    pinMode(BTN_UP, INPUT_PULLUP);
    pinMode(BTN_DOWN, INPUT_PULLUP);
    pinMode(BTN_LEFT, INPUT_PULLUP);
    pinMode(BTN_RIGHT, INPUT_PULLUP);
    pinMode(BTN_SELECT, INPUT_PULLUP);
    pinMode(BTN_MENU, INPUT_PULLUP);
    
}

void updateButtons() {

}

bool upPressed() {
    return !digitalRead(BTN_UP);
}

bool downPressed() {
    return !digitalRead(BTN_DOWN);
}

bool leftPressed() {
    return !digitalRead(BTN_LEFT);
}

bool rightPressed() {
    return !digitalRead(BTN_RIGHT);
}

 bool selectPressed() {
    return !digitalRead(BTN_SELECT);

 }

 bool menuPressed() {
    return !digitalRead(BTN_MENU);
 }