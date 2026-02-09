#include "PlantSelection.h"
#include "Chomper.h"
#include "Font.h"
#include "Game.h"
#include "GameGrid.h"
#include "Level.h"
#include "Object.h"
#include "Peashooter.h"
#include "Potato.h"
#include "Shop.h"
#include "Sound.h"
#include "Sunflower.h"
#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

const int PLANT_COOLDOWN = 1;

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

const float HUD_SCALE = 3;
const int BUTTON_WIDTH = 100 * HUD_SCALE;
const int BUTTON_HEIGHT = 140 * HUD_SCALE;
const int BUTTON_MARGIN = 16 * HUD_SCALE;
const int PRICE_FONT_SIZE = 20 * HUD_SCALE;
const Rectangle BUTTON_TEXTURE_RECT = {7 * HUD_SCALE,
                                       18 * HUD_SCALE,
                                       85 * HUD_SCALE,
                                       85 * HUD_SCALE};
const Rectangle BUTTON_PRICE_RECT = {
    5 * HUD_SCALE, 110 * HUD_SCALE, 60 * HUD_SCALE, 25 * HUD_SCALE};
Rectangle COINS_TEXT_RECT = {
    37 * HUD_SCALE,
    10 * HUD_SCALE,
    86 * HUD_SCALE,
    16 * HUD_SCALE,
};
Color HOVER_COLOR;

const int CHOMPER_PRICE = 75;
const int PEASHOOTER_PRICE = 50;
const int SUNFLOWER_PRICE = 25;

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
};

PlantButton PotatoButton = {
    &POTATO_AWAKE_TEXTURE,
    &POTATO_AWAKE_FRAME_WIDTH,
    &POTATO_AWAKE_FRAME_HEIGHT,
    (Object * (*)(void *)) newPotatoObject,
    (void *(*)(Vector2))newPotato,
    CHOMPER_PRICE,
    0,
    false,
    &IsPlantUnlocked[POTATO],
    true,
    false,
};

PlantButton *PlantButtons[] = {&SunflowerButton,
                               &PeashooterButton,
                               &ChomperButton,
                               &PotatoButton};

const int PlantButtonsSize = sizeof(PlantButtons) / sizeof(PlantButtons[0]);
float X_OFFSET;
float Y_OFFSET = BUTTON_MARGIN * 1.5;

void ResetSelected() {
    for (int i = 0; i < PlantButtonsSize; i++) {
        PlantButtons[i]->selected = false;
    }
    SelectedButton = NULL;
}

