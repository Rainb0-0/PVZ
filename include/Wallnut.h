#ifndef WALLNUT_H
#define WALLNUT_H

#include "Plant.h"
#include "State.h"
#include "raylib.h"

typedef struct Wallnut {
    Vector2 pos;
    float hp;
    int frameIndex;
    float frameTime;
    State *state;
    void (*draw)(struct Wallnut *self);
    void (*update)(struct Wallnut *self);
} Wallnut;

extern Texture2D WALLNUT_NORMAL_TEXTURE;

extern const int WALLNUT_FRAME_WIDTH;
extern const int WALLNUT_FRAME_HEIGHT;

Plant *newWallnutPlant(Wallnut *self);
Wallnut *newWallnut(Vector2 pos);

void Wallnut_Init();
void Wallnut_Draw(Wallnut *self);
void Wallnut_Update(Wallnut *self);

#endif