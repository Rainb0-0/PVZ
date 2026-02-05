#ifndef STATE_H
#define STATE_H

#include "raylib.h"

typedef struct State {
    int frameWidth;
    int frameHeight;
    int maxFrameIndex;
    float frameTime;
    Texture2D *texture;
} State;

#endif