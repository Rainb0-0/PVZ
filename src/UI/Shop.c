#include "Shop.h"
#include "Font.h"
#include "Game.h"
#include "MainMenu.h"
#include "Plant.h"
#include "PlantSelection.h"
#include "SceneManager.h"
#include "Sound.h"
#include "Wallnut.h"
#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int INIT_COIN_COUNT = 50;
int CoinCount = 10;

Texture2D SHOP_BACKGROUND_TEXTURE;
Texture2D COIN_BANK_TEXTURE;
Texture2D BACK_TEXTURE;
const char *SHOP_BACKGROUND_PATH = "Shop.png";
const char *COIN_BANK_PATH = "Sprites/CoinBank.png";
const char *BACK_PATH = "Back.png";
const char *BUY_SOUND_PATH = "Sounds/Shop/Buy/";
const char *FAIL_SOUND_PATH = "Sounds/Shop/Fail/";

const int PEASHOOTER_SHOP_PRICE = 0;
const int SUNFLOWER_SHOP_PRICE = 0;
const int POTATO_SHOP_PRICE = 5;
const int CHOMPER_SHOP_PRICE = 20;
const int WALLNUT_SHOP_PRICE = 10;

FILE *SHOP_FILE;
const char *SHOP_FILE_PATH = "shop.bin";

const float SHOP_SCALE = 1.1;
const float COIN_BANK_SCALE = SHOP_SCALE * 1.3;
const float SHOP_PRICE_FONT_SIZE = 24 * SHOP_SCALE;
float SHOP_BUTTON_WIDTH;
float SHOP_BUTTON_HEIGHT;
const float SHOP_BUTTON_MARGIN = 40 * SHOP_SCALE;
const Rectangle SHOP_BUTTON_TEXTURE_RECT = {7 * SHOP_SCALE,
                                            18 * SHOP_SCALE,
                                            85 * SHOP_SCALE,
                                            85 * SHOP_SCALE};
const Rectangle SHOP_BUTTON_PRICE_RECT = {
    5 * SHOP_SCALE,
    110 * SHOP_SCALE,
    60 * SHOP_SCALE,
    25 * SHOP_SCALE,
};

Rectangle BackButton = {10 * SHOP_SCALE, 10 * SHOP_SCALE};
bool BackButtonHover = false;

Rectangle SHOP_COIN_TEXT_RECT = {
    37 * COIN_BANK_SCALE,
    10 * COIN_BANK_SCALE,
    86 * COIN_BANK_SCALE,
    16 * COIN_BANK_SCALE,
};

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

ShopButton ShopWallnutButton = {
    &WALLNUT_NORMAL_TEXTURE,
    &WALLNUT_FRAME_WIDTH,
    &WALLNUT_FRAME_HEIGHT,
    false,
    WALLNUT_SHOP_PRICE,
    WALLNUT,
};

ShopButton *ShopButtons[PLANTCOUNT] = {
    &ShopPeashooterButton,
    &ShopSunflowrButton,
    &ShopPotatoButton,
    &ShopWallnutButton,
    &ShopChomperButton,
};

// TODO add marigold

// binary file, 1 int for each plant. the last int is for the number
// of coins from the last save
void Shop_WriteDefaults() {
    rewind(SHOP_FILE);
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
            default:
                unlocked = false;
                break;
            }
            fwrite(&unlocked, sizeof(int), 1, SHOP_FILE);
        } else {
            fwrite(&INIT_COIN_COUNT, sizeof(int), 1, SHOP_FILE);
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
            int temp = IsPlantUnlocked[i] ? 1 : 0;
            fwrite(&temp, sizeof(int), 1, SHOP_FILE);
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
    rewind(SHOP_FILE);
    Shop_ReadFile();
    SHOP_BUTTON_WIDTH = SEED_PACKET.width * SHOP_SCALE;
    SHOP_BUTTON_HEIGHT = SEED_PACKET.height * SHOP_SCALE;
    float totalWidth = (PLANTCOUNT)*SHOP_BUTTON_WIDTH +
                       (PLANTCOUNT - 1) * SHOP_BUTTON_MARGIN;
    float X_OFFSET = (GetScreenWidth() - totalWidth) / 2;
    float Y_OFFSET = (GetScreenHeight() / 2) - SHOP_BUTTON_HEIGHT / 1.2;
    for (int i = 0; i < PLANTCOUNT; i++) {
        Rectangle rect = {X_OFFSET +
                              i * (SHOP_BUTTON_WIDTH + SHOP_BUTTON_MARGIN),
                          Y_OFFSET, SHOP_BUTTON_WIDTH,
                          SHOP_BUTTON_HEIGHT};
        ShopButtons[i]->bounds = rect;
        ShopButtons[i]->unlocked = &IsPlantUnlocked[ShopButtons[i]->type];
    }
    if (!IsTextureValid(SHOP_BACKGROUND_TEXTURE)) {
        SHOP_BACKGROUND_TEXTURE = LoadTexture(SHOP_BACKGROUND_PATH);
    }
    if (!IsTextureValid(COIN_BANK_TEXTURE)) {
        COIN_BANK_TEXTURE = LoadTexture(COIN_BANK_PATH);
    }
    if (!IsTextureValid(BACK_TEXTURE)) {
        BACK_TEXTURE = LoadTexture(BACK_PATH);
    }
    BackButton.width = BACK_TEXTURE.width * SHOP_SCALE;
    BackButton.height = BACK_TEXTURE.height * SHOP_SCALE;
    CacheAllOgg(BUY_SOUND_PATH);
    CacheAllOgg(FAIL_SOUND_PATH);
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
    if (*self->unlocked == true) {
        Color overlay = GREEN;
        overlay.a = 'f';
        DrawRectangleRec(self->bounds, overlay);
    }
    if (CoinCount < self->price && *self->unlocked == false) {
        Color overlay = BLACK;
        overlay.a = 'f';
        DrawRectangleRec(self->bounds, overlay);
    }
    if (self->hovered && *self->unlocked == false && self->price <= CoinCount) {
        Color HoverColor = BLACK;
        HoverColor.a = '0';
        DrawRectangleRec(self->bounds, HoverColor);
    }
    char price[10];
    sprintf(price, "%d", self->price);
    Vector2 textSize = MeasureTextEx(FONT,
                                     price, SHOP_PRICE_FONT_SIZE, 1);
    Vector2 textPosition = {
        self->bounds.x + SHOP_BUTTON_PRICE_RECT.x +
            (SHOP_BUTTON_PRICE_RECT.width - textSize.x) / 2,
        self->bounds.y + SHOP_BUTTON_PRICE_RECT.y +
            (SHOP_BUTTON_PRICE_RECT.height - textSize.y) / 2};

    DrawTextPro(FONT, price, textPosition,
                origin, 0, SHOP_PRICE_FONT_SIZE, 1, BLACK);
}

