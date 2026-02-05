#ifndef SHOP_H
#define SHOP_H

#include "Plant.h"
#include "raylib.h"

typedef struct ShopButton {
    Texture2D *texture;
    int *textureWidth;
    int *textureHeight;
    bool hovered;
    int price;
    PlantType type;
    Rectangle bounds;
    bool *unlocked;
} ShopButton;

extern int CoinCount;

extern bool IsPlantUnlocked[PLANTCOUNT];

void Shop_SaveState();

void Shop_Init();
void Shop_Draw();
void Shop_Update();

#endif