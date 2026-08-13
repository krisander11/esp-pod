#ifndef SETTINGS_H
#define SETTINGS_H

void initSettings();
void saveSettings();
void resetSettings();
int getSavedVolume();
void setSavedVolume(int volume);

int getSavedBrightness();
void setSavedBrightness(int brightness);

int getLastTrack();
void setLastTrack(int t);

#endif