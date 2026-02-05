#ifndef POTATO_H
#define POTATO_H

#include "Plant.h"
#include "State.h"
#include "raylib.h"

typedef struct Potato {
    bool active;
    Vector2 pos;
    float sincePlant;
    float activationCooldown;
    State *state;
    float hp;
    int frameIndex;
    float frameTime;
    void (*draw)(struct Potato *self);
    void (*update)(struct Potato *self);
} Potato;

extern Texture2D POTATO_AWAKE_TEXTURE;
extern const int POTATO_AWAKE_FRAME_WIDTH;
extern const int POTATO_AWAKE_FRAME_HEIGHT;

Plant *newPotatoPlant(Potato *self);
Potato *newPotato(Vector2 pos);
void Potato_Init();
void Potato_Draw(Potato *self);
void Potato_Update(Potato *self);

#endif