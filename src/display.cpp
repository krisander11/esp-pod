#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "display.h"
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void initDisplay() {
    Wire.begin(21,22);

    Serial.println("Calling display.begin()...");
    bool ok = display.begin(0X3C, true);
    Serial.print("display.begin() returned: ");
    Serial.println(ok ? "true" : "false");

    display.clearDisplay();
    display.setTextColor(SH110X_WHITE);
    display.display();
    Serial.println("initDisplay() done");

}

void showBootScreen() {
    Serial.println("showBootScreen() start");
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(20 ,20);
    display.println("ESP32 POD");

    display.setTextSize(1);
    display.setCursor(35 ,48);

    display.println("Booting...");
    display.display();
    Serial.println("showBootScreen() done, display.display() called");

    delay(1500);

}

void updateDisplay() {
    
}

void setBrightness(uint8_t level) {
    display.setContrast(level);
}