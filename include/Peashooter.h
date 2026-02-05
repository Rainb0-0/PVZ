#ifndef PEASHOOTER_H
#define PEASHOOTER_H

#include "Plant.h"
#include "State.h"
#include "raylib.h"

extern Texture2D PEASHOOTER_IDLE_TEXTURE;
extern Texture2D PEASHOOTER_SHOOTING_TEXTURE;

extern const int PEASHOOTER_IDLE_FRAME_WIDTH;
extern const int PEASHOOTER_IDLE_FRAME_HEIGHT;

typedef struct Peashooter {
    float hp;
    Vector2 pos;
    int frameIndex;
    float frameTime;
    float sinceShot;
    State *state;
    void (*draw)(struct Peashooter *self);
    void (*update)(struct Peashooter *self);
} Peashooter;

void Peashooter_Init();
Plant *newPeashooterPlant(Peashooter *self);
Peashooter *newPeashooter(Vector2 pos);

void Peashooter_Draw(Peashooter *self);
void Peashooter_Update(Peashooter *self);

#endif