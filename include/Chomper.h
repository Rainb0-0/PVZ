#ifndef CHOMPER_H
#define CHOMPER_H

#include "Plant.h"
#include "State.h"
#include "raylib.h"

typedef struct Chomper {
    float hp;
    Vector2 pos;
    float timeAlive;
    State *state;
    int frameIndex;
    float frameTime;
    void (*update)(struct Chomper *self);
    void (*draw)(struct Chomper *self);
} Chomper;

extern Texture2D CHOMPER_TEXTURE;

extern const Vector2 VEL_COEF;

extern const int CHOMPER_IDLE_FRAME_WIDTH;
extern const int CHOMPER_IDLE_FRAME_HEIGHT;

Plant *newChomperPlant(Chomper *self);
Chomper *newChomper(Vector2 pos);

void Chomper_Init();
void Chomper_Draw(Chomper *self);
void Chomper_Update(Chomper *self);

#endif