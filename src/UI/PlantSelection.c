#include "PlantSelection.h"
#include "Chomper.h"
#include "Font.h"
#include "Game.h"
#include "GameGrid.h"
#include "Jalapeno.h"
#include "Level.h"
#include "MainMenu.h"
#include "Music.h"
#include "Object.h"
#include "Peashooter.h"
#include "Potato.h"
#include "Shop.h"
#include "Sound.h"
#include "Sunflower.h"
#include "Wallnut.h"
#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

Texture2D BACKGROUND;
Texture2D SEED_PACKET;
Texture2D SUN_BANK;
Texture2D LOCK;
Texture2D COINS_TEXTURE;
const char *SEED_PACKET_PATH = "Sprites/Packet.png";
const char *BACKGROUND_PATH = "Sprites/SeedBank.png";
const char *SUN_BANK_PATH = "Sprites/SunBank.png";
const char *LOCK_PATH = "Sprites/Lock.png";
const char *COINS_PATH = "Sprites/CoinBank.png";
const char *PLANT_SOUND_PATH = "Sounds/Game/Plant/";

const float HUD_SCALE = 0.9;
const int BUTTON_WIDTH = 100;
const int BUTTON_HEIGHT = 140;
const int BUTTON_MARGIN = 5 * HUD_SCALE;
const int PRICE_FONT_SIZE = 12 * HUD_SCALE;
Rectangle SUN_BANK_RECATANGLE;
const Rectangle BUTTON_TEXTURE_RECT = {7, 18, 85, 85};
const Vector2 BACKGROUND_MARGINS = {9 * HUD_SCALE, 9 * HUD_SCALE};
const Rectangle BUTTON_PRICE_RECT = {5, 110, 60, 25};
const Rectangle SUN_COUNT_RECT = {12, 62, 54, 22};
Rectangle COINS_TEXT_RECT = {
    37 * HUD_SCALE,
    10 * HUD_SCALE,
    86 * HUD_SCALE,
    16 * HUD_SCALE,
};
Color HOVER_COLOR;

Texture2D ZOMBIE_HEAD_TEXTURE;
Texture2D METER_TEXTURE;
Texture2D PROGRESS_TEXTURE;

const char *ZOMBIE_HEAD_PATH = "Sprites/Meter/head.png";
const char *METER_PATH = "Sprites/Meter/meter.png";
const char *PROGRESS_PATH = "Sprites/Meter/text.png";
const float METER_HEIGHT = 27;

const int CHOMPER_PRICE = 150;
const float CHOMPER_PLANT_COOLDOWN = 40;
const int PEASHOOTER_PRICE = 100;
const float PEASHOOTER_PLANT_COOLDOWN = 7.5;
const int SUNFLOWER_PRICE = 50;
const float SUNFLOWER_PLANT_COOLDOWN = 7.5;
const int POTATO_PRICE = 25;
const float POTATO_PLANT_COOLDOWN = 30;
const int WALLNUT_PRICE = 50;
const float WALLNUT_PLANT_COOLDOWN = 30;
const int JALAPENO_PRICE = 200;
const float JALAPENO_PLANT_COOLDOWN = 1;

PlantButton *SelectedButton = NULL;

PlantButton PeashooterButton = {
    &PEASHOOTER_IDLE_TEXTURE,
    &PEASHOOTER_IDLE_FRAME_WIDTH,
    &PEASHOOTER_IDLE_FRAME_HEIGHT,
    (Object * (*)(void *)) newPeashooterObject,
    (void *(*)(Vector2))newPeashooter,
    PEASHOOTER_PRICE,
    0,
    false,
    &IsPlantUnlocked[PEASHOOTER],
    true,
    false,
    PEASHOOTER_PLANT_COOLDOWN,
};

PlantButton SunflowerButton = {
    &SUNFLOWER_IDLE_TEXTURE,
    &SUNFLOWER_IDLE_FRAME_WIDTH,
    &SUNFLOWER_IDLE_FRAME_HEIGHT,
    (Object * (*)(void *)) newSunflowerObject,
    (void *(*)(Vector2))newSunflower,
    SUNFLOWER_PRICE,
    0,
    false,
    &IsPlantUnlocked[SUNFLOWER],
    true,
    false,
    SUNFLOWER_PLANT_COOLDOWN,
};