void PlantButton_Draw(PlantButton *self) {
    Rectangle currentRect = {self->topLeft.x,
                             self->topLeft.y,
                             self->bottomRight.x - self->topLeft.x,
                             self->bottomRight.y - self->topLeft.y};
    DrawTextureEx(SEED_PACKET, self->topLeft, 0, HUD_SCALE, WHITE);
    Rectangle src = {0, 0, *self->textureWidth, *self->textureHeight};
    float height = *self->textureHeight;
    float width = *self->textureWidth;
    if (height < width) {
        height = height * BUTTON_TEXTURE_RECT.width / width;
        width = BUTTON_TEXTURE_RECT.width;
    } else if (width < height) {
        width = width * BUTTON_TEXTURE_RECT.height / height;
        height = BUTTON_TEXTURE_RECT.height;
    } else {
        width = BUTTON_TEXTURE_RECT.width;
        height = BUTTON_TEXTURE_RECT.height;
    }
    Vector2 mid = {self->topLeft.x + BUTTON_WIDTH / 2,
                   self->topLeft.y +
                       BUTTON_TEXTURE_RECT.y +
                       BUTTON_TEXTURE_RECT.height / 2};
    Rectangle dst = {mid.x - width / 2, mid.y - height / 2, width, height};
    Vector2 origin = {0, 0};
    DrawTexturePro(*self->texture, src, dst, origin, 0, WHITE);
    char price[] = "0";
    sprintf(price, "%d", self->price);
    Vector2 textSize = MeasureTextEx(FONT, price, PRICE_FONT_SIZE, 1);
    Vector2 textPosition = {
        self->topLeft.x + BUTTON_PRICE_RECT.x + (BUTTON_PRICE_RECT.width - textSize.x) / 2,
        self->topLeft.y + BUTTON_PRICE_RECT.y + (BUTTON_PRICE_RECT.height - textSize.y) / 2};
    DrawTextEx(FONT, price, textPosition, PRICE_FONT_SIZE, 1, BLACK);
    if (self->hovered && PLANT_COOLDOWN <= self->sinceCooldown) {
        if (self->active)
            DrawRectangleRec(currentRect, HOVER_COLOR);
    }
    if (self->selected) {
        DrawRectangleLinesEx(currentRect, 10, DARKGRAY);
    }
    if (*self->unlocked == false || (currentLevel == &LEVEL3 &&
                                     self->newPlant == newSunflower)) {
        Color dis = HOVER_COLOR;
        dis.a = 'f';
        DrawRectangleRec(currentRect, dis);
        float lockScale = HUD_SCALE / 1.5;
        Vector2 lockPos = {currentRect.x + currentRect.width / 2 -
                               LOCK.width * lockScale / 2,
                           currentRect.y + currentRect.height / 2 -
                               LOCK.height * lockScale / 2};
        Rectangle lockSrc = {0, 0, LOCK.width, LOCK.height};
        Rectangle lockDst = {lockPos.x, lockPos.y,
                             LOCK.width * lockScale, LOCK.height * lockScale};
        DrawTexturePro(LOCK, lockSrc, lockDst, origin, 0, WHITE);
    }
    if ((self->sinceCooldown < PLANT_COOLDOWN || self->active == false) &&
        *self->unlocked) {
        float barHeight = self->sinceCooldown *
                          currentRect.height / PLANT_COOLDOWN;
        if (currentRect.height < barHeight)
            barHeight = currentRect.height;
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
    float width = GetScreenWidth() - X_OFFSET - 2 * BUTTON_MARGIN;
    float height = SUN_BANK.height * width / SUN_BANK.width;
    Rectangle src = {0, 0, SUN_BANK.width, SUN_BANK.height};
    Rectangle dst = {
        X_OFFSET + BUTTON_MARGIN,
        GetScreenHeight() - height - BUTTON_MARGIN,
        width,
        height,
    };
    Vector2 origin = {0, 0};
    DrawTexturePro(SUN_BANK, src, dst, origin, 0, WHITE);
    char suns[10];
    sprintf(suns, "%d", SunCount);
    Vector2 textSize = MeasureTextEx(FONT, suns, PRICE_FONT_SIZE, 1);
    Vector2 textPos = {X_OFFSET + BUTTON_MARGIN + (width - textSize.x) / 2,
                       GetScreenHeight() - BUTTON_MARGIN * 1.5 - textSize.y};
    DrawTextPro(FONT,
                suns, textPos, origin, 0, PRICE_FONT_SIZE, 1, BLACK);
}

void PlantSelection_Init() {
    HOVER_COLOR.a = '5';
    HOVER_COLOR.g = 'f';
    HOVER_COLOR.b = 'f';
    HOVER_COLOR.r = 'f';
    X_OFFSET = GetScreenWidth() -
               BUTTON_WIDTH - 2 * BUTTON_MARGIN;
    SEED_PACKET = LoadTexture(SEED_PACKET_PATH);
    BACKGROUND = LoadTexture(BACKGROUND_PATH);
    SUN_BANK = LoadTexture(SUN_BANK_PATH);
    LOCK = LoadTexture(LOCK_PATH);
    COINS_TEXTURE = LoadTexture(COINS_PATH);
    const float cellHeight = BUTTON_HEIGHT + BUTTON_MARGIN;
    for (int i = 0; i < PlantButtonsSize; i++) {
        PlantButton *current = PlantButtons[i];
        Vector2 topLeft = {X_OFFSET + BUTTON_MARGIN, i * cellHeight + Y_OFFSET};
        Vector2 bottomRight = {topLeft.x + BUTTON_WIDTH,
                               topLeft.y + BUTTON_HEIGHT};
        current->topLeft.x = topLeft.x;
        current->topLeft.y = topLeft.y;
        current->bottomRight.x = bottomRight.x;
        current->bottomRight.y = bottomRight.y;
    }
}

void PlantSelection_Draw() {
    Rectangle backgroundDst = {
        X_OFFSET,
        0,
        GetScreenWidth() - X_OFFSET,
        GetScreenHeight(),
    };
    Rectangle backgroundSrc = {
        0, 0, BACKGROUND.width, BACKGROUND.height};
    Vector2 origin = {0, 0};
    NPatchInfo npi;
    npi.source = backgroundSrc;
    npi.bottom = 0;
    npi.left = 0;
    npi.right = 0;
    npi.top = 0;
    npi.layout = NPATCH_NINE_PATCH;
    DrawTextureNPatch(BACKGROUND, npi, backgroundDst, origin, 0, WHITE);
    for (int i = 0; i < PlantButtonsSize; i++) {
        PlantButton *temp = PlantButtons[i];
        PlantButton_Draw(PlantButtons[i]);
    }
    SunCount_Draw();
    Rectangle src = {0, 0, COINS_TEXTURE.width, COINS_TEXTURE.height};
    Rectangle dst = {0, GetScreenHeight() - src.height * HUD_SCALE,
                     src.width * HUD_SCALE, src.height * HUD_SCALE};
    DrawTexturePro(COINS_TEXTURE, src, dst, origin, 0, WHITE);
    char bal[10];
    sprintf(bal, "%d", CoinCount);
    float coinFontSize = 16 * HUD_SCALE;
    Vector2 textSize = MeasureTextEx(FONT, bal, coinFontSize, 1);
    Vector2 textPosition = {
        COINS_TEXT_RECT.x + (COINS_TEXT_RECT.width - textSize.x) / 2,
        GetScreenHeight() - dst.height + COINS_TEXT_RECT.y +
            (COINS_TEXT_RECT.height - textSize.y) / 2,
    };
    DrawTextPro(FONT, bal, textPosition, origin, 0, coinFontSize, 1, WHITE);
}

void PlantSelection_Update() {
    Vector2 mousePos = GetMousePosition();
    bool isClicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
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
            current->hovered = true;
        } else {
            current->hovered = false;
        }
        if (isClicked && IsPositionInsideRect(currentRect, mousePos) &&
            *current->unlocked &&
            (currentLevel == &LEVEL3 && current->newPlant != newSunflower)) {
            if (current->sinceCooldown < PLANT_COOLDOWN ||
                current->active == false)
                continue;
            ResetSelected();
            SelectedButton = current;
            current->selected = true;
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
                PlayRandomOggWithPitch(PLANT_SOUND_PATH, 1);
                ResetSelected();
            }
        }
        // TODO reset selected if not in the grid
    }
}