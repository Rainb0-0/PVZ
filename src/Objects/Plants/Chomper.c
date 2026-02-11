#include "Chomper.h"
#include "Game.h"
#include "GameGrid.h"
#include "Object.h"
#include "Plant.h"
#include "State.h"
#include "raylib.h"
#include <stdlib.h>

Texture2D CHOMPER_IDLE_TEXTURE;

const char *CHOMPER_IDLE_TEXTURE_PATH = "Sprites/Chomper.png";

const float CHOMPER_HP = 100;
const float CHOMPER_LIFE = 20;
const Vector2 VEL_COEF = {1.0 / 3, 1.0 / 3};

const int CHOMPER_IDLE_FRAME_WIDTH = 101;
const int CHOMPER_IDLE_FRAME_HEIGHT = 105;
const int CHOMPER_IDLE_MAX_FRAMES = 122;
const float CHOMPER_IDLE_FRAME_TIME = 0.01;

State CHOMPER_IDLE = {
    CHOMPER_IDLE_FRAME_WIDTH,
    CHOMPER_IDLE_FRAME_HEIGHT,
    CHOMPER_IDLE_MAX_FRAMES,
    1,
    CHOMPER_IDLE_FRAME_TIME,
    &CHOMPER_IDLE_TEXTURE,
};

Plant *newChomperPlant(Chomper *self) {
    Plant *nc = (Plant *)malloc(sizeof(Plant));
    nc->hp = &self->hp;
    nc->MAXHP = CHOMPER_HP;
    nc->type = CHOMPER;
    nc->self = self;
    return nc;
}

Chomper *newChomper(Vector2 pos) {
    Chomper *c = (Chomper *)malloc(sizeof(Chomper));
    chompers[GetRowIndex(pos.y)]++;
    c->pos.x = pos.x;
    c->pos.y = pos.y;
    c->draw = Chomper_Draw;
    c->update = Chomper_Update;
    c->state = &CHOMPER_IDLE;
    c->frameIndex = 0;
    c->frameTime = 0;
    c->timeAlive = 0;
    c->hp = CHOMPER_HP;
    return c;
}

void Chomper_Init() {
    CHOMPER_IDLE_TEXTURE = LoadTexture(CHOMPER_IDLE_TEXTURE_PATH);
}
void Chomper_Draw(Chomper *self) {
    int index = FindObjectIndex(self, true);
    Vector2 offset = {0, 0};
    DrawObject(Objects[index], 0.9, offset, WHITE);
}
void Chomper_Update(Chomper *self) {
    if (CHOMPER_LIFE < self->timeAlive || self->hp <= 0) {
        chompers[GetRowIndex(self->pos.y)]--;
        RemoveObject(self, true);
        return;
    }
    self->timeAlive += GetFrameTime();
}