PlantButton ChomperButton = {
    &CHOMPER_IDLE_TEXTURE,
    &CHOMPER_IDLE_FRAME_WIDTH,
    &CHOMPER_IDLE_FRAME_HEIGHT,
    (Object * (*)(void *)) newChomperObject,
    (void *(*)(Vector2))newChomper,
    CHOMPER_PRICE,
    0,
    false,
    &IsPlantUnlocked[CHOMPER],
    true,
    false,
    CHOMPER_PLANT_COOLDOWN,
};

PlantButton PotatoButton = {
    &POTATO_AWAKE_TEXTURE,
    &POTATO_AWAKE_FRAME_WIDTH,
    &POTATO_AWAKE_FRAME_HEIGHT,
    (Object * (*)(void *)) newPotatoObject,
    (void *(*)(Vector2))newPotato,
    POTATO_PRICE,
    0,
    false,
    &IsPlantUnlocked[POTATO],
    true,
    false,
    POTATO_PLANT_COOLDOWN,
};

PlantButton WallnutButton = {
    &WALLNUT_NORMAL_TEXTURE,
    &WALLNUT_FRAME_WIDTH,
    &WALLNUT_FRAME_HEIGHT,
    (Object * (*)(void *)) newWallnutObject,
    (void *(*)(Vector2))newWallnut,
    WALLNUT_PRICE,
    0,
    false,
    &IsPlantUnlocked[WALLNUT],
    true,
    false,
    WALLNUT_PLANT_COOLDOWN,
};

PlantButton JalapenoButton = {
    &JALAPENO_TEXTURE,
    &JALAPENO_FRAME_WIDTH,
    &JALAPENO_FRAME_HEIGHT,
    (Object * (*)(void *)) newJalapenoObject,
    (void *(*)(Vector2))newJalapeno,
    JALAPENO_PRICE,
    0,
    false,
    &IsPlantUnlocked[JALAPENO],
    true,
    false,
    JALAPENO_PLANT_COOLDOWN,
};

PlantButton *
    PlantButtons[] = {
        &SunflowerButton,
        &PeashooterButton,
        &PotatoButton,
        &WallnutButton,
        &ChomperButton,
        &JalapenoButton,
};

const int PlantButtonsSize = sizeof(PlantButtons) / sizeof(PlantButtons[0]);
float X_OFFSET = 10 * HUD_SCALE;

void ResetSelected() {
    for (int i = 0; i < PlantButtonsSize; i++) {
        PlantButtons[i]->selected = false;
    }
    SelectedButton = NULL;
}

