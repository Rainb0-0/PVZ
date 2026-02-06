#ifndef LEVEL_H
#define LEVEL_H

#include "raylib.h"

typedef struct Level {
    Texture2D *background;
    int initSunCount;
    bool infiniteZombies;
    int normalZombieCount;
    int flagZombieCount;
    float spawnCooldown;
    bool lawnMowersActive;
    bool naturalSunSpawns;
    char *title;
} Level;

extern Level LEVEL1;

extern Level *currentLevel;
extern int zombiesKilled;

void Level_Init();
void Level_Draw();
void Level_Update();

void Level_Destroy();

#endif