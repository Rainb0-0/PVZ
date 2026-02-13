#include "Sun.h"
#include "Game.h"
#include "GameGrid.h"
#include "PlantSelection.h"
#include "Sound.h"
#include "raylib.h"
#include <math.h>
#include <stdlib.h>

const int SunWorth = 25;

const float SUN_TARGET_TIME = 0.45;
const float SUN_AGE = 10;
const float SUN_ALPHA_TIME = 1;

Texture2D SUN_TEXTURE;

const char *SUN_PATH = "Sprites/Sun.png";
const char *SUN_COLLECT_SOUND_PATH = "Sounds/Sun/";
const float SUN_SCALE = 1;

const float SUN_FALLING_VEL = 100;

const int SUN_FRAME_WIDTH = 140;
const int SUN_FRAME_HEIGHT = 140;
const int SUN_MAX_FRAMES = 65;
const float SUN_FRAME_TIME = 0.01;

State SUN_IDLE = {
    SUN_FRAME_WIDTH,
    SUN_FRAME_HEIGHT,
    SUN_MAX_FRAMES,
    1,
    SUN_FRAME_TIME,
    &SUN_TEXTURE,
};

void Sun_Init() {
    if (!IsTextureValid(SUN_TEXTURE)) {
        SUN_TEXTURE = LoadTexture(SUN_PATH);
    }
    CacheAllOgg(SUN_COLLECT_SOUND_PATH);
}

Sun *newSun(Vector2 dest) {
    Sun *new = (Sun *)malloc(sizeof(Sun));
    const int offsetLimit = 150;
    float xOffset = rand() % offsetLimit - offsetLimit / 2;
    float yOffset = rand() % offsetLimit - offsetLimit / 2;
    dest.x = xOffset + dest.x;
    dest.y = yOffset + dest.y;
    new->dest.x = dest.x;
    new->dest.y = dest.y;
    new->pos.x = dest.x;
    new->pos.y = GetCellDimensions().y * -4 + dest.y;
    new->initPos = dest;
    new->vel.x = 0;
    new->vel.y = 0;
    new->accel.x = 0;
    new->accel.y = 0;
    new->state = &SUN_IDLE;
    new->frameIndex = 0;
    new->alpha = 0;
    new->frameTime = 0;
    new->update = Sun_Update;
    new->draw = Sun_Draw;
    new->isFalling = true;
    new->isClicked = false;
    new->sinceLanded = 0;
    return new;
}

void Sun_Draw(Sun *self) {
    int index = FindObjectIndex(self, false);
    Vector2 offset = {0, 0};
    if (self->alpha > 1.0f)
        self->alpha = 1.0f;
    if (self->alpha < 0.0f)
        self->alpha = 0.0f;
    Color c = {255, 255, 255, (unsigned char)(self->alpha * 255)};
    DrawObject(Objects[index], SUN_SCALE, offset, c);
}

void Sun_Update(Sun *self) {
    float dt = GetFrameTime();
    float xDistance = self->dest.x - self->pos.x;
    float yDistance = self->dest.y - self->pos.y;
    if (self->alpha < 1 && self->isFalling) {
        self->alpha += 0.01;
    }
    if (self->isFalling) {
        if (fabs(yDistance) < POSITION_TOLERANCE.y)
            self->isFalling = false;
        self->vel.y = SUN_FALLING_VEL;
    } else if (self->isFalling == false && !self->isClicked) {
        self->sinceLanded += dt;
        if (SUN_AGE < self->sinceLanded) {
            if (self->alpha <= 0) {
                RemoveObject(self, false);
                return;
            } else {
                self->alpha -= 0.01;
            }
        }
        self->vel.y = 0;
    }
    if (self->isClicked) {
        if (fabs(xDistance) < POSITION_TOLERANCE.x &&
            fabs(yDistance) < POSITION_TOLERANCE.y) {
            SunCount += SunWorth;
            RemoveObject(self, false);
        }
        float d = sqrt(xDistance * xDistance + yDistance * yDistance);
        self->accel.x = (xDistance - self->vel.x * SUN_TARGET_TIME) /
                        SUN_TARGET_TIME / SUN_TARGET_TIME * 2;
        self->accel.y = (yDistance - self->vel.y * SUN_TARGET_TIME) /
                        SUN_TARGET_TIME / SUN_TARGET_TIME * 2;
        self->vel.x += self->accel.x * dt;
        self->vel.y += self->accel.y * dt;
    }
    self->pos.x += self->vel.x * dt;
    self->pos.y += self->vel.y * dt;
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 center = {self->pos.x, self->pos.y};
        float frameWidth = (*self->state).frameWidth;
        float frameHeight = (*self->state).frameHeight;
        float radius = fmax(frameHeight, frameWidth) / 4;
        Vector2 mousePosition = GetMousePosition();
        if (IsPositionInsideCircle(center, radius, mousePosition) &&
            self->isClicked == false) {
            self->isClicked = true;
            self->isFalling = false;
            self->dest.x = SUN_BANK_RECATANGLE.x +
                           SUN_BANK_RECATANGLE.width / 2;
            self->dest.y = SUN_BANK_RECATANGLE.y +
                           SUN_BANK_RECATANGLE.height / 2;
            PlayRandomOgg(SUN_COLLECT_SOUND_PATH, 1, true);
        }
    }
}