void PlantButton_Draw(PlantButton *self) {
    Vector2 origin = {0, 0};
    Rectangle spsrc = {0, 0, SEED_PACKET.width, SEED_PACKET.height};
    Rectangle currentRect = {self->topLeft.x,
                             self->topLeft.y,
                             self->bottomRight.x - self->topLeft.x,
                             self->bottomRight.y - self->topLeft.y};
    DrawTexturePro(SEED_PACKET, spsrc, currentRect, origin, 0, WHITE);
    Rectangle src = {0, 0, *self->textureWidth, *self->textureHeight};
    float height = *self->textureHeight;
    float width = *self->textureWidth;
    float scale = currentRect.height / SEED_PACKET.height;
    if (height < width) {
        height = height * BUTTON_TEXTURE_RECT.width * scale / width;
        width = BUTTON_TEXTURE_RECT.width * scale;
    } else if (width < height) {
        width = width * BUTTON_TEXTURE_RECT.height * scale / height;
        height = BUTTON_TEXTURE_RECT.height * scale;
    } else {
        width = BUTTON_TEXTURE_RECT.width * scale;
        height = BUTTON_TEXTURE_RECT.height * scale;
    }
    Vector2 mid = {self->topLeft.x + currentRect.width / 2,
                   self->topLeft.y +
                       BUTTON_TEXTURE_RECT.y * scale +
                       BUTTON_TEXTURE_RECT.height * scale / 2};
    Rectangle dst = {mid.x - width / 2, mid.y - height / 2, width, height};
    DrawTexturePro(*self->texture, src, dst, origin, 0, WHITE);
    char price[10] = "0";
    sprintf(price, "%d", self->price);
    Vector2 textSize = MeasureTextEx(FONT, price, PRICE_FONT_SIZE, 1);
    Vector2 textPosition = {
        self->topLeft.x + BUTTON_PRICE_RECT.x * scale +
            (BUTTON_PRICE_RECT.width * scale - textSize.x) / 2,
        self->topLeft.y + BUTTON_PRICE_RECT.y * scale +
            (BUTTON_PRICE_RECT.height * scale - textSize.y) / 2};
    DrawTextEx(FONT, price, textPosition, PRICE_FONT_SIZE, 1, BLACK);
    if (self->hovered && self->maxCooldown <= self->sinceCooldown) {
        if (self->active)
            DrawRectangleRec(currentRect, HOVER_COLOR);
    }
    if (self->selected) {
        DrawRectangleLinesEx(currentRect, 2, DARKGRAY);
    }
    if (*self->unlocked == false || (currentLevel == &LEVEL3 &&
                                     self->newPlant == newSunflower)) {
        Color dis = HOVER_COLOR;
        dis.a = 'f';
        DrawRectangleRec(currentRect, dis);
        float lockScale = HUD_SCALE / 3;
        Vector2 lockPos = {currentRect.x + currentRect.width / 2 -
                               LOCK.width * lockScale / 2,
                           currentRect.y + currentRect.height / 2 -
                               LOCK.height * lockScale / 2};
        Rectangle lockSrc = {0, 0, LOCK.width, LOCK.height};
        Rectangle lockDst = {lockPos.x, lockPos.y,
                             LOCK.width * lockScale, LOCK.height * lockScale};
        DrawTexturePro(LOCK, lockSrc, lockDst, origin, 0, WHITE);
    }
    if ((self->sinceCooldown < self->maxCooldown || self->active == false) &&
        *self->unlocked) {
        float barHeight = (1 - self->sinceCooldown / self->maxCooldown) *
                          currentRect.height;
        if (SunCount < self->price) {
            barHeight = currentRect.height;
        }
        if (currentRect.height < barHeight)
            barHeight = currentRect.height;
        if (currentLevel == &LEVEL3 && self->newPlant == newSunflower) {
            barHeight = 0;
        }
        Rectangle barRec = {
            currentRect.x,
            currentRect.y + currentRect.height - barHeight,
            currentRect.width,
            barHeight,
        };
        Color bar = HOVER_COLOR;
        bar.a = 'f';
        DrawRectangleRec(barRec, bar);
    }
}

void SunCount_Draw() {
    // float width = BACKGROUND.h;
    Rectangle src = {0, 0, SUN_BANK.width, SUN_BANK.height};
    Vector2 origin = {0, 0};
    DrawTexturePro(SUN_BANK, src, SUN_BANK_RECATANGLE, origin, 0, WHITE);
    char suns[10];
    sprintf(suns, "%d", SunCount);
    Vector2 textSize = MeasureTextEx(FONT, suns, PRICE_FONT_SIZE, 1);
    float scale = SUN_BANK_RECATANGLE.height / SUN_BANK.height;
    Vector2 textPos = {
        SUN_BANK_RECATANGLE.x +
            SUN_COUNT_RECT.x * scale +
            (SUN_COUNT_RECT.width * scale - textSize.x) / 2,
        SUN_BANK_RECATANGLE.y +
            SUN_COUNT_RECT.y * scale +
            (SUN_COUNT_RECT.height * scale - textSize.y) / 2,
    };
    DrawTextPro(FONT,
                suns, textPos, origin, 0, PRICE_FONT_SIZE, 1, BLACK);
}

