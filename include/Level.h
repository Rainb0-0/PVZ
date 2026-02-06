#ifndef LEVEL_H
#define LEVEL_H

#include "raylib.h"

#define LEVEL_COUNT 4

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

extern Level *LEVELS[];

extern Level *currentLevel;
extern int zombiesKilled;

void Level_Init();
void Level_Draw();
void Level_Update();

void Level_Destroy();

void Draw_Grid();

#endif