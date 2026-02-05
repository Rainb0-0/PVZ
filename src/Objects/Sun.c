#include "Sun.h"
#include "Game.h"
#include "GameGrid.h"
#include "PlantSelection.h"
#include "Sound.h"
#include "raylib.h"
#include <math.h>
#include <stdlib.h>

const int SunWorth = 10;

Texture2D SUN_TEXTURE;

const char *SUN_PATH = "Sprites/Sun.png";
const char *SUN_COLLECT_SOUND_PATH = "Sounds/Sun/";
const float SUN_SCALE = 1.5;

const float SUN_FALLING_VEL = 300;
const float SUN_MOVING_VEL = 2500;

const int SUN_FRAME_WIDTH = 400;
const int SUN_FRAME_HEIGHT = 400;
const int SUN_MAX_FRAMES = 13;
const float SUN_FRAME_TIME = FRAME_TIME;

State SUN_IDLE = {
    SUN_FRAME_WIDTH,
    SUN_FRAME_HEIGHT,
    SUN_MAX_FRAMES,
    SUN_FRAME_TIME,
    &SUN_TEXTURE};

void Sun_Init() {
    SUN_TEXTURE = LoadTexture(SUN_PATH);
}

Sun *newSun(Vector2 dest) {
    Sun *new = (Sun *)malloc(sizeof(Sun));
    const int offsetLimit = 300;
    float xOffset = rand() % offsetLimit - offsetLimit / 2;
    float yOffset = rand() % offsetLimit - offsetLimit / 2;
    dest.x = xOffset + dest.x;
    dest.y = yOffset + dest.y;
    new->dest.x = dest.x;
    new->dest.y = dest.y;
    new->pos.x = dest.x;
    new->pos.y = -GetCellDimensions().y;
    new->vel.x = 0;
    new->vel.y = 0;
    new->state = &SUN_IDLE;
    new->frameIndex = 0;
    new->alpha = 0;
    new->frameTime = 0;
    new->update = Sun_Update;
    new->draw = Sun_Draw;
    new->isFalling = true;
    new->isClicked = false;
    return new;
}

void Sun_Draw(Sun *self) {
    int index = FindObjectIndex(self, false);
    Vector2 offset = {-100, -100};
    DrawObject(Objects[index], SUN_SCALE, offset, WHITE);
}

void Sun_Update(Sun *self) {
    float xDistance = fabs(self->pos.x - self->dest.x);
    float yDistance = fabs(self->pos.y - self->dest.y);
    if (self->isFalling) {
        if (yDistance < POSITION_TOLERANCE.y)
            self->isFalling = false;
        self->vel.y = SUN_FALLING_VEL;
    } else {
        self->vel.y = 0;
    }
    if (self->isClicked) {
        if (xDistance < POSITION_TOLERANCE.x && yDistance < POSITION_TOLERANCE.y) {
            SunCount += SunWorth;
            RemoveObject(self, false);
        }
        float d = sqrt(xDistance * xDistance + yDistance * yDistance);
        self->vel.x = SUN_MOVING_VEL * xDistance / d;
        self->vel.y = SUN_MOVING_VEL * yDistance / d;
    }
    float dt = GetFrameTime();
    self->pos.x += self->vel.x * dt;
    self->pos.y += self->vel.y * dt;
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        Vector2 center = {self->pos.x, self->pos.y};
        float frameWidth = (*self->state).frameWidth;
        float frameHeight = (*self->state).frameHeight;
        float radius = fmax(frameHeight, frameWidth);
        Vector2 mousePosition = GetMousePosition();
        if (IsPositionInsideCircle(center, radius / SUN_SCALE / 2, mousePosition) &&
            self->isClicked == false) {
            self->isClicked = true;
            self->dest.x = X_OFFSET + (GetScreenWidth() - X_OFFSET) / 2;
            self->dest.y = GetScreenHeight() - 250;
            PlayRandomOggWithPitch(SUN_COLLECT_SOUND_PATH, 1);
        }
    }
}