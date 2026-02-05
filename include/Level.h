#ifndef LEVEL_H
#define LEVEL_H

#include "raylib.h"

typedef struct Level {
    int normalCount;
    int flagCount;
    float spawnRate;
    bool sunflowerActive;
    Texture2D *background;
} Level;

extern const Level l1;

extern Level currentLevel;
extern int zombiesKilled;

void Level_Init();
void Level_Draw();
void Level_Update();

void Level_Reset();

#endif