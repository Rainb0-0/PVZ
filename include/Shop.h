#ifndef SHOP_H
#define SHOP_H

#include "raylib.h"

typedef struct ShopButton {
    Texture2D *texture;
    int *textureWidth;
    int *textureHeight;
    bool hovered;
    bool *unlocked;
    int price;
    PlantType type;
    Rectangle bounds;
} ShopButton;

void Shop_Init();
void Shop_Draw();
void Shop_Update();

#endif