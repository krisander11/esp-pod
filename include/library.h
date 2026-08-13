#ifndef LIBRARY_H
#define LIBRARY_H

#include <Arduino.h>

#define MAX_SONGS 200

struct Song {

    uint16_t track;

    String title;

    String artist;

    uint16_t duration;

    bool favorite;

    uint16_t plays;

};

extern Song songs[MAX_SONGS];

extern int totalSongs;

int getSongCount();

void loadLibrary();

void saveLibrary();

Song getSong(int index);

void renameSong(int index, String name);

void toggleFavorite(int index);

void setArtist(int index, String artist);

void setDuration(int index, int seconds);

#endif