void CoinCount_Draw() {
    Vector2 origin = {0, 0};
    Rectangle src = {0, 0, COINS_TEXTURE.width, COINS_TEXTURE.height};
    Rectangle dst = {0, GetScreenHeight() - src.height * HUD_SCALE,
                     src.width * HUD_SCALE, src.height * HUD_SCALE};
    DrawTexturePro(COINS_TEXTURE, src, dst, origin, 0, WHITE);
    char bal[10];
    sprintf(bal, "%d", CoinCount);
    float coinFontSize = 20 * HUD_SCALE;
    Vector2 textSize = MeasureTextEx(FONT, bal, coinFontSize, 1);
    Vector2 textPosition = {
        COINS_TEXT_RECT.x + (COINS_TEXT_RECT.width - textSize.x) / 2,
        GetScreenHeight() - dst.height + COINS_TEXT_RECT.y +
            (COINS_TEXT_RECT.height - textSize.y) / 2,
    };
    DrawTextPro(FONT, bal, textPosition, origin, 0, coinFontSize, 1, WHITE);
}

const float PAUSEBUTTON_SCALE = HUD_SCALE * 0.3;
bool PAUSEBUTTON_HOVERED = false;

void PauseButton_Draw() {
    float width = BUTTON_NORMAL_TEXTURE.width;
    float height = BUTTON_NORMAL_TEXTURE.height;
    Rectangle src = {0, 0, width, height};
    Rectangle dst = {GetScreenWidth() - width * PAUSEBUTTON_SCALE,
                     GetScreenHeight() - height * PAUSEBUTTON_SCALE,
                     width * PAUSEBUTTON_SCALE, height * PAUSEBUTTON_SCALE};
    Vector2 origin = {0, 0};
    Color c = WHITE;
    if (PAUSEBUTTON_HOVERED)
        c = LIGHTGRAY;
    DrawTexturePro(BUTTON_NORMAL_TEXTURE, src, dst, origin, 0, c);
    char text[] = "PAUSE";
    float fontSize = PRICE_FONT_SIZE;
    Vector2 textSize = MeasureTextEx(FONT, text, fontSize, 1);
    Vector2 textPos = {dst.x + (dst.width - textSize.x) / 2,
                       dst.y + (dst.height - textSize.y) / 2};
    DrawTextPro(FONT, text, textPos, origin, 0, fontSize, 1, WHITE);
}

void PauseButton_Update() {
    float width = BUTTON_NORMAL_TEXTURE.width;
    float height = BUTTON_NORMAL_TEXTURE.height;
    Rectangle dst = {GetScreenWidth() - width * PAUSEBUTTON_SCALE,
                     GetScreenHeight() - height * PAUSEBUTTON_SCALE,
                     width * PAUSEBUTTON_SCALE, height * PAUSEBUTTON_SCALE};
    Vector2 mousePos = GetMousePosition();
    if (IsPositionInsideRect(dst, mousePos)) {
        PAUSEBUTTON_HOVERED = true;
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            PlayRandomOgg(BUTTON_CLIKC_SOUND_PATH, 1, false);
            mh.playing = false;
            GamePaused = true;
        }
    } else {
        PAUSEBUTTON_HOVERED = false;
    }
}

