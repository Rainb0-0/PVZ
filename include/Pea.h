#ifndef PEA_H
#define PEA_H

#include "Peashooter.h"
#include "State.h"
#include "raylib.h"

typedef struct Pea {
    float attackDamage;
    int frameIndex;
    float frameTime;
    Vector2 vel;
    Vector2 pos;
    State *state;
    void (*draw)(struct Pea *self);
    void (*update)(struct Pea *self);
} Pea;

void Pea_Init();
Pea *newPea(Peashooter *ps);

void Pea_Update(Pea *self);
void Pea_Draw(Pea *self);

#endif