#ifndef PLAYER_H
#define PLAYER_H

#include "library.h"

void initPlayer();

void loadSavedVolume();

void addPlay(int index);

void playTrack(int track);
void nextTrack();
void previousTrack();

void pauseTrack();
void resumeTrack();

void volumeUp();
void volumeDown();

int getCurrentTrack();
int getVolume();

Song getCurrentSong();

#endif