void DrawShopBackground() {
    float width = SHOP_BACKGROUND_TEXTURE.width;
    float height = SHOP_BACKGROUND_TEXTURE.height;
    float sw = GetScreenWidth();
    float sh = GetScreenHeight();
    float COIN_BANK_AR;
    COIN_BANK_AR = fmax(sw / width, sh / height);
    width = width * COIN_BANK_AR;
    height = height * COIN_BANK_AR;
    Rectangle src = {0, 0, SHOP_BACKGROUND_TEXTURE.width,
                     SHOP_BACKGROUND_TEXTURE.height};
    Rectangle dst = {(sw - width) / 2, 0, width, height};
    Vector2 origin = {0, 0};
    DrawTexturePro(SHOP_BACKGROUND_TEXTURE, src, dst, origin, 0, WHITE);
    char title[] = "SHOP";
    float titleScale = 1.5;
    Vector2 textSize = MeasureTextEx(FONT, title,
                                     SHOP_PRICE_FONT_SIZE * titleScale, 1);
    Vector2 textPosition = {(sw - textSize.x) / 2, 30};
    DrawTextPro(FONT, title, textPosition,
                origin, 0, SHOP_PRICE_FONT_SIZE * titleScale, 1, BLACK);
}

void DrawCoinBank() {
    Rectangle src = {0, 0, COIN_BANK_TEXTURE.width, COIN_BANK_TEXTURE.height};
    float x = (GetScreenWidth() - src.width * COIN_BANK_SCALE) / 2;
    Rectangle dst = {
        x,
        GetScreenHeight() - src.height * COIN_BANK_SCALE,
        src.width * COIN_BANK_SCALE,
        src.height * COIN_BANK_SCALE,
    };
    Vector2 origin = {0, 0};
    DrawTexturePro(COIN_BANK_TEXTURE, src, dst, origin, 0, WHITE);
    char bal[10];
    sprintf(bal, "%d", CoinCount);
    Vector2 textSize = MeasureTextEx(FONT, bal,
                                     SHOP_PRICE_FONT_SIZE, 1);
    Vector2 textPosition = {
        dst.x + SHOP_COIN_TEXT_RECT.x +
            (SHOP_COIN_TEXT_RECT.width - textSize.x) / 2,
        dst.y + SHOP_COIN_TEXT_RECT.y +
            (SHOP_COIN_TEXT_RECT.height - textSize.y) / 2,
    };
    float sw = GetScreenWidth();
    float sh = GetScreenHeight();
    DrawTextEx(FONT, bal, textPosition, SHOP_PRICE_FONT_SIZE, 1, WHITE);
}

void Shop_Draw() {
    DrawShopBackground();
    for (int i = 0; i < PLANTCOUNT; i++) {
        DrawShopButton(ShopButtons[i]);
    }
    DrawCoinBank();
    Rectangle src = {0, 0, BACK_TEXTURE.width, BACK_TEXTURE.height};
    Vector2 origin = {0, 0};
    Color tint;
    if (BackButtonHover) {
        tint = GRAY;
    } else {
        tint = WHITE;
    }
    DrawTexturePro(BACK_TEXTURE, src, BackButton, origin, 0, tint);
}

void Shop_Update() {
    if (IsKeyPressed(KEY_R)) {
        Shop_WriteDefaults();
        Shop_ReadFile();
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        SceneManager_Change(SCENE_MAINMENU);
    }
    Vector2 mousePos = GetMousePosition();
    for (int i = 0; i < PLANTCOUNT; i++) {
        ShopButton *current = ShopButtons[i];
        if (IsPositionInsideRect(current->bounds, mousePos)) {
            current->hovered = true;
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (*current->unlocked == false && current->price <= CoinCount) {
                    CoinCount -= current->price;
                    IsPlantUnlocked[current->type] = true;
                    Shop_SaveState();
                    fflush(SHOP_FILE);
                    PlayRandomOgg(BUY_SOUND_PATH, 1, false);
                    break;
                } else if (CoinCount < current->price) {
                    PlayRandomOgg(FAIL_SOUND_PATH, 1, false);
                }
            }
        } else {
            current->hovered = false;
        }
    }
    if (IsPositionInsideRect(BackButton, mousePos)) {
        BackButtonHover = true;
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            PlayRandomOgg(BUTTON_CLIKC_SOUND_PATH, 1, false);
            SceneManager_Change(SCENE_MAINMENU);
        }
    } else {
        BackButtonHover = false;
    }
}