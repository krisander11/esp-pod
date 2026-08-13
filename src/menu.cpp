#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#include "menu.h"
#include "buttons.h"
#include "player.h"
#include "library.h"
#include "battery.h"
#include "settings.h"
#include "display.h"

extern Adafruit_SH1106G display;


enum ScreenState {
    SCREEN_MAIN,
    SCREEN_SONGS,
    SCREEN_PLAYER,
    SCREEN_MANAGER,
    SCREEN_FAVORITES,
    SCREEN_SETTINGS,
    SCREEN_ABOUT
};

const char *mainMenu[] = {
"Music",
"Favorites",
"Music Manager",
"Settings",
"About"
};

const int MAIN_MENU_COUNT = sizeof(mainMenu)/sizeof(mainMenu[0]);

ScreenState currentState;

int menuIndex = 0;
int currentScreen = 0;
int songIndex = 0;
int lastFavIndex = 0;

const char *managerMenu[] = {
    "Rename Song",
    "Edit Artist",
    "Edit Duration",
    "Toggle Favorite",
    "Song Info",
    "Library Info",
};

const int MANAGER_COUNT = sizeof(managerMenu)/sizeof(managerMenu[0]);
const int WHITE = 7;

int managerIndex = 0;
int favoriteIndex = 0;
int favCount = 0;

//new for menu to pick song
int managerStep = 0;
int managerSongIndex = 0;
int kbIndex = 2;
int editTarget = 0;

const char CHARSET[] = " ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
const int CHARSET_LEN = sizeof(CHARSET) - 2;

char renameBuffer[17];
int renameLen = 0;
int renameCursor = 0;

bool lastUp=false;
bool lastDown=false;
bool lastLeft=false;
bool lastRight=false;
bool lastSelect=false;
bool lastMenu=false;

bool pressedUp() {
    bool now=upPressed();
    bool pressed=now && !lastUp;
    lastUp=now;
    return pressed;
}

bool pressedDown() {
    bool now=downPressed();
    bool pressed=now && !lastDown;
    lastDown=now;
    return pressed;
}

bool pressedLeft() {
    bool now=leftPressed();
    bool pressed=now && !lastLeft;
    lastLeft=now;
    return pressed;
}

bool pressedRight() {
    bool now=rightPressed();
    bool pressed=now && !lastRight;
    lastRight=now;
    return pressed;
}

bool pressedSelect() {
    bool now=selectPressed();
    bool pressed=now && !lastSelect;
    lastSelect=now;
    return pressed;
}

bool pressedMenu() {
    bool now=menuPressed();
    bool pressed=now &&!lastMenu;
    lastMenu=now;
    return pressed;
}

void drawHeader(const char *title){
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0,0);
    display.println(title);

    display.drawLine(0,10,127,10,WHITE);
}

void drawFooter(const char *text) {
    display.setCursor(0,56);
    display.println(text);
}

void initMenu() {
    currentScreen=SCREEN_MAIN;

    menuIndex=0;

    songIndex=0;

    managerIndex=0;

    favoriteIndex=0;
}

void drawScreen() {
    drawHeader("ESP32 POD");

    for (int i = 0; i < MAIN_MENU_COUNT; i++) {
        display.setCursor(4, 14 + i *10);
        if(i == menuIndex) {
            display.print(">");
        }else{
            display.print(" ");
        }
        display.println(mainMenu[i]);
    }
    display.display();
}

void updateMainScreen() {
    if (pressedDown()) {
        menuIndex = (menuIndex + 1) %MAIN_MENU_COUNT;
    }
    if (pressedUp()) {
        menuIndex = (menuIndex - 1 + MAIN_MENU_COUNT) %MAIN_MENU_COUNT;
    }
    if(pressedSelect()) {
        switch (menuIndex) {
            case 0: songIndex = 0; currentScreen = SCREEN_SONGS; break;

            case 1: favoriteIndex = 0; currentScreen = SCREEN_FAVORITES; break;

            case 2: managerIndex = 0; currentScreen = SCREEN_MANAGER; break;

            case 3: currentScreen = SCREEN_SETTINGS; break;

            case 4: currentScreen = SCREEN_ABOUT; break;
        }
    }
}

void drawSongsScreen() {
    drawHeader("music");

    int count = getSongCount();
    if (count == 0) {
        display.setCursor(4,20);
        display.println("No songs found");
    }else{
        Song s = getSong(songIndex);
        display.setCursor(0,14);
        display.print(songIndex + 1);
        display.print("/");
        display.println(s.title);
        display.setCursor(0,26);
        display.println(s.artist);
    }

    drawFooter("Sel:Play Menu:Back");
    display.display();
}

