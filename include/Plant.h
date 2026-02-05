#ifndef PLANT_H
#define PLANT_H

#include "raylib.h"

typedef enum PlantType {
    PEASHOOTER,
    SUNFLOWER,
    CHOMPER,
    POTATO,
    MARIGOLD, // TODO : CHANGE WITH SOMETHING ELSE
    PLANTCOUNT
} PlantType;

typedef struct Plant {
    PlantType type;
    float *hp;
    float MAXHP;
    void *self;
} Plant;

#endif