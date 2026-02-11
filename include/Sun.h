#ifndef SUN_H
#define SUN_H

#include "State.h"
#include "raylib.h"

extern Texture2D SUN_TEXTURE;

extern const int SUN_FRAME_WIDTH;
extern const int SUN_FRAME_HEIGHT;

typedef struct Sun {
    Vector2 initPos;
    Vector2 pos;
    Vector2 vel;
    Vector2 accel;
    Vector2 dest;
    bool isFalling;
    bool isClicked;
    float alpha;
    State *state;
    int frameIndex;
    float frameTime;
    float sinceLanded;
    void (*draw)(struct Sun *self);
    void (*update)(struct Sun *self);
} Sun;

Sun *newSun(Vector2 dest);

void Sun_Init();

void Sun_Draw(Sun *self);
void Sun_Update(Sun *self);

#endif