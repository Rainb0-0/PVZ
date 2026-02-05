#include "Sunflower.h"
#include "Game.h"
#include "Object.h"
#include "Plant.h"
#include "State.h"
#include "Sun.h"
#include "raylib.h"
#include <stdlib.h>

Texture2D SUNFLOWER_IDLE_TEXTURE;

const int SUNFLOWER_HP = 100;
const float SUNFLOWER_COOLDOWN = 7.5;

const char *SUNFLOWER_IDLE_PATH = "Sprites/Sunflower.png";
const int SUNFLOWER_IDLE_FRAME_WIDTH = 200;
const int SUNFLOWER_IDLE_FRAME_HEIGHT = 200;
const int SUNFLOWER_IDLE_MAX_FRAMES = 24;
const float SUNFLOWER_IDLE_FRAME_TIME = FRAME_TIME;

State SUNFLOWER_IDLE = {
    SUNFLOWER_IDLE_FRAME_WIDTH,
    SUNFLOWER_IDLE_FRAME_HEIGHT,
    SUNFLOWER_IDLE_MAX_FRAMES,
    SUNFLOWER_IDLE_FRAME_TIME,
    &SUNFLOWER_IDLE_TEXTURE,
};

Plant *newSunflowerPlant(Sunflower *self) {
    Plant *sp = (Plant *)malloc(sizeof(Plant));
    sp->hp = &self->hp;
    sp->type = SUNFLOWER;
    sp->MAXHP = self->hp;
    sp->self = self;
    return sp;
}

Sunflower *newSunflower(Vector2 pos) {
    Sunflower *s = (Sunflower *)malloc(sizeof(Sunflower));
    s->draw = Sunflower_Draw;
    s->update = Sunflower_Update;
    s->frameIndex = 0;
    s->frameTime = 0;
    s->hp = SUNFLOWER_HP;
    s->pos.x = pos.x;
    s->pos.y = pos.y;
    s->sinceSun = 0;
    s->state = &SUNFLOWER_IDLE;
    return s;
}

void Sunflower_Init() {
    SUNFLOWER_IDLE_TEXTURE = LoadTexture(SUNFLOWER_IDLE_PATH);
}

void Sunflower_Draw(Sunflower *self) {
    int index = FindObjectIndex(self, true);
    Vector2 offset = {-70, -40};
    DrawObject(Objects[index], 1.6, offset, WHITE);
}

void Sunflower_Update(Sunflower *self) {
    if (self->hp <= 0) {
        RemoveObject(self, true);
        return;
    }
    if (SUNFLOWER_COOLDOWN < self->sinceSun) {
        self->sinceSun = 0;
        Sun *news = newSun(self->pos);
        Object *newso = newSunObject(news);
        AddObject(newso);
    }
    self->sinceSun += GetFrameTime();
}