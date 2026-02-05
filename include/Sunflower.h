#ifndef SUNFLOWER_H
#define SUNFLOWER_H

#include "Plant.h"
#include "State.h"
#include "raylib.h"

typedef struct Sunflower {
    float hp;
    Vector2 pos;
    int frameIndex;
    float frameTime;
    float sinceSun;
    State *state;
    void (*draw)(struct Sunflower *self);
    void (*update)(struct Sunflower *self);
} Sunflower;

extern Texture2D SUNFLOWER_IDLE_TEXTURE;
extern Texture2D SUNFLOWER_SHOOTING_TEXTURE;

extern const int SUNFLOWER_IDLE_FRAME_WIDTH;
extern const int SUNFLOWER_IDLE_FRAME_HEIGHT;

Plant *newSunflowerPlant(Sunflower *self);
Sunflower *newSunflower(Vector2 pos);

void Sunflower_Init();
void Sunflower_Draw(Sunflower *self);
void Sunflower_Update(Sunflower *self);

#endif