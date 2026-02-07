#include "Peashooter.h"
#include "Game.h"
#include "Pea.h"
#include "Sound.h"
#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

Texture2D PEASHOOTER_IDLE_TEXTURE;
Texture2D PEASHOOTER_SHOOTING_TEXTURE;
Sound PEASHOOTER_SHOOT_SOUND;

static char *IDLE_PATH = "Sprites/Peashooter/normal.png";
static char *SHOOTING_PATH = "Sprites/Peashooter/shooting.png";
static char *SHOOT_SOUND_PATH = "Sounds/Peashooter/";
const float PEASHOOTER_HP = 100;
const float PEASHOOTER_COOLDOWN = 1;
// IDLE
const int PEASHOOTER_IDLE_FRAME_WIDTH = 80;
const int PEASHOOTER_IDLE_FRAME_HEIGHT = 80;
const int PEASHOOTER_IDLE_MAX_FRAMES = 120;
const float PEASHOOTER_IDLE_FRAME_TIME = 0.01;
// SHOOTING
const int PEASHOOTER_SHOOTING_FRAME_WIDTH = 80;
const int PEASHOOTER_SHOOTING_FRAME_HEIGHT = 80;
const int PEASHOOTER_SHOOTING_MAX_FRAMES = 120;
const float PEASHOOTER_SHOOTING_FRAME_TIME = 0.01;

State PEASHOOTER_IDLE = {
    PEASHOOTER_IDLE_FRAME_WIDTH,
    PEASHOOTER_IDLE_FRAME_HEIGHT,
    PEASHOOTER_IDLE_MAX_FRAMES,
    1,
    PEASHOOTER_IDLE_FRAME_TIME,
    &PEASHOOTER_IDLE_TEXTURE};

State PEASHOOTER_SHOOTING = {
    PEASHOOTER_SHOOTING_FRAME_WIDTH,
    PEASHOOTER_SHOOTING_FRAME_HEIGHT,
    PEASHOOTER_SHOOTING_MAX_FRAMES,
    1,
    PEASHOOTER_SHOOTING_FRAME_TIME,
    &PEASHOOTER_SHOOTING_TEXTURE};

void Peashooter_Init() {
    PEASHOOTER_IDLE_TEXTURE = LoadTexture(IDLE_PATH);
    PEASHOOTER_SHOOTING_TEXTURE = LoadTexture(SHOOTING_PATH);
}

Plant *newPeashooterPlant(Peashooter *self) {
    Plant *pla = (Plant *)malloc(sizeof(Plant));
    pla->hp = &self->hp;
    pla->self = self;
    pla->type = PEASHOOTER;
    pla->MAXHP = self->hp;
    return pla;
}

Peashooter *newPeashooter(Vector2 pos) {
    Peashooter *p =
        (Peashooter *)malloc(sizeof(Peashooter));
    p->hp = PEASHOOTER_HP;
    p->state = &PEASHOOTER_IDLE;
    p->draw = Peashooter_Draw;
    p->update = Peashooter_Update;
    p->frameTime = 0;
    p->sinceShot = 0;
    p->frameIndex = 0;
    p->pos.x = pos.x;
    p->pos.y = pos.y;
    return p;
}

void ChangePeashooterState(Peashooter *self) {
    if (self->state == &PEASHOOTER_IDLE) {
        self->state = &PEASHOOTER_SHOOTING;
    } else {
        self->state = &PEASHOOTER_IDLE;
    }
}

void Peashooter_Shoot(Peashooter *self) {
    Pea *new = newPea(self);
    Object *newpo = newPeaObject(new);
    AddObject(newpo);
}

void Peashooter_Draw(Peashooter *self) {
    int index = FindObjectIndex(self, true);
    Vector2 offset = {-150, -100};
    DrawObject(Objects[index], 4, offset, WHITE);
}

void Peashooter_Update(Peashooter *self) {
    if (self->hp <= 0) {
        RemoveObject(self, true);
        return;
    } else if (self->state == &PEASHOOTER_IDLE) {
        if (
            self->frameIndex == self->state->maxFrameIndex &&
            PEASHOOTER_COOLDOWN <= self->sinceShot) {
            if (isZombieInLaneOfPos(self->pos)) {
                ChangePeashooterState(self);
            }
        }
    } else {
        float diff = fabs((self->state->maxFrameIndex / 2) - self->frameIndex);
        if (diff <= 1) {
            if (PEASHOOTER_COOLDOWN <= self->sinceShot) {
                self->sinceShot = 0;
                Peashooter_Shoot(self);
                PlayRandomOggWithPitch(SHOOT_SOUND_PATH, 0.3);
            }
        } else if (self->frameIndex == self->state->maxFrameIndex) {
            if (!isZombieInLaneOfPos(self->pos) ||
                self->sinceShot < PEASHOOTER_COOLDOWN) {
                ChangePeashooterState(self);
            }
        }
    }
    self->sinceShot += GetFrameTime();
}
