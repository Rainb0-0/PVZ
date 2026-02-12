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

extern Level LEVEL1;
extern Level LEVEL2;
extern Level LEVEL3;
extern Level LEVEL4;

extern Level *currentLevel;
extern int zombiesKilled;
extern int normalZombiesSpawned;
extern int flagZombiesSpawned;

void Level_Init();
void Level_Draw();
void Level_Update();

void Level_Destroy();

void Draw_Grid();

#endif