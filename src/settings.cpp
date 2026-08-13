#include <Arduino.h>
#include <LittleFS.h>
#include "settings.h"

struct SettingsData {
    uint8_t volume;
    uint8_t brightness;
    int16_t lastTrack;
};

static const char *SETTINGS_PATH = "/settings.dat";

static SettingsData current;

static SettingsData
defaultSettings() {
    SettingsData d;
    d.volume = 20;
    d.brightness = 255;
    d.lastTrack = -1;
    return d;
}

void saveSettings() {
    File f = LittleFS.open(SETTINGS_PATH, "w");
    if (!f) {
        return;
    }
    f.write((uint8_t *)&current, sizeof(SettingsData));
    f.close();
}
void initSettings() {
    if (!LittleFS.begin(true)) {
        current = defaultSettings();
        return;
    }

    File f = LittleFS.open(SETTINGS_PATH, "r");
    if (!f || f.size() !=sizeof(SettingsData)) {
        if (f) {
            f.close();
        }
        current = defaultSettings();
        saveSettings();
        return;
    }
    f.read((uint8_t *)&current, sizeof(SettingsData));
    f.close();
}

void resetSettings() {
    current = defaultSettings();
    saveSettings();
}

int getSavedVolume() {
    return current.volume;
}

void setSavedVolume(int v) {
    current.volume = (uint8_t)v;
}

int getSavedBrightness() {
    return current.brightness;
}

void setSavedBrightness(int b) {
    current.brightness = (uint8_t)b;
}

int getLastTrack() {
    return current.lastTrack;
}

void setLastTrack(int t) {
    current.lastTrack = (int16_t)t;
}