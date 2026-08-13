#include "library.h"

Song songs[MAX_SONGS];

int totalSongs=0;

void loadLibrary() {

    totalSongs=4;

    songs[0]={1,"Jackie and Wilson","Hozier",233, false,0};// peak music, if your reading this go listen to it!

    songs[1]={2,"DJ in Brazil","Technicolours",201, false,0};// more peak music

    songs[2]={3,"Reflections","The Neighbourhood",244, true,12};// hi stardance developer person, can i get more star dust if you like this song? Yes im breaking the fourth wall! :-0

    songs[3]={4,"Match Box","Mar Q", 198, false, 4};//just goated.

}

void saveLibrary(){

}

Song getSong(int index) {

    return songs[index];

}

void renameSong(int index, String name) {
     songs[index].title = name;
}

void toggleFavorite(int index) {
    songs[index].favorite = !songs[index].favorite;
}

void addPlay(int index) {

    songs[index].plays++;
}

int getSongCount() {

    return totalSongs;

}

void setArtist(int index, String artist) {
    songs[index].artist = artist;
}

void setDuration(int index, int seconds) {
    songs[index].duration = seconds;
}