#include "LawnMower.h"
#include "Game.h"
#include "GameGrid.h"
#include "Object.h"
#include "Sound.h"
#include "State.h"
#include "raylib.h"
#include <stdlib.h>

Texture2D LAWNMOWER_TEXTURE;

const char *LAWNMOWER_TEXTURE_PATH = "Sprites/LawnMower.png";
const char *LAWNMOWER_SOUND_PATH = "Sounds/LawnMower/";

const int DAMAGE = 1000;
const Vector2 VEL = {700, 0};

const int LAWNMOWER_FRAME_WIDTH = 92;
const int LAWNMOWER_FRAME_HEIGHT = 75;
const float LAWNMOWER_FRAME_TIME = 0.01;
const int LAWNMOWER_MAX_FRAMES = 48;
State LAWNMOWER_IDLE = {
    LAWNMOWER_FRAME_WIDTH,
    LAWNMOWER_FRAME_HEIGHT,
    1,
    1,
    LAWNMOWER_FRAME_TIME,
    &LAWNMOWER_TEXTURE,
};

State LAWNMOWER_ACTIVE = {
    LAWNMOWER_FRAME_WIDTH,
    LAWNMOWER_FRAME_HEIGHT,
    LAWNMOWER_MAX_FRAMES,
    1,
    LAWNMOWER_FRAME_TIME,
    &LAWNMOWER_TEXTURE,
};

LawnMower *newLawnMower(Vector2 pos) {
    LawnMower *nl = (LawnMower *)malloc(sizeof(LawnMower));
    nl->pos.x = pos.x;
    nl->pos.y = pos.y;
    nl->vel.x = VEL.x;
    nl->vel.y = VEL.y;
    nl->active = false;
    nl->frameIndex = 0;
    nl->frameTime = 0;
    nl->draw = LawnMower_Draw;
    nl->update = LawnMower_Update;
    nl->state = &LAWNMOWER_IDLE;
    return nl;
}

void LawnMower_Init() {
if (!IsTextureValid(LAWNMOWER_TEXTURE)){
    LAWNMOWER_TEXTURE = LoadTexture(LAWNMOWER_TEXTURE_PATH);
}
}

void LawnMower_Draw(LawnMower *self) {
    int index = FindObjectIndex(self, false);
    Vector2 offset = {-45, 0};
    if (self->active)
        offset.y += rand() % 5;
    DrawObject(Objects[index], 1, offset, WHITE);
}

void LawnMower_Update(LawnMower *self) {
    if (GetScreenWidth() < self->pos.x) {
        RemoveObject(self, false);
        calcluateWeight(self->pos.y);
        return;
    }
    Zombie *here = ZombieAtPosition(self->pos);
    if (self->active == false) {
        if (here != NULL) {
            self->active = true;
            PlayRandomOggWithPitch(LAWNMOWER_SOUND_PATH, 1);
            self->state = &LAWNMOWER_ACTIVE;
            lawnMowers[GetRowIndex(self->pos.y)] = 0;
            DamageZombie(here, DAMAGE);
        }
    } else {
        float dt = GetFrameTime();
        self->pos.x += self->vel.x * dt;
        self->pos.y += self->vel.y * dt;
        if (here != NULL) {
            DamageZombie(here, DAMAGE);
        }
    }
}