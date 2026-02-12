#include "LevelSelector.h"
#include "Font.h"
#include "Game.h"
#include "Level.h"
#include "MainMenu.h"
#include "Music.h"
#include "SceneManager.h"
#include "Sound.h"
#include "math.h"
#include "raylib.h"
#include <stdio.h>

float LEVELSTATS[LEVEL_COUNT];

FILE *LEVELSTATS_FILE;

const char *LEVELSTATS_FILE_PATH = "levelstats.bin";

const float LEVELSELECTOR_SCALE = 0.35;
const float LEVELSELECTOR_MARGIN = 50 * LEVELSELECTOR_SCALE;
const float LEVELSELECTOR_FONT_SIZE = 30;

Texture2D LEVELSELECTOR_BACKGROUND_TEXTURE;
Texture2D LEVELSELECTOR_BACK_TEXTURE;

Texture2D LEVEL1_THUMBNAIL_TEXTURE;
Texture2D LEVEL2_THUMBNAIL_TEXTURE;
Texture2D LEVEL3_THUMBNAIL_TEXTURE;
Texture2D LEVEL4_THUMBNAIL_TEXTURE;

const char *LEVELSELECTOR_BACKGROUND_PATH = "LevelSelector.png";
const char *LEVELSELECTOR_BACK_PATH = "Back.png";
const char *LEVEL1_THUMBNAIL_PATH = "Sprites/Thumbnails/1.png";
const char *LEVEL2_THUMBNAIL_PATH = "Sprites/Thumbnails/2.png";
const char *LEVEL3_THUMBNAIL_PATH = "Sprites/Thumbnails/3.png";
const char *LEVEL4_THUMBNAIL_PATH = "Sprites/Thumbnails/4.png";

const char *LEVEL_START_SOUND_PATH = "Sounds/Level/Start/";

Rectangle LevelSelectBackButton = {
    1 * LEVELSELECTOR_MARGIN,
    1 * LEVELSELECTOR_MARGIN,
};
bool LevelSelectorBackButtonHover = false;
const float LevelSelectorBackButtonScale = 1;

LevelSelectorButton LEVELBUTTON1 = {
    &LEVEL1_THUMBNAIL_TEXTURE,
    false,
    "NORMAL DAY",
    &LEVEL1,
};

LevelSelectorButton LEVELBUTTON2 = {
    &LEVEL2_THUMBNAIL_TEXTURE,
    false,
    "FLAG ZOMBIE",
    &LEVEL2,
};

LevelSelectorButton LEVELBUTTON3 = {
    &LEVEL3_THUMBNAIL_TEXTURE,
    false,
    "NIGHT",
    &LEVEL3,
};

