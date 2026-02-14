#ifndef ANIMATION_H
#define ANIMATION_H

#include "State.h"
#include "raylib.h"

typedef struct Animation {
    State *state;
    Vector2 pos;
    float scale;
    Vector2 offset;
    int frameIndex;
    float frameTime;
    void (*draw)(struct Animation *self);
    void (*update)(struct Animation *self);
} Animation;

Animation *newAnimation(State *self, Vector2 pos, Vector2 offset, float scale);
void Animation_Draw(Animation *self);
void Animation_Update(Animation *self);

#endif