#include "Jalapeno.h"
#include "Game.h"
#include "Object.h"
#include "Plant.h"
#include "Sound.h"
#include "State.h"
#include "raylib.h"
#include <stdlib.h>

Texture2D JALAPENO_TEXTURE;

const char *JALAPENO_PATH = "Sprites/Jalapeno.png";
const char *JALAPENO_EXPLOSION_SOUND_PATH = "Sounds/Jalapeno/";

const float JALAPENO_HP = 5000;

const int JALAPENO_FRAME_WIDTH = 72;
const int JALAPENO_FRAME_HEIGHT = 87;
const float JALAPENO_FRAME_TIME = 0.01;
const int JALAPENO_MAX_FRAMES = 100;

State JALAPENO_IDLE = {
    JALAPENO_FRAME_WIDTH,
    JALAPENO_FRAME_HEIGHT,
    JALAPENO_MAX_FRAMES,
    1,
    JALAPENO_FRAME_TIME,
    &JALAPENO_TEXTURE,
};

Plant *newJalapenoPlant(Jalapeno *self) {
    Plant *jp = (Plant *)malloc(sizeof(Plant));
    jp->self = self;
    jp->hp = &self->hp;
    jp->MAXHP = JALAPENO_HP;
    jp->type = JALAPENO;
    return jp;
}

Jalapeno *newJalapeno(Vector2 pos) {
    Jalapeno *nj = (Jalapeno *)malloc(sizeof(Jalapeno));
    nj->pos = pos;
    nj->hp = JALAPENO_HP;
    nj->frameTime = 0;
    nj->frameIndex = 0;
    nj->state = &JALAPENO_IDLE;
    nj->update = Jalapeno_Update;
    nj->draw = Jalapeno_Draw;
    return nj;
}

void Jalapeno_Init() {
    if (!IsTextureValid(JALAPENO_TEXTURE)) {
        JALAPENO_TEXTURE = LoadTexture(JALAPENO_PATH);
    }
    CacheAllOgg(JALAPENO_EXPLOSION_SOUND_PATH);
}
void Jalapeno_Draw(Jalapeno *self) {
    int index = FindObjectIndex(self, true);
    Vector2 offset = {0, 0};
    DrawObject(Objects[index], 1, offset, WHITE);
};
void Jalapeno_Update(Jalapeno *self) {
    if (self->frameIndex == self->state->maxFrameIndex - 1) {
        KillZombiesInRow(self->pos);
        PlayRandomOgg(JALAPENO_EXPLOSION_SOUND_PATH, 1, true);
        RemoveObject(self, true);
        return;
    }
};