LevelSelectorButton LEVELBUTTON4 = {
    &LEVEL4_THUMBNAIL_TEXTURE,
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

// 4 floats, the max time the player has survived in seconds
void LevelStats_WriteDefaults() {
    rewind(LEVELSTATS_FILE);
    float temp = 0;
    for (int i = 0; i < LEVEL_COUNT; i++) {
        fwrite(&temp, sizeof(float), 1, LEVELSTATS_FILE);
        LEVELSTATS[i] = 0;
    }
}

void LevelStats_ReadFile() {
    rewind(LEVELSTATS_FILE);
    for (int i = 0; i < LEVEL_COUNT; i++) {
        fread(&LEVELSTATS[i], sizeof(float), 1, LEVELSTATS_FILE);
    }
}

void LevelStats_SaveState() {
    rewind(LEVELSTATS_FILE);
    for (int i = 0; i < LEVEL_COUNT; i++) {
        fwrite(&LEVELSTATS[i], sizeof(float), 1, LEVELSTATS_FILE);
    }
}

void LevelSelector_Init() {
    if (!FileExists(LEVELSTATS_FILE_PATH)) {
        LEVELSTATS_FILE = fopen(LEVELSTATS_FILE_PATH, "wb+");
        LevelStats_WriteDefaults();
    } else {
        LEVELSTATS_FILE = fopen(LEVELSTATS_FILE_PATH, "rb+");
    }
    LevelStats_ReadFile();
    if (!IsTextureValid(LEVELSELECTOR_BACKGROUND_TEXTURE)) {
        LEVELSELECTOR_BACKGROUND_TEXTURE =
            LoadTexture(LEVELSELECTOR_BACKGROUND_PATH);
    }
    if (!IsTextureValid(LEVEL1_THUMBNAIL_TEXTURE)) {
        LEVEL1_THUMBNAIL_TEXTURE = LoadTexture(LEVEL1_THUMBNAIL_PATH);
    }
    if (!IsTextureValid(LEVEL2_THUMBNAIL_TEXTURE)) {
        LEVEL2_THUMBNAIL_TEXTURE = LoadTexture(LEVEL2_THUMBNAIL_PATH);
    }
    if (!IsTextureValid(LEVEL3_THUMBNAIL_TEXTURE)) {
        LEVEL3_THUMBNAIL_TEXTURE = LoadTexture(LEVEL3_THUMBNAIL_PATH);
    }
    if (!IsTextureValid(LEVEL4_THUMBNAIL_TEXTURE)) {
        LEVEL4_THUMBNAIL_TEXTURE = LoadTexture(LEVEL4_THUMBNAIL_PATH);
    }
    if (!IsTextureValid(LEVELSELECTOR_BACK_TEXTURE)) {
        LEVELSELECTOR_BACK_TEXTURE = LoadTexture(LEVELSELECTOR_BACK_PATH);
    }
    float width = LEVEL1_THUMBNAIL_TEXTURE.width * LEVELSELECTOR_SCALE;
    float height = LEVEL1_THUMBNAIL_TEXTURE.height * LEVELSELECTOR_SCALE;
    int rows = 2;
    int cols = 2;
    float totalWidth = cols * width + (cols - 1) * LEVELSELECTOR_MARGIN;
    float totalHeight = rows * height + (rows - 1) * LEVELSELECTOR_MARGIN;
    float X_OFFSET = (GetScreenWidth() - totalWidth) / 2;
    float Y_OFFSET = (GetScreenHeight() - totalHeight) / 2 + 30;
    for (int i = 0; i < LEVEL_COUNT; i++) {
        int row = i / cols;
        int col = i % cols;
        Rectangle bounds = {
            X_OFFSET + col * (width + LEVELSELECTOR_MARGIN),
            Y_OFFSET + row * (height + LEVELSELECTOR_MARGIN),
            width,
            height};

        LEVELBUTTONS[i]->bounds = bounds;
    }
    CacheAllOgg(LEVEL_START_SOUND_PATH);
}

void DrawLevelSelectorBackground() {
    float width = LEVELSELECTOR_BACKGROUND_TEXTURE.width;
    float height = LEVELSELECTOR_BACKGROUND_TEXTURE.height;
    float sw = GetScreenWidth();
    float sh = GetScreenHeight();
    float COIN_BANK_SCALE;
    COIN_BANK_SCALE = fmax(sw / width, sh / height);
    width = width * COIN_BANK_SCALE;
    height = height * COIN_BANK_SCALE;
    Rectangle src = {0, 0, LEVELSELECTOR_BACKGROUND_TEXTURE.width,
                     LEVELSELECTOR_BACKGROUND_TEXTURE.height};
    Rectangle dst = {(sw - width) / 2, 0, width, height};
    Vector2 origin = {0, 0};
    DrawTexturePro(LEVELSELECTOR_BACKGROUND_TEXTURE, src, dst, origin, 0, WHITE);
    char title[] = "LEVELS";
    float titleScale = 1;
    Vector2 textSize = MeasureTextEx(FONT, title,
                                     LEVELSELECTOR_FONT_SIZE * titleScale, 1);
    Vector2 textPosition = {(sw - textSize.x) / 2, 32.5};
    DrawTextPro(FONT, title, textPosition,
                origin, 0, LEVELSELECTOR_FONT_SIZE * titleScale, 1, BLACK);
}

void LevelSelector_Draw() {
    DrawLevelSelectorBackground();
    for (int i = 0; i < LEVEL_COUNT; i++) {
        Rectangle src = {0, 0, LEVELBUTTONS[i]->thumbnail->width,
                         LEVELBUTTONS[i]->thumbnail->height};
        Vector2 origin = {0, 0};
        if (LEVELBUTTONS[i]->hovered)
            DrawTexturePro(*LEVELBUTTONS[i]->thumbnail, src,
                           LEVELBUTTONS[i]->bounds, origin, 0, WHITE);
        else
            DrawTexturePro(*LEVELBUTTONS[i]->thumbnail, src,
                           LEVELBUTTONS[i]->bounds, origin, 0, GRAY);
    }
    Rectangle src = {
        0,
        0,
        LEVELSELECTOR_BACK_TEXTURE.width,
        LEVELSELECTOR_BACK_TEXTURE.height,
    };
    Rectangle dst = {
        LevelSelectBackButton.x,
        LevelSelectBackButton.y,
        LEVELSELECTOR_BACK_TEXTURE.width * LevelSelectorBackButtonScale,
        LEVELSELECTOR_BACK_TEXTURE.height * LevelSelectorBackButtonScale,
    };
    Vector2 origin = {0, 0};
    if (LevelSelectorBackButtonHover) {
        DrawTexturePro(LEVELSELECTOR_BACK_TEXTURE, src, dst, origin, 0, GRAY);
    } else {
        DrawTexturePro(LEVELSELECTOR_BACK_TEXTURE, src, dst, origin, 0, WHITE);
    }
}

void LevelSelector_Update() {
    if (IsKeyPressed(KEY_ESCAPE)) {
        SceneManager_Change(SCENE_MAINMENU);
    }
    Vector2 mousePos = GetMousePosition();
    for (int i = 0; i < LEVEL_COUNT; i++) {
        if (IsPositionInsideRect(LEVELBUTTONS[i]->bounds, mousePos)) {
            LEVELBUTTONS[i]->hovered = true;
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                currentLevel = LEVELBUTTONS[i]->level;
                Game_Init();
                SceneManager_Change(SCENE_GAME);
                PlayRandomOgg(LEVEL_START_SOUND_PATH, 1, false);
                SwitchPlaylist(&mh, GAME_MUSIC_PATH);
            }
        } else {
            LEVELBUTTONS[i]->hovered = false;
        }
    }
    Rectangle backButtonRect = {
        LevelSelectBackButton.x,
        LevelSelectBackButton.y,
        LEVELSELECTOR_BACK_TEXTURE.width * LevelSelectorBackButtonScale,
        LEVELSELECTOR_BACK_TEXTURE.height * LevelSelectorBackButtonScale,
    };
    if (IsPositionInsideRect(backButtonRect, mousePos)) {
        LevelSelectorBackButtonHover = true;
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            PlayRandomOgg(BUTTON_CLIKC_SOUND_PATH, 1, false);
            SceneManager_Change(SCENE_MAINMENU);
        }
    } else {
        LevelSelectorBackButtonHover = false;
    }
}