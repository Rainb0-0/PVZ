#include "Shop.h"
#include "Game.h"
#include "Plant.h"
#include "PlantSelection.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int CoinCount = 0;

const int PEASHOOTER_PRICE = 0;
const int SUNFLOWER_PRICE = 0;
const int POTATO_PRICE = 5;
const int CHOMPER_PRICE = 10;
const int MARIGOLD_PRICE = 20;

FILE *SHOP_FILE;
const char *SHOP_FILE_PATH = "shop.bin";

bool IsPlantUnlocked[PLANTCOUNT];

ShopButton ShopPeashooterButton = {
    &PEASHOOTER_IDLE_TEXTURE,
    &PEASHOOTER_IDLE_FRAME_WIDTH,
    &PEASHOOTER_IDLE_FRAME_HEIGHT,
    &IsPlantUnlocked[PEASHOOTER],
    false,
    PEASHOOTER_PRICE,
    PEASHOOTER,
};

ShopButton ShopSunflowrButton = {
    &SUNFLOWER_IDLE_TEXTURE,
    &SUNFLOWER_IDLE_FRAME_WIDTH,
    &SUNFLOWER_IDLE_FRAME_HEIGHT,
    &IsPlantUnlocked[SUNFLOWER],
    false,
    SUNFLOWER_PRICE,
    SUNFLOWER,
};

ShopButton ShopPotatoButton = {
    &POTATO_AWAKE_TEXTURE,
    &POTATO_AWAKE_FRAME_WIDTH,
    &POTATO_AWAKE_FRAME_HEIGHT,
    &IsPlantUnlocked[POTATO],
    false,
    POTATO_PRICE,
    POTATO,
};

ShopButton ShopChomperButton = {
    &CHOMPER_IDLE_TEXTURE,
    &CHOMPER_IDLE_FRAME_WIDTH,
    &CHOMPER_IDLE_FRAME_HEIGHT,
    &IsPlantUnlocked[CHOMPER],
    false,
    CHOMPER_PRICE,
    CHOMPER,
};

ShopButton *ShopButtons[PLANTCOUNT] = {
    &ShopPeashooterButton,
    &ShopSunflowrButton,
    &ShopPotatoButton,
    &ShopChomperButton,
};

// TODO add marigold

// binary file, 1 int for each plant. the last int is for the number
// of coins from the last save
void Shop_WriteDefaults() {
    for (int i = 0; i <= PLANTCOUNT; i++) {
        if (i < PLANTCOUNT) {
            int unlocked = false;
            switch (i) {
            case PEASHOOTER:
                unlocked = true;
                break;
            case SUNFLOWER:
                unlocked = true;
                break;
            case POTATO:
                unlocked = true;
                break;
            default:
                unlocked = false;
                break;
            }
            fwrite(&unlocked, sizeof(int), 1, SHOP_FILE);
        } else {
            fwrite(&CoinCount, sizeof(int), 1, SHOP_FILE);
        }
    }
}

void Shop_ReadFile() {
    rewind(SHOP_FILE);
    for (int i = 0; i <= PLANTCOUNT; i++) {
        if (i < PLANTCOUNT) {
            int temp = 0;
            fread(&temp, sizeof(int), 1, SHOP_FILE);
            IsPlantUnlocked[i] = (bool)temp;
        } else {
            fread(&CoinCount, sizeof(int), 1, SHOP_FILE);
        }
    }
}

void Shop_SaveState() {
    rewind(SHOP_FILE);
    for (int i = 0; i <= PLANTCOUNT; i++) {
        if (i < PLANTCOUNT) {
            fwrite(&IsPlantUnlocked[i], sizeof(int), 1, SHOP_FILE);
        } else {
            fwrite(&CoinCount, sizeof(int), 1, SHOP_FILE);
        }
    }
}

void Shop_Init() {
    if (!FileExists(SHOP_FILE_PATH)) {
        SHOP_FILE = fopen(SHOP_FILE_PATH, "wb+");
        Shop_WriteDefaults();
    } else {
        SHOP_FILE = fopen(SHOP_FILE_PATH, "rb+");
    }
    Shop_ReadFile();
}

void Shop_Draw() {
    for (int i = 0; i < 0; i++) {
    }
}

void Shop_Update() {
}