void Meter_Draw() {
    float localScale = HUD_SCALE;
    float fraction = ((float)zombiesKilled) /
                     (currentLevel->normalZombieCount +
                      currentLevel->flagZombieCount);
    int fullW = METER_TEXTURE.width;
    int w1 = (int)(fullW * fraction);
    int w2 = fullW - w1;
    float offset = 20;
    Rectangle progressSrc = {0, 0, PROGRESS_TEXTURE.width,
                             PROGRESS_TEXTURE.height};
    Rectangle src1 = {fullW - w1, METER_HEIGHT, w1, METER_HEIGHT};
    Rectangle src2 = {0, 0, w2, METER_HEIGHT};

    Rectangle dst1 = {GetScreenWidth() - w1 * localScale - offset,
                      PROGRESS_TEXTURE.height * localScale + offset / 2,
                      w1 * localScale, METER_HEIGHT * localScale};
    Rectangle dst2 = {GetScreenWidth() - fullW * localScale - offset,
                      PROGRESS_TEXTURE.height * localScale + offset / 2,
                      w2 * localScale, METER_HEIGHT * localScale};
    Rectangle progressDst = {
        GetScreenWidth() - fullW * localScale +
            (fullW - PROGRESS_TEXTURE.width) * localScale / 2 - offset,
        offset / 2 + 1,
        PROGRESS_TEXTURE.width * localScale,
        PROGRESS_TEXTURE.height * localScale,
    };
    Vector2 origin = {0, 0};
    Rectangle srcHead = {0, 0, ZOMBIE_HEAD_TEXTURE.width,
                         ZOMBIE_HEAD_TEXTURE.height};
    Rectangle dstHead = {dst1.x - srcHead.width * localScale / 2,
                         dst1.y,
                         srcHead.width * localScale, srcHead.height * localScale};
    DrawTexturePro(METER_TEXTURE, src1, dst1, origin, 0, WHITE);
    DrawTexturePro(METER_TEXTURE, src2, dst2, origin, 0, WHITE);
    DrawTexturePro(PROGRESS_TEXTURE, progressSrc, progressDst, origin, 0, WHITE);
    DrawTexturePro(ZOMBIE_HEAD_TEXTURE, srcHead, dstHead, origin, 0, WHITE);
}

void PlantSelection_Init() {
    HOVER_COLOR.a = '5';
    HOVER_COLOR.g = 'f';
    HOVER_COLOR.b = 'f';
    HOVER_COLOR.r = 'f';
    float sbh = BACKGROUND.height * HUD_SCALE - BACKGROUND_MARGINS.y * 2;
    float width = sbh * SUN_BANK.width / SUN_BANK.height;
    Rectangle tmp = {
        BACKGROUND_MARGINS.x + X_OFFSET,
        BACKGROUND_MARGINS.y,
        width,
        sbh,
    };
    SUN_BANK_RECATANGLE = tmp;
    if (!IsTextureValid(SEED_PACKET)) {
        SEED_PACKET = LoadTexture(SEED_PACKET_PATH);
    }
    if (!IsTextureValid(BACKGROUND)) {
        BACKGROUND = LoadTexture(BACKGROUND_PATH);
    }
    if (!IsTextureValid(SUN_BANK)) {
        SUN_BANK = LoadTexture(SUN_BANK_PATH);
    }
    if (!IsTextureValid(LOCK)) {
        LOCK = LoadTexture(LOCK_PATH);
    }
    if (!IsTextureValid(COINS_TEXTURE)) {
        COINS_TEXTURE = LoadTexture(COINS_PATH);
    }
    if (!IsTextureValid(METER_TEXTURE)) {
        METER_TEXTURE = LoadTexture(METER_PATH);
    }
    if (!IsTextureValid(ZOMBIE_HEAD_TEXTURE)) {
        ZOMBIE_HEAD_TEXTURE = LoadTexture(ZOMBIE_HEAD_PATH);
    }
    if (!IsTextureValid(PROGRESS_TEXTURE)) {
        PROGRESS_TEXTURE = LoadTexture(PROGRESS_PATH);
    }
    const float cellHeight = BUTTON_HEIGHT + BUTTON_MARGIN;
    for (int i = 0; i < PlantButtonsSize; i++) {
        PlantButton *current = PlantButtons[i];
        float height = BACKGROUND.height * HUD_SCALE - 2 * BACKGROUND_MARGINS.y;
        float width = height * BUTTON_WIDTH / BUTTON_HEIGHT;
        Vector2 topLeft = {X_OFFSET + SUN_BANK_RECATANGLE.width +
                               SUN_BANK_RECATANGLE.x +
                               i * (width + BUTTON_MARGIN),
                           BACKGROUND_MARGINS.y};
        Vector2 bottomRight = {topLeft.x + width,
                               topLeft.y + height};
        current->topLeft.x = topLeft.x;
        current->topLeft.y = topLeft.y;
        current->bottomRight.x = bottomRight.x;
        current->bottomRight.y = bottomRight.y;
        current->sinceCooldown = 0;
    }
    ResetSelected();
    PlayRandomOgg(PLANT_SOUND_PATH, 0, false);
}

