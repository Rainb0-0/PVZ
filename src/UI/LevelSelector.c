#include "LevelSelector.h"
#include "Level.h"
#include "raylib.h"
#include <stdio.h>
/*
4 buttons
not all of them are active
you have to complete level 1 for 2 and 2 for 3 and 3 for 4
each button has a thumbnail and a small description
LEVEL 1: normal zombie as thumbnail on day.png
LEVEL 2: flag zombie as thumbnail on day.png, broken lawnmower
LEVEL 3: normal and flag on night.png
LEVEL 4: infinite symbol on day.png with zombies

clicking each level switches to that level
it has a button to return to the main mesnu

*/

FILE *LEVELSTATS_FILE;

const char *LEVELSTATS_FILE_PATH = "levelstats.bin";

int LevelNormalZombiesKilled[LEVEL_COUNT];
int LevelFlagZombiesKilled[LEVEL_COUNT];

const float LEVELSELECTOR_SCALE = 3;

Texture2D LEVEL1_THUMBNAIL_TEXTUE;
Texture2D LEVEL2_THUMBNAIL_TEXTUE;
Texture2D LEVEL3_THUMBNAIL_TEXTUE;
Texture2D LEVEL4_THUMBNAIL_TEXTUE;

const char *LEVEL1_THUMBNAIL_PATH = "Sprites/Thumbnails/1.png";
const char *LEVEL2_THUMBNAIL_PATH = "Sprites/Thumbnails/2.png";
const char *LEVEL3_THUMBNAIL_PATH = "Sprites/Thumbnails/3.png";
const char *LEVEL4_THUMBNAIL_PATH = "Sprites/Thumbnails/4.png";

LevelSelectorButton LEVELBUTTON1 = {
    &LEVEL1_THUMBNAIL_TEXTUE,
    false,
    "NORMAL DAY",
    &LEVEL1,
};

LevelSelectorButton LEVELBUTTON2 = {
    &LEVEL2_THUMBNAIL_TEXTUE,
    false,
    "FLAG ZOMBIE",
    &LEVEL2,
};

LevelSelectorButton LEVELBUTTON3 = {
    &LEVEL3_THUMBNAIL_TEXTUE,
    false,
    "NIGHT",
    &LEVEL3,
};

LevelSelectorButton LEVELBUTTON4 = {
    &LEVEL4_THUMBNAIL_TEXTUE,
    false,
    "INFINITE",
    &LEVEL4,
};

LevelSelectorButton *LEVELBUTTONS[LEVEL_COUNT] = {
    &LEVELBUTTON1,
    &LEVELBUTTON2,
    &LEVELBUTTON3,
    &LEVELBUTTON4,
};

/*
the first int contains the last level which has been unlocked
the second to fifth ints contain the normal zombies killed on each level
the sixth to ninth ints contain the flag zombies killed on each level
*/
void LevelStats_WriteDefaults() {
    int defaults[LEVEL_COUNT * 2] = {0};
    fwrite(defaults, sizeof(int), LEVEL_COUNT * 2, LEVELSTATS_FILE);
}

void LevelStats_ReadFile() {
    int data[LEVEL_COUNT * 2];
    fread(data, sizeof(int), LEVEL_COUNT * 2, LEVELSTATS_FILE);
    for (int i = 0; i < LEVEL_COUNT; i++) {
        LevelNormalZombiesKilled[i] = data[i];
    }
    for (int i = LEVEL_COUNT; i < LEVEL_COUNT * 2; i++) {
        LevelFlagZombiesKilled[i - LEVEL_COUNT] = data[i];
    }
}

void LevelStats_SaveState() {
    int data[LEVEL_COUNT * 2];
    for (int i = 0; i < LEVEL_COUNT; i++) {
        data[i] = LevelNormalZombiesKilled[i];
    }
    for (int i = LEVEL_COUNT; i < LEVEL_COUNT * 2; i++) {
        data[i] = LevelFlagZombiesKilled[i - LEVEL_COUNT];
    }
    rewind(LEVELSTATS_FILE);
    fwrite(data, sizeof(int), LEVEL_COUNT * 2, LEVELSTATS_FILE);
}

void LevelSelector_Init() {
    if (!FileExists(LEVELSTATS_FILE_PATH)) {
        LEVELSTATS_FILE = fopen(LEVELSTATS_FILE_PATH, "wb+");
        LevelStats_WriteDefaults();
    } else {
        LEVELSTATS_FILE = fopen(LEVELSTATS_FILE_PATH, "rb+");
    }
    LevelStats_ReadFile();
    LEVEL1_THUMBNAIL_TEXTUE = LoadTexture(LEVEL1_THUMBNAIL_PATH);
    LEVEL2_THUMBNAIL_TEXTUE = LoadTexture(LEVEL2_THUMBNAIL_PATH);
    LEVEL3_THUMBNAIL_TEXTUE = LoadTexture(LEVEL3_THUMBNAIL_PATH);
    LEVEL4_THUMBNAIL_TEXTUE = LoadTexture(LEVEL4_THUMBNAIL_PATH);
}

void LevelSelector_Draw() {
}

void LevelSelector_Update() {
}