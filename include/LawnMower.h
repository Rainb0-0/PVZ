#ifndef LAWNMOWER_H
#define LAWNMOWER_H

#include "State.h"
#include "raylib.h"

typedef struct LawnMower {
    Vector2 pos;
    bool active;
    Vector2 vel;
    State *state;
    int frameIndex;
    float frameTime;
    void (*draw)(struct LawnMower *self);
    void (*update)(struct LawnMower *self);
} LawnMower;

LawnMower *newLawnMower(Vector2 pos);

void LawnMower_Init();
void LawnMower_Draw(LawnMower *self);
void LawnMower_Update(LawnMower *self);

#endif