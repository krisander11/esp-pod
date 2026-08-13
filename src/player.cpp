#include <Arduino.h>
#include <HardwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include "player.h"
#include "settings.h"

HardwareSerial mp3Serial(2);
DFRobotDFPlayerMini player;

int currentTrack = 0;
int volume = 20;

void loadSavedVolume() {
    volume = getSavedVolume();
    if (volume < 0) volume = 0;
    if (volume > 30) volume = 30;
    player.volume(volume);
}

void initPlayer() {

    mp3Serial.begin(9600, SERIAL_8N1, 16,17);
    if (player.begin(mp3Serial)){

        loadSavedVolume();

    }
}

void playTrack(int track) {

    currentTrack = track;
    player.play(track);
    addPlay(track - 1);
    setLastTrack(track);
    saveSettings();

}

void nextTrack() {

    currentTrack++;

    player.next();

}

void previousTrack() {

    if(currentTrack>1)
        currentTrack--;
    
    player.previous();

}

void pauseTrack() {
    player.pause();
}

void resumeTrack(){
    player.start();
}

void volumeUp(){
    if(volume<30){

        volume++;

        player.volume(volume);
        setSavedVolume(volume);
        saveSettings();

    }
}

void volumeDown() {
    if(volume>0){

        volume--;
        player.volume(volume);
        setSavedVolume(volume);
        saveSettings();
    }
}

int getCurrentTrack(){
    return currentTrack;
}

int getVolume() {
    return volume;
}
Song getCurrentSong() {
    return songs[currentTrack - 1];
}