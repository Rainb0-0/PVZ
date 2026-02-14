#ifndef JALAPENO_H
#define JALAPENO_H

#include "Plant.h"
#include "State.h"
#include "raylib.h"

typedef struct Jalapeno {
    float hp;
    Vector2 pos;
    int frameIndex;
    float frameTime;
    State *state;
    void (*draw)(struct Jalapeno *self);
    void (*update)(struct Jalapeno *self);
} Jalapeno;

extern Texture2D JALAPENO_TEXTURE;
extern const int JALAPENO_FRAME_WIDTH;
extern const int JALAPENO_FRAME_HEIGHT;

Plant *newJalapenoPlant(Jalapeno *self);
Jalapeno *newJalapeno(Vector2 pos);

void Jalapeno_Init();
void Jalapeno_Draw(Jalapeno *self);
void Jalapeno_Update(Jalapeno *self);

#endif