void updateSongsScreen() {
    int count = getSongCount();

    if (count > 0) {
        if(pressedDown()) {
            songIndex = (songIndex + 1) % count;
        }
        if(pressedUp()) {
            songIndex = (songIndex - 1 + count) % count;
        }
        if(pressedSelect()) {
            playTrack(getSong(songIndex).track);
            currentScreen = SCREEN_PLAYER;
        }

    }

    if(pressedMenu()) {
            currentScreen = SCREEN_MAIN;
        }
}

void drawFavouritesScreen() {
    drawHeader("Favorites");


    int total = getSongCount();
    int lastfavIndex = -1;
    for (int i = 0; i < total; i++){
        if(getSong(i).favorite) {
            favCount++;
            lastFavIndex = i;
        }
    }

    if (favCount == 0) {
        display.setCursor(4,20);
        display.println("No Favourites yet");
    }else{
        if (favoriteIndex >= total || !getSong(favoriteIndex).favorite) {
            favoriteIndex = lastFavIndex;
        }
        Song s = getSong(favoriteIndex);
        display.setCursor(0,14);
        display.println(s.title);
        display.setCursor(0,26);
        display.println(s.artist);
    }

    drawFooter("Sel:Play Menu:Back");
    display.display();
}
void updateFavouritesScreen() {
    int total = getSongCount();

    if (pressedDown() || pressedUp()) {
        int step = pressedDown() ? 1 : -1;
        for (int tries = 0; tries < total; tries++) {
            favoriteIndex = (favoriteIndex + step + total) % total;
            if (getSong(favoriteIndex).favorite) break;
        }
    }

    if (pressedSelect() && total > 0 && getSong(favoriteIndex).favorite){
        playTrack(getSong(favoriteIndex).track);
        currentScreen = SCREEN_PLAYER;
    }

    if (pressedMenu()) {
        currentScreen = SCREEN_MAIN;
    }
}

void drawPlayerScreen() {
    drawHeader("Now Playing");

    Song s = getCurrentSong();
    display.setCursor(0,14);
    display.println(s.title);
    display.setCursor(0,24);
    display.println(s.artist);

    display.setCursor(0,38);
    display.print("Vol: ");
    display.println(getVolume());

    drawFooter("</> trk ^/v Vol");
    display.display();
}

void updatePlayerScreen() {
    if (pressedRight()) {
        nextTrack();
    }
    if (pressedLeft()) {
        previousTrack();
    }
    if (pressedUp()) {
        volumeUp();
    }
    if (pressedDown()) {
        volumeDown();
    }
    if (pressedSelect()) {
        static bool playing = true;
        if (playing) {
            pauseTrack();
        }else{
            resumeTrack();
        }
        playing = !playing;
    }
    if (pressedMenu()) {
        currentScreen = SCREEN_MAIN;
    }
}

void drawManagerPickSong() {
    // was draw manager screen changed in v1.3
}

void updateManagerPickSong() {
    int count = getSongCount();

    if (count > 0) {
        if (pressedDown()) {
            managerSongIndex = (managerSongIndex + 1) % count;
        }
        if (pressedUp()) {
            managerSongIndex = (managerSongIndex - 1 + count) % count;
        }
        if (pressedSelect()) {
            managerIndex = 0;
            managerStep = 1;
        }
    }
    if (pressedMenu()) {
        currentScreen = SCREEN_MAIN;
    }
}

void drawManagerActions() {
    Song s = getSong(managerSongIndex);

    drawHeader(s.title.c_str());

    for (int i = 0; i< MANAGER_COUNT; i++) { // fixed v1.3
        display.setCursor(4, 14 + i * 10);
        if (i == managerIndex ) {
            display.print(">");
        }else{
            display.print(" ");
        }
        display.print(managerMenu[i]);
        if (i == 3) {
            display.print(s.favorite ? " [ON]" : "[OFF]");
        }
    }
    drawFooter("Sel:Choose Menu:Back");
    display.display();
}

