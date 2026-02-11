#include "Pea.h"
#include "Game.h"
#include "Peashooter.h"
#include "Sound.h"
#include "State.h"
#include "Zombie.h"
#include "raylib.h"
#include <stdlib.h>

Texture2D CREATION;
Texture2D NORMAL;
Texture2D DESTRUCTION;

const Vector2 PEA_VELOCITY = {500, 0};
const float PEA_ATTACK_DAMAGE = 10;
const char *CREATION_PATH = "Sprites/Pea/creation.png";
const char *NORMAL_PATH = "Sprites/Pea/normal.png";
const char *DESTRUCTION_PATH = "Sprites/Pea/destruction.png";
const char *PEA_SPLAT_SOUND_PATH = "Sounds/Pea/Destruction/";

// NORMAL
const int PEA_NORMAL_FRAME_WIDTH = 28;
const int PEA_NORMAL_FRAME_HEIGHT = 28;
const int PEA_NORMAL_MAX_FRAMES = 1;
const float PEA_NORMAL_FRAME_TIME = FRAME_TIME;
State PEA_NORMAL = {
    PEA_NORMAL_FRAME_WIDTH,
    PEA_NORMAL_FRAME_HEIGHT,
    PEA_NORMAL_MAX_FRAMES,
    1,
    PEA_NORMAL_FRAME_TIME,
    &NORMAL};
// DESTRUCTION
const int PEA_DESTRUCTION_FRAME_WIDTH = 24;
const int PEA_DESTRUCTION_FRAME_HEIGHT = 24;
const int PEA_DESTRUCTION_MAX_FRAMES = 4;
const float PEA_DESTRUCTION_FRAME_TIME = 0.1;
State PEA_DESTRUCTION = {
    PEA_DESTRUCTION_FRAME_WIDTH,
    PEA_DESTRUCTION_FRAME_HEIGHT,
    PEA_DESTRUCTION_MAX_FRAMES,
    1,
    PEA_DESTRUCTION_FRAME_TIME,
    &DESTRUCTION};

void Pea_Init() {
if (!IsTextureValid(CREATION)){
    CREATION = LoadTexture(CREATION_PATH);
}
if (!IsTextureValid(NORMAL)){
    NORMAL = LoadTexture(NORMAL_PATH);
}
if (!IsTextureValid(DESTRUCTION)){
    DESTRUCTION = LoadTexture(DESTRUCTION_PATH);
}
}

Pea *newPea(Peashooter *ps) {
    Pea *p = (Pea *)malloc(sizeof(Pea));
    float xOffset = +100;
    p->pos.x = ps->pos.x + xOffset;
    p->pos.y = ps->pos.y;
    p->attackDamage = PEA_ATTACK_DAMAGE;
    p->vel.x = PEA_VELOCITY.x;
    p->vel.y = PEA_VELOCITY.y;
    p->frameTime = 0;
    p->draw = Pea_Draw;
    p->update = Pea_Update;
    p->frameIndex = 0;
    p->state = &PEA_NORMAL;
    return p;
}

void ChangePeaState(Pea *self) {
    if (self->state == &PEA_NORMAL) {
        self->state = &PEA_DESTRUCTION;
    }
}

void Pea_Draw(Pea *self) {
    int index = FindObjectIndex(self, false);
    float scale = 3;
    Vector2 offset = {-PEA_NORMAL_FRAME_WIDTH / 2 * scale,
                      -PEA_NORMAL_FRAME_HEIGHT / 2 * scale};
    DrawObject(Objects[index], scale, offset, WHITE);
}

void Pea_Update(Pea *self) {
    if (GetScreenWidth() < self->pos.x) {
        RemoveObject(self, false);
    } else if (self->state == &PEA_DESTRUCTION) {
        if (self->frameIndex == self->state->maxFrameIndex) {
            RemoveObject(self, false);
        }
    } else {
        Zombie *here = ZombieAtPosition(self->pos);
        if (here != NULL) {
            DamageZombie(here, self->attackDamage);
            ChangePeaState(self);
            PlayRandomOggWithPitch(PEA_SPLAT_SOUND_PATH, 1);
        } else {
            float dt = GetFrameTime();
            self->pos.x += self->vel.x * dt;
            self->pos.y += self->vel.y * dt;
        }
    }
}
