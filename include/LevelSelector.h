#ifndef LEVELSELECTOR_H
#define LEVELSELECTOR_H

#include "Level.h"
#include "raylib.h"

typedef struct LevelSelectorButton {
    Texture2D *thumbnail;
    bool hovered;
    const char *description;
    Level *level;
    Rectangle bounds;
} LevelSelectorButton;

extern float LEVELSTATS[LEVEL_COUNT];

void LevelStats_SaveState();
void LevelStats_ReadFile();

void LevelSelector_Init();
void LevelSelector_Draw();
void LevelSelector_Update();

#endif