void updateManagerActions() {
    if (pressedDown()) {
        managerIndex = (managerIndex + 1) % MANAGER_COUNT;
    }
    if (pressedUp()) {
        managerIndex = (managerIndex - 1 + MANAGER_COUNT) % MANAGER_COUNT;
    }
    if (pressedSelect()) {
        switch (managerIndex) {
            case 0: { // Rename Song
                editTarget = 0;
                String title = getSong(managerSongIndex).title;
                renameLen = title.length();
                if (renameLen > 16) renameLen = 16;
                for (int i = 0; i < 16; i++) {
                    renameBuffer[i] = (i < renameLen) ? title.charAt(i) : ' ';
                }
                renameBuffer[16] = '\0';
                renameCursor = 0;
                kbIndex = 2;
                managerStep = 2;
                break;
            }
            case 1: { // Edit Artist
                editTarget = 1;
                String artist = getSong(managerSongIndex).artist;
                renameLen = artist.length();
                if (renameLen > 16) renameLen = 16;
                for (int i = 0; i < 16; i++) {
                    renameBuffer[i] = (i < renameLen) ? artist.charAt(i) : ' ';
                }
                renameBuffer[16] = '\0';
                renameCursor = 0;
                kbIndex = 2;
                managerStep = 2;
                break;
            }
            case 2: // Edit Duration
                managerStep = 5;
                break;
            case 3: // Toggle Favorite
                toggleFavorite(managerSongIndex);
                break;
            case 4: // Song Info
                managerStep = 3;
                break;
            case 5: // Library Info
                managerStep = 4;
                break;
        }
    }
    if (pressedMenu()) {
        managerStep = 0;
    }
}

int charsetIndexOf(char c) {
    for (int i = 0; i< CHARSET_LEN; i++) {
        if (CHARSET[i] == c) return i;
    }
    return 0;
}

void drawManagerDuration() {
    drawHeader("Edit Duration");

    int d = getSong(managerSongIndex).duration;
    display.setCursor(0,24);
    display.print(d / 60);
    display.print(":");
    if (d % 60 < 10) display.print("0");
    display.println(d % 60);

    drawFooter("^v:1s </>:10s Sel:Save");
    display.display();

}

void updateManagerDuration() {
    static int editDuration = -1;
    if (editDuration < 0) editDuration = getSong(managerSongIndex).duration;

    if (pressedUp()) editDuration += 1;
    if (pressedDown()) editDuration = max(0, editDuration - 1);
    if (pressedRight()) editDuration += 10;
    if (pressedLeft()) editDuration = max(0, editDuration - 10);

    if (pressedSelect()) {
        setDuration(managerSongIndex, editDuration);
        editDuration = -1;
        managerStep = 1;
    }
    if(pressedMenu()) {
        editDuration = -1;
        managerStep = 1;
    }
}

const char KB_CHARS[] = " ABCDEFGHIJKLMNOPQRSTUVWXYZabchefghijklmnopqrstuvwxyz1234567890";
const int KB_CHAR_COUNT =sizeof(KB_CHARS) - 2; // 63, excludes null terminator
const int KB_TOTAL_KEYS = KB_CHAR_COUNT + 2;
const int KB_JUMP = 9;

char keyLabel(int idx, char *buf) {
    if (idx == 0) { buf[0]='O'; buf[1]='K'; buf[2]=0; }
    else if (idx == 1) { buf[0]='D'; buf[1]='E'; buf[2]='L'; buf[3]=0; }
    else { buf[0] = KB_CHARS[idx-2]; buf[1] = 0; }
    return buf[0];
}

void drawManagerRename() {
    drawHeader(editTarget == 0 ? "Rename Song" : "Edit Artist");

    display.setCursor(0,16);
    display.println(renameBuffer);
    int x = renameCursor * 6;
    display.drawLine(x, 26, x + 5, 26, WHITE);

    display.setCursor(0,38);
    char buf[4];
    for (int offset = -2; offset <= 2; offset++) {
        int idx = ((kbIndex + offset) % KB_TOTAL_KEYS + KB_TOTAL_KEYS) % KB_TOTAL_KEYS;
        keyLabel(idx, buf);
        if (offset == 0) display.print("[");
        display.print(buf);
        if (offset == 0) display.print("]");
        display.print(" ");
    }
    drawFooter("</>Chr ^vPage Sel:Pick");
    display.display();
}

void updateManagerRename() {
    if (pressedRight()) {
        kbIndex = (kbIndex + 1) % KB_TOTAL_KEYS;
    }
    if (pressedLeft()) {
        kbIndex = (kbIndex - 1 + KB_TOTAL_KEYS) % KB_TOTAL_KEYS;
    }
    if (pressedDown()) {
        kbIndex = (kbIndex + KB_JUMP) % KB_TOTAL_KEYS;
    }
    if (pressedUp()) {
        kbIndex = (kbIndex - KB_JUMP + KB_TOTAL_KEYS) % KB_TOTAL_KEYS;
    }
    if (pressedSelect()) {
        if (kbIndex == 0) {
            String trimmed = String(renameBuffer);
            trimmed.trim();
            if (trimmed.length() == 0) trimmed = "untitled";
            if (editTarget == 0) {
                renameSong(managerSongIndex, trimmed.c_str());
            }else{
                setArtist(managerSongIndex, trimmed.c_str());
            }
            managerStep = 1;
        } else if (kbIndex ==1) {
            if (renameCursor > 0) {
                renameCursor--;
                renameBuffer[renameCursor] = ' ';
            }
        } else {
            renameBuffer[renameCursor] = KB_CHARS[kbIndex - 2];
            if (renameCursor < 15) renameCursor++;
        }
    }
    if (pressedMenu()) {
        managerStep = 1;
    }
}