void PlantSelection_Draw() {
    float bgScale = HUD_SCALE * 1;
    Rectangle backgroundDst = {
        X_OFFSET,
        0,
        BACKGROUND.width * bgScale,
        BACKGROUND.height * bgScale,
    };
    Rectangle backgroundSrc = {
        0, 0, BACKGROUND.width, BACKGROUND.height};
    Vector2 origin = {0, 0};
    DrawTexturePro(BACKGROUND, backgroundSrc, backgroundDst,
                   origin, 0, WHITE);
    for (int i = 0; i < PlantButtonsSize; i++) {
        PlantButton *temp = PlantButtons[i];
        PlantButton_Draw(PlantButtons[i]);
    }
    SunCount_Draw();
    CoinCount_Draw();
    if (SelectedButton != NULL && GamePaused == false) {
        Vector2 mousePos = GetMousePosition();
        float width = *SelectedButton->textureWidth;
        float height = *SelectedButton->textureHeight;
        Rectangle src = {0, 0, width, height};
        Rectangle dst = {
            mousePos.x - width * HUD_SCALE / 2,
            mousePos.y - height * HUD_SCALE / 2,
            width * HUD_SCALE,
            height * HUD_SCALE,
        };
        Color c = {255, 255, 255, 200};
        DrawTexturePro(*SelectedButton->texture,
                       src, dst, origin, 0, c);
    }
    if (currentLevel != &LEVEL4) {
        Meter_Draw();
    }
    if (!GamePaused) {
        PauseButton_Draw();
    }
}

void PlantSelection_Update() {
    Vector2 mousePos = GetMousePosition();
    bool isClicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    bool justSelected = false;
    for (int i = 0; i < PlantButtonsSize; i++) {
        PlantButton *current = PlantButtons[i];
        Rectangle currentRect = {current->topLeft.x,
                                 current->topLeft.y,
                                 current->bottomRight.x - current->topLeft.x,
                                 current->bottomRight.y - current->topLeft.y};
        if (SunCount < current->price) {
            current->active = false;
        } else {
            current->active = true;
        }
        if (IsPositionInsideRect(currentRect, mousePos) && *current->unlocked) {
            if (currentLevel == &LEVEL3 && current->newPlant == newSunflower) {
                ;
            } else {
                current->hovered = true;
            }
        } else {
            current->hovered = false;
        }
        if (isClicked && IsPositionInsideRect(currentRect, mousePos) &&
            *current->unlocked) {
            if (current->sinceCooldown < current->maxCooldown ||
                current->active == false)
                continue;
            if (currentLevel == &LEVEL3 && current->newPlant == newSunflower)
                continue;
            if (current == SelectedButton) {
                ResetSelected();
            } else {
                ResetSelected();
                SelectedButton = current;
                current->selected = true;
                justSelected = true;
            }
        }
        current->sinceCooldown += GetFrameTime();
    }
    if (isClicked) {
        if (IsPositionInsideRect(GetPlayfieldRect(), mousePos) &&
            SelectedButton != NULL) {
            Vector2 pos = SnapPosToGrid(mousePos);
            if (PlantAtPosition(pos) == NULL) {
                void *new = SelectedButton->newPlant(pos);
                Object *newObject = SelectedButton->newPlantObject(new);
                AddObject(newObject);
                calcluateWeight(pos.y);
                SelectedButton->sinceCooldown = 0;
                SunCount -= SelectedButton->price;
                PlayRandomOgg(PLANT_SOUND_PATH, 1, true);
                ResetSelected();
            }
        }
        if (!IsPositionInsideRect(GetPlayfieldRect(), mousePos) && !justSelected) {
            ResetSelected();
        }
    }
    if (!GamePaused) {
        PauseButton_Update();
    }
}