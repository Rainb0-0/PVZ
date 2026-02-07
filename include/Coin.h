#ifndef COIN_H
#define COIN_H

#include "State.h"
#include "raylib.h"

typedef struct Coin {
    Vector2 pos;
    Vector2 vel;
    Vector2 accel;
    State *state;
    int frameIndex;
    float frameTime;
    bool isClicked;
    void (*draw)(struct Coin *self);
    void (*update)(struct Coin *self);
} Coin;

Coin *newCoin(Vector2 pos, bool gold);

void Coin_Init();
void Coin_Draw(Coin *self);
void Coin_Update(Coin *self);

#endif