void drawManagerSongInfo() {
    Song s = getSong(managerSongIndex);
    drawHeader("Song Info");

    display.setCursor(0,14);
    display.print("Title: ");
    display.println(s.title);

    display.setCursor(0,24);
    display.print("Artist: ");
    display.println(s.artist);
    
    display.setCursor(0,34);
    display.print("Duration: ");
    display.print(s.duration);
    display.println("s");

    display.setCursor(0,44);
    display.print("Plays: ");
    display.print(s.plays);
    display.print(" Fav: ");
    display.println(s.favorite ? "Yes" : "No");

    drawFooter("Menu:Back");
    display.display();
}

void updateManagerSongInfo() {
    if (pressedMenu()) {
        managerStep = 1;
    }
}

void drawManagerLibraryInfo() {
    if (pressedMenu()) {
        managerStep = 1;
    }
}

void updateManagerLibraryInfo() {
    if (pressedMenu()) {
        managerStep = 1;
    }
}

void drawManagerScreen() {
    switch (managerStep) {
        case 0: drawManagerPickSong(); break;
        case 1: drawManagerActions(); break;
        case 2: drawManagerRename(); break;
        case 3: drawManagerSongInfo(); break;
        case 4: drawManagerLibraryInfo(); break;
        case 5: drawManagerDuration(); break;
    }
}

void updateManagerScreen() {
    switch (managerStep) {
        case 0: updateManagerPickSong(); break;
        case 1: updateManagerActions(); break;
        case 2: updateManagerRename(); break;
        case 3: updateManagerSongInfo(); break;
        case 4: updateManagerLibraryInfo(); break;
        case 5: updateManagerDuration(); break;
    }
}

void drawSettingsScreen() {
    drawHeader("Settings");

    display.setCursor(0,16);
    display.print("Volume: ");
    display.println(getVolume());

    display.setCursor(0,26);
    display.println(getSavedBrightness());

    display.setCursor(0,38);
    display.print("Battery: ");
    display.print(getBatteryLevel());
    display.println("%");

    drawFooter("</>brt ^v Vol Sel:Rst");
    display.display();

}

void UpdateSettingsScreen() {
    if (pressedUp()) {
        volumeUp();
    }
    if (pressedDown()) {
        volumeDown();
    }
    if (pressedRight()) {
        int b = getSavedBrightness();
        b = (b <= 235) ? b + 20 : 255;
        setSavedBrightness(b);
        setBrightness(b);
        saveSettings();
    }
    if (pressedLeft()) {
        int b = getSavedBrightness();
        b = (b >= 20) ? b - 20 : 0;
        setSavedBrightness(b);
        setBrightness(b);
        saveSettings();
    }   
    if (pressedSelect()) {
        resetSettings();
        loadSavedVolume();
        setBrightness(getSavedBrightness());
    }
    if (pressedMenu()) {
        currentScreen = SCREEN_MAIN;
    }
}

void drawAboutScreen() {
    drawHeader("About");

    display.setCursor (0,16);
    display.println("ESP32 POD");
    display.setCursor(0,28);
    display.print("v1.3");

    drawFooter("Menu:Back");
    display.display();
}

void updateAboutScreen() {
    if (pressedMenu()) {
        currentScreen = SCREEN_MAIN;
    }
}

void updateMenu() {
    switch (currentScreen) {
        case SCREEN_MAIN:
        updateMainScreen();
        drawScreen();
        break;
        case SCREEN_SONGS:
        updateSongsScreen();
        drawSongsScreen();
        break;
        case SCREEN_FAVORITES:
        updateFavouritesScreen();
        drawFavouritesScreen();
        break;
        case SCREEN_PLAYER:
        updatePlayerScreen();
        drawPlayerScreen();
        break;
        case SCREEN_MANAGER:
        updateManagerScreen();
        drawManagerScreen();
        break;
        case SCREEN_SETTINGS:
        UpdateSettingsScreen();
        drawSettingsScreen();
        break;
        case SCREEN_ABOUT: 
        updateAboutScreen();
        drawAboutScreen();
        break;
    }
}//new code end 10/8/2026 v1.3