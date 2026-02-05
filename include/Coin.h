#ifndef COIN_H
#define COIN_H

#include "State.h"
#include "raylib.h"

typedef struct Coin {
    Vector2 pos;
    State *state;
    int frameIndex;
    float frameTime;
    void (*draw)(struct Coin *self);
    void (*update)(struct Coin *self);
} Coin;

Coin *newCoin(Vector2 pos);

void Coin_Init();
void Coin_Draw(Coin *self);
void Coin_Update(Coin *self);

#endif