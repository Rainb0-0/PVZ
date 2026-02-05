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

const char *POTATO_SLEEP_PATH = "Sprites/Potato/sleep.png";
const char *POTATO_AWAKE_PATH = "Sprites/Potato/awake.png";
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

const float POTATO_HP = 100;
const float POTATO_COOLDOWN = 10;
const float BLAST_RADIUS = 600;

State POTATO_SLEEP = {
    POTATO_SLEEP_FRAME_WIDTH,
    POTATO_SLEEP_FRAME_HEIGHT,
    1,
    POTATO_SLEEP_FRAME_TIME,
    &POTATO_SLEEP_TEXTURE,
};

State POTATO_ACTIVATION = {
    POTATO_SLEEP_FRAME_WIDTH,
    POTATO_SLEEP_FRAME_HEIGHT,
    POTATO_SLEEP_MAX_FRAMES,
    POTATO_SLEEP_FRAME_TIME,
    &POTATO_SLEEP_TEXTURE,
};

State POTATO_AWAKE = {
    POTATO_AWAKE_FRAME_WIDTH,
    POTATO_AWAKE_FRAME_HEIGHT,
    POTATO_AWAKE_MAX_FRAMES,
    POTATO_AWAKE_FRAME_TIME,
    &POTATO_AWAKE_TEXTURE,
};

Plant *newPotatoPlant(Potato *self) {
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
    POTATO_AWAKE_TEXTURE = LoadTexture(POTATO_AWAKE_PATH);
    POTATO_SLEEP_TEXTURE = LoadTexture(POTATO_SLEEP_PATH);
}

void Potato_Draw(Potato *self) {
    int index = FindObjectIndex(self, true);
    Vector2 offset = {-100, -100};
    // DrawCircle(self->pos.x, self->pos.y, BLAST_RADIUS, RED);
    DrawObject(Objects[index], 3, offset, WHITE);
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
            PlayRandomOggWithPitch(POTATO_GROW_SOUND_PATH, 1);
            self->state = &POTATO_ACTIVATION;
        }
    } else if (self->state == &POTATO_ACTIVATION) {
        if (self->frameIndex == self->state->maxFrameIndex) {
            self->state = &POTATO_AWAKE;
        }
    } else {
        if (ZombieAtPosition(self->pos)) {
            KillZombiesInCircle(self->pos, BLAST_RADIUS);
            // TODO some kind of cloud ?
            PlayRandomOggWithPitch(POTATO_EXPLODE_SOUND_PATH, 1);
            self->hp = 0;
        }
    }
}