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

void LevelSelector_Init();
void LevelSelector_Draw();
void LevelSelector_Update();

#endif