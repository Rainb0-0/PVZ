#include "Wallnut.h"
#include "Object.h"
#include "Plant.h"
#include "State.h"
#include "raylib.h"
#include <math.h>
#include <stdlib.h>

Texture2D WALLNUT_NORMAL_TEXTURE;
Texture2D WALLNUT_DAMAGED1_TEXTURE;
Texture2D WALLNUT_DAMAGED2_TEXTURE;

const char *WALLNUT_NORMAL_PATH = "Sprites/Wallnut/normal.png";
const char *WALLNUT_DAMAGED1_PATH = "Sprites/Wallnut/damage1.png";
const char *WALLNUT_DAMAGED2_PATH = "Sprites/Wallnut/damage2.png";

const float WALLNUT_HP = 1500;

const int WALLNUT_FRAME_WIDTH = 120;
const int WALLNUT_FRAME_HEIGHT = 120;
const int WALLNUT_MAX_FRAMES = 1;
const float WALLNUT_FRAME_TIME = 1;

State WALLNUT_NORMAL = {
    WALLNUT_FRAME_WIDTH,
    WALLNUT_FRAME_HEIGHT,
    WALLNUT_MAX_FRAMES,
    1,
    WALLNUT_FRAME_TIME,
    &WALLNUT_NORMAL_TEXTURE,
};
State WALLNUT_DAMAGED1 = {
    WALLNUT_FRAME_WIDTH,
    WALLNUT_FRAME_HEIGHT,
    WALLNUT_MAX_FRAMES,
    1,
    WALLNUT_FRAME_TIME,
    &WALLNUT_DAMAGED1_TEXTURE,
};
State WALLNUT_DAMAGED2 = {
    WALLNUT_FRAME_WIDTH,
    WALLNUT_FRAME_HEIGHT,
    WALLNUT_MAX_FRAMES,
    1,
    WALLNUT_FRAME_TIME,
    &WALLNUT_DAMAGED2_TEXTURE,
};

Plant *newWallnutPlant(Wallnut *self) {
    Plant *np = (Plant *)malloc(sizeof(Plant));
    np->self = self;
    np->MAXHP = WALLNUT_HP;
    np->type = WALLNUT;
    np->hp = &self->hp;
    return np;
}

Wallnut *newWallnut(Vector2 pos) {
    Wallnut *nw = (Wallnut *)malloc(sizeof(Wallnut));
    nw->pos = pos;
    nw->hp = WALLNUT_HP;
    nw->state = &WALLNUT_NORMAL;
    nw->frameIndex = 0;
    nw->frameTime = 0;
    nw->draw = Wallnut_Draw;
    nw->update = Wallnut_Update;
    return nw;
}

void Wallnut_Init() {
    if (!IsTextureValid(WALLNUT_NORMAL_TEXTURE)) {
        WALLNUT_NORMAL_TEXTURE = LoadTexture(WALLNUT_NORMAL_PATH);
    }
    if (!IsTextureValid(WALLNUT_DAMAGED1_TEXTURE)) {
        WALLNUT_DAMAGED1_TEXTURE = LoadTexture(WALLNUT_DAMAGED1_PATH);
    }
    if (!IsTextureValid(WALLNUT_DAMAGED2_TEXTURE)) {
        WALLNUT_DAMAGED2_TEXTURE = LoadTexture(WALLNUT_DAMAGED2_PATH);
    }
}

void Wallnut_Draw(Wallnut *self) {
    int index = FindObjectIndex(self, true);
    Vector2 offset = {15, 0};
    DrawObject(Objects[index], 0.75, offset, WHITE);
}

void Wallnut_Update(Wallnut *self) {
    if (self->hp <= 0) {
        RemoveObject(self, true);
        return;
    }
    int step = ceilf(((float)self->hp / WALLNUT_HP) * 3); // 1,2,3
    if (step == 1) {
        self->state = &WALLNUT_DAMAGED2;
    } else if (step == 2) {
        self->state = &WALLNUT_DAMAGED1;
    } else {
        self->state = &WALLNUT_NORMAL;
    }
}