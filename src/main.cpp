#include <Arduino.h>

#include "library.h"
#include "display.h"
#include "buttons.h"
#include "player.h"
#include "menu.h"
#include "battery.h"
#include "settings.h"

void setup() {

    Serial.begin(1152000);


initSettings();
initDisplay();
setBrightness(getSavedBrightness());
initButtons();
initPlayer();
initMenu();
initBattery();

loadLibrary();

showBootScreen();

}

void loop() {
    
    updateButtons();
    updateMenu();
    updateDisplay();
}