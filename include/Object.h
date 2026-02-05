#ifndef OBJECT_H
#define OBJECT_H

#include "Chomper.h"
#include "LawnMower.h"
#include "Pea.h"
#include "Peashooter.h"
#include "Potato.h"
#include "State.h"
#include "Sun.h"
#include "Sunflower.h"
#include "Zombie.h"
#include "raylib.h"

typedef enum ObjectType {
    PLANT,
    PEA,
    LAWNMOWER,
    SUN,
    ZOMBIE,
    TYPECOUNT
} ObjectType;

typedef struct Object {
    void (*update)();
    void (*draw)();
    State **state;
    int *frameIndex;
    float *frameTime;
    Vector2 *pos;
    ObjectType type;
    void *self;
} Object;

extern Object *Objects[1000];
extern int ObjectsCount;

void DrawObject(Object *ob, float scale, Vector2 offset, Color tint);
int FindObjectIndex(void *self, bool plant);
void RemoveObject(void *self, bool plant);
void AddObject(Object *ob);

Object *newLawnMowerObject(LawnMower *self);
Object *newZombieObject(Zombie *self);
Object *newPeaObject(Pea *self);
Object *newPotatoObject(Potato *self);
Object *newPeashooterObject(Peashooter *self);
Object *newChomperObject(Chomper *self);
Object *newSunflowerObject(Sunflower *self);
Object *newSunObject(Sun *self);

#endif