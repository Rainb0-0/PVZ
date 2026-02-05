#include "Shop.h"
#include "Game.h"
#include "Plant.h"
#include "PlantSelection.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int CoinCount = 0;

const int PEASHOOTER_SHOP_PRICE = 0;
const int SUNFLOWER_SHOP_PRICE = 0;
const int POTATO_SHOP_PRICE = 5;
const int CHOMPER_SHOP_PRICE = 10;
const int MARIGOLD_SHOP_PRICE = 20;

FILE *SHOP_FILE;
const char *SHOP_FILE_PATH = "shop.bin";

const float SHOP_BUTTON_SCALE = 6;
float SHOP_BUTTON_WIDTH;
float SHOP_BUTTON_HEIGHT;
const float SHOP_BUTTON_MARGIN = 40 * SHOP_BUTTON_SCALE;
const Rectangle SHOP_BUTTON_TEXTURE_RECT = {7 * SHOP_BUTTON_SCALE,
                                            18 * SHOP_BUTTON_SCALE,
                                            85 * SHOP_BUTTON_SCALE,
                                            85 * SHOP_BUTTON_SCALE};

bool IsPlantUnlocked[PLANTCOUNT];

ShopButton ShopPeashooterButton = {
    &PEASHOOTER_IDLE_TEXTURE,
    &PEASHOOTER_IDLE_FRAME_WIDTH,
    &PEASHOOTER_IDLE_FRAME_HEIGHT,
    false,
    PEASHOOTER_SHOP_PRICE,
    PEASHOOTER,
};

ShopButton ShopSunflowrButton = {
    &SUNFLOWER_IDLE_TEXTURE,
    &SUNFLOWER_IDLE_FRAME_WIDTH,
    &SUNFLOWER_IDLE_FRAME_HEIGHT,
    false,
    SUNFLOWER_SHOP_PRICE,
    SUNFLOWER,
};

ShopButton ShopPotatoButton = {
    &POTATO_AWAKE_TEXTURE,
    &POTATO_AWAKE_FRAME_WIDTH,
    &POTATO_AWAKE_FRAME_HEIGHT,
    false,
    POTATO_SHOP_PRICE,
    POTATO,
};

ShopButton ShopChomperButton = {
    &CHOMPER_IDLE_TEXTURE,
    &CHOMPER_IDLE_FRAME_WIDTH,
    &CHOMPER_IDLE_FRAME_HEIGHT,
    false,
    CHOMPER_SHOP_PRICE,
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
    SHOP_BUTTON_WIDTH = SEED_PACKET.width * SHOP_BUTTON_SCALE;
    SHOP_BUTTON_HEIGHT = SEED_PACKET.height * SHOP_BUTTON_SCALE;
    float totalWidth = (PLANTCOUNT - 1) * SHOP_BUTTON_WIDTH +
                       (PLANTCOUNT - 2) * SHOP_BUTTON_MARGIN;
    float X_OFFSET = (GetScreenWidth() - totalWidth) / 2;
    float Y_OFFSET = (GetScreenHeight() / 2) - SHOP_BUTTON_HEIGHT;
    for (int i = 0; i < PLANTCOUNT - 1; i++) { // TODO marigold support
        Rectangle rect = {X_OFFSET +
                              i * (SHOP_BUTTON_WIDTH + SHOP_BUTTON_MARGIN),
                          Y_OFFSET, SHOP_BUTTON_WIDTH,
                          SHOP_BUTTON_HEIGHT};
        ShopButtons[i]->bounds = rect;
        ShopButtons[i]->unlocked = &IsPlantUnlocked[i];
    }
}

void DrawShopButton(ShopButton *self) {
    Rectangle bgsrc = {0, 0, SEED_PACKET.width, SEED_PACKET.height};
    Vector2 origin = {0, 0};
    DrawTexturePro(SEED_PACKET, bgsrc, self->bounds, origin, 0, WHITE);
    Rectangle src = {0, 0, *self->textureWidth, *self->textureHeight};
    float height = *self->textureHeight;
    float width = *self->textureWidth;
    if (height < width) {
        height = height * SHOP_BUTTON_TEXTURE_RECT.width / width;
        width = SHOP_BUTTON_TEXTURE_RECT.width;
    } else if (width < height) {
        width = width * SHOP_BUTTON_TEXTURE_RECT.height / height;
        height = SHOP_BUTTON_TEXTURE_RECT.height;
    } else {
        width = SHOP_BUTTON_TEXTURE_RECT.width;
        height = SHOP_BUTTON_TEXTURE_RECT.height;
    }
    Rectangle dst = {
        self->bounds.x + SHOP_BUTTON_TEXTURE_RECT.x,
        self->bounds.y + SHOP_BUTTON_TEXTURE_RECT.y,
        width,
        height,
    };
    DrawTexturePro(*self->texture, src, dst, origin, 0, WHITE);
    if (self->hovered) {
        Color HoverColor = BLACK;
        HoverColor.a = 'f';
        DrawRectangleRec(self->bounds, HoverColor);
    }
}

void Shop_Draw() {
    for (int i = 0; i < PLANTCOUNT - 1; i++) { // TODO marigold support
        DrawShopButton(ShopButtons[i]);
    }
}

void Shop_Update() {
    Vector2 mousePos = GetMousePosition();
    for (int i = 0; i < PLANTCOUNT - 1; i++) {
        ShopButton *current = ShopButtons[i];
        current->hovered = IsPositionInsideRect(current->bounds, mousePos);
    }
}