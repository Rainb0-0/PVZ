#ifndef PLANTSELECTION_H
#define PLANTSELECTION_H

#include "Object.h"
#include "raylib.h"

typedef struct PlantButton {
    Texture2D *texture;
    int *textureWidth;
    int *textureHeight;
    Object *(*newPlantObject)(void *self);
    void *(*newPlant)(Vector2 pos);
    int price;
    float sinceCooldown;
    bool active;
    bool *unlocked; // for shop
    bool hovered;
    bool selected;
    float maxCooldown;
    Vector2 topLeft;
    Vector2 bottomRight;
} PlantButton;

extern float X_OFFSET;

extern Rectangle SUN_BANK_RECATANGLE;

extern PlantButton *PlantButtons[];
extern const int PlantButtonsSize;

extern Texture2D SEED_PACKET;

extern PlantButton PeashooterButton;
extern PlantButton SunflowerButton;
extern PlantButton ChomperButton;
extern PlantButton PotatoButton;

void PlantSelection_Init();
void PlantSelection_Draw();
void PlantSelection_Update();

#endif