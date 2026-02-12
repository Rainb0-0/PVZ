#include "Potato.h"
#include "Game.h"
#include "Object.h"
#include "Plant.h"
#include "Sound.h"
#include "State.h"
#include "raylib.h"
#include "stdlib.h"

Texture2D POTATO_SLEEP_TEXTURE;
Texture2D POTATO_AWAKE_TEXTURE;
Texture2D POTATO_EXPLODED_TEXTURE;

const char *POTATO_SLEEP_PATH = "Sprites/Potato/sleep.png";
const char *POTATO_AWAKE_PATH = "Sprites/Potato/awake.png";
const char *POTATO_EXPLODED_PATH = "Sprites/Potato/exploded.png";
const char *POTATO_GROW_SOUND_PATH = "Sounds/Potato/Grow/";
const char *POTATO_EXPLODE_SOUND_PATH = "Sounds/Potato/Explode/";

const int POTATO_SLEEP_FRAME_WIDTH = 96;
const int POTATO_SLEEP_FRAME_HEIGHT = 78;
const int POTATO_SLEEP_MAX_FRAMES = 20;
const float POTATO_SLEEP_FRAME_TIME = FRAME_TIME;

const int POTATO_AWAKE_FRAME_WIDTH = 96;
const int POTATO_AWAKE_FRAME_HEIGHT = 78;
const int POTATO_AWAKE_MAX_FRAMES = 26;
const float POTATO_AWAKE_FRAME_TIME = FRAME_TIME;

const int POTATO_EXPLODED_FRAME_WIDTH = 132;
const int POTATO_EXPLODED_FRAME_HEIGHT = 93;
const int POTATO_EXPLODED_MAX_FRAMES = 1;
const float POTATO_EXPLODED_FRAME_TIME = FRAME_TIME;
const float POTATO_EXPLODED_DURATION = 2;

const float POTATO_HP = 300;
const float POTATO_COOLDOWN = 15;
const float BLAST_RADIUS = 70;

State POTATO_SLEEP = {
    POTATO_SLEEP_FRAME_WIDTH,
    POTATO_SLEEP_FRAME_HEIGHT,
    1,
    1,
    POTATO_SLEEP_FRAME_TIME,
    &POTATO_SLEEP_TEXTURE,
};

State POTATO_ACTIVATION = {
    POTATO_SLEEP_FRAME_WIDTH,
    POTATO_SLEEP_FRAME_HEIGHT,
    POTATO_SLEEP_MAX_FRAMES,
    1,
    POTATO_SLEEP_FRAME_TIME,
    &POTATO_SLEEP_TEXTURE,
};

State POTATO_AWAKE = {
    POTATO_AWAKE_FRAME_WIDTH,
    POTATO_AWAKE_FRAME_HEIGHT,
    POTATO_AWAKE_MAX_FRAMES,
    1,
    POTATO_AWAKE_FRAME_TIME,
    &POTATO_AWAKE_TEXTURE,
};

State POTATO_EXPLODED = {
    POTATO_EXPLODED_FRAME_WIDTH,
    POTATO_EXPLODED_FRAME_HEIGHT,
    POTATO_EXPLODED_MAX_FRAMES,
    1,
    POTATO_EXPLODED_FRAME_TIME,
    &POTATO_EXPLODED_TEXTURE,
};

Plant *
newPotatoPlant(Potato *self) {
    Plant *pp = (Plant *)malloc(sizeof(Plant));
    pp->hp = &self->hp;
    pp->MAXHP = self->hp;
    pp->type = POTATO;
    pp->self = self;
    return pp;
}

Potato *newPotato(Vector2 pos) {
    Potato *p = (Potato *)malloc(sizeof(Potato));
    p->pos.x = pos.x;
    p->pos.y = pos.y;
    p->activationCooldown = POTATO_COOLDOWN;
    p->active = false;
    p->frameIndex = 0;
    p->frameTime = 0;
    p->hp = POTATO_HP;
    p->sincePlant = 0;
    p->state = &POTATO_SLEEP;
    p->draw = Potato_Draw;
    p->update = Potato_Update;
    return p;
}

void Potato_Init() {
    if (!IsTextureValid(POTATO_AWAKE_TEXTURE)) {
        POTATO_AWAKE_TEXTURE = LoadTexture(POTATO_AWAKE_PATH);
    }
    if (!IsTextureValid(POTATO_SLEEP_TEXTURE)) {
        POTATO_SLEEP_TEXTURE = LoadTexture(POTATO_SLEEP_PATH);
    }
    if (!IsTextureValid(POTATO_EXPLODED_TEXTURE)) {
        POTATO_EXPLODED_TEXTURE = LoadTexture(POTATO_EXPLODED_PATH);
    }
    CacheAllOgg(POTATO_GROW_SOUND_PATH);
    CacheAllOgg(POTATO_EXPLODE_SOUND_PATH);
}

void Potato_Draw(Potato *self) {
    int index = FindObjectIndex(self, true);
    Vector2 offset = {0, 0};
    // DrawCircle(self->pos.x, self->pos.y, BLAST_RADIUS, RED);
    DrawObject(Objects[index], 0.9, offset, WHITE);
}

void Potato_Update(Potato *self) {
    if (self->hp <= 0) {
        RemoveObject(self, true);
        return;
    }
    float dt = GetFrameTime();
    if (self->state == &POTATO_SLEEP) {
        self->sincePlant += dt;
        if (self->activationCooldown <= self->sincePlant) {
            PlayRandomOgg(POTATO_GROW_SOUND_PATH, 1, true);
            self->state = &POTATO_ACTIVATION;
        }
    } else if (self->state == &POTATO_ACTIVATION) {
        if (self->frameIndex == self->state->maxFrameIndex) {
            self->state = &POTATO_AWAKE;
        }
    } else if (self->state == &POTATO_AWAKE) {
        Vector2 tempPos = {self->pos.x +
                               POSITION_TOLERANCE.x * 4,
                           self->pos.y};
        if (ZombieAtPosition(tempPos)) {
            KillZombiesInCircle(self->pos, BLAST_RADIUS);
            PlayRandomOgg(POTATO_EXPLODE_SOUND_PATH, 1, true);
            self->state = &POTATO_EXPLODED;
        }
    } else {
        self->sincePlant += dt;
        if (self->activationCooldown + POTATO_EXPLODED_DURATION < self->sincePlant) {
            RemoveObject(self, true);
        }
    }
}