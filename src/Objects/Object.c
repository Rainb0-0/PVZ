#include "Object.h"
#include "Chomper.h"
#include "Coin.h"
#include "Game.h"
#include "LawnMower.h"
#include "Pea.h"
#include "Peashooter.h"
#include "Potato.h"
#include "Sun.h"
#include "Sunflower.h"
#include "Zombie.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Object *Objects[1000];
int ObjectsCount = 1000;

void DrawObject(Object *ob, float scale, Vector2 offset, Color tint) {
    int global = *ob->frameIndex;
    State *state = *ob->state;
    int height = state->frameHeight;
    int width = state->frameWidth;
    int row = global / (state->maxFrameIndex / state->textureRows);
    int col = global % (state->maxFrameIndex / state->textureRows);
    Rectangle src = {
        col * state->frameWidth,
        row * state->frameHeight,
        state->frameWidth,
        state->frameHeight};

    Rectangle dst = {ob->pos->x - width / 2 + offset.x,
                     ob->pos->y - height / 2 + offset.y,
                     width * scale, height * scale};
    Vector2 origin = {0, 0};
    if (GamePaused == false) {
        *ob->frameIndex %= (*ob->state)->maxFrameIndex;
        if ((*ob->state)->frameTime <= *ob->frameTime) {
            (*ob->frameIndex)++;
            *ob->frameTime = 0;
        }
    }
    *ob->frameTime += GetFrameTime();
    DrawTexturePro(*(*ob->state)->texture, src, dst, origin, 0, tint);
    DrawCircle(ob->pos->x, ob->pos->y, 10, RED);
}

int FindObjectIndex(void *self, bool plant) {
    if (plant) {
        for (int i = 0; i < ObjectsCount; i++) {
            if (Objects[i] == NULL)
                continue;
            if (Objects[i]->type != PLANT)
                continue;
            Plant *cur = (Plant *)Objects[i]->self;
            if (cur->self == self)
                return i;
        }
        return -1;
    } else {
        for (int i = 0; i < ObjectsCount; i++) {
            if (Objects[i] == NULL)
                continue;
            if (Objects[i]->self == self)
                return i;
        }
        return -1;
    }
}

void RemoveObject(void *self, bool plant) {
    int index = FindObjectIndex(self, plant);
    if (index == -1)
        return;
    if (plant) {
        Plant *cur = (Plant *)(Objects[index]->self);
        calcluateWeight(Objects[index]->pos->y);
        free(cur->self);
    }
    Objects[index] = NULL;
    // free(self);
}

void AddObject(Object *ob) {
    // TODO dynamic
    for (int i = 0; i < ObjectsCount; i++) {
        if (Objects[i] == NULL) {
            Objects[i] = ob;
            return;
        }
    }
}

Object *newLawnMowerObject(LawnMower *self) {
    Object *lo = (Object *)malloc(sizeof(Object));
    lo->draw = self->draw;
    lo->update = self->update;
    lo->frameIndex = &self->frameIndex;
    lo->state = &self->state;
    lo->type = LAWNMOWER;
    lo->frameTime = &self->frameTime;
    lo->pos = &self->pos;
    lo->self = self;
    return lo;
}

Object *newZombieObject(Zombie *self) {
    Object *zo = (Object *)malloc(sizeof(Object));
    zo->draw = self->draw;
    zo->update = self->update;
    zo->frameIndex = &self->frameIndex;
    zo->state = &self->state;
    zo->type = ZOMBIE;
    zo->frameTime = &self->frameTime;
    zo->pos = &self->pos;
    zo->self = self;
    return zo;
}

Object *newPeaObject(Pea *self) {
    Object *po = (Object *)malloc(sizeof(Object));
    po->type = PEA;
    po->draw = self->draw;
    po->update = self->update;
    po->frameIndex = &self->frameIndex;
    po->state = &self->state;
    po->frameTime = &self->frameTime;
    po->pos = &self->pos;
    po->self = self;
    return po;
}

Object *newPotatoObject(Potato *self) {
    Object *po = (Object *)malloc(sizeof(Object));
    Plant *pla = newPotatoPlant(self);
    po->draw = self->draw;
    po->update = self->update;
    po->frameIndex = &self->frameIndex;
    po->state = &self->state;
    po->self = pla;
    po->type = PLANT;
    po->frameTime = &self->frameTime;
    po->pos = &self->pos;
    return po;
}

Object *newPeashooterObject(Peashooter *self) {
    Object *po = (Object *)malloc(sizeof(Object));
    Plant *pla = newPeashooterPlant(self);
    po->draw = self->draw;
    po->update = self->update;
    po->frameIndex = &self->frameIndex;
    po->state = &self->state;
    po->self = pla;
    po->type = PLANT;
    po->frameTime = &self->frameTime;
    po->pos = &self->pos;
    return po;
}

Object *newChomperObject(Chomper *self) {
    Object *co = (Object *)malloc(sizeof(Object));
    Plant *pla = newChomperPlant(self);
    co->draw = self->draw;
    co->update = self->update;
    co->frameIndex = &self->frameIndex;
    co->state = &self->state;
    co->self = pla;
    co->type = PLANT;
    co->frameTime = &self->frameTime;
    co->pos = &self->pos;
    return co;
}

Object *newSunflowerObject(Sunflower *self) {
    Object *so = (Object *)malloc(sizeof(Object));
    Plant *pla = newSunflowerPlant(self);
    so->draw = self->draw;
    so->update = self->update;
    so->frameIndex = &self->frameIndex;
    so->state = &self->state;
    so->frameTime = &self->frameTime;
    so->type = PLANT;
    so->self = pla;
    so->pos = &self->pos;
    return so;
}

Object *newSunObject(Sun *self) {
    Object *so = (Object *)malloc(sizeof(Object));
    so->draw = self->draw;
    so->update = self->update;
    so->frameIndex = &self->frameIndex;
    so->state = &self->state;
    so->type = SUN;
    so->frameTime = &self->frameTime;
    so->pos = &self->pos;
    so->self = self;
    return so;
}

Object *newCoinObject(Coin *self) {
    Object *so = (Object *)malloc(sizeof(Object));
    so->draw = self->draw;
    so->update = self->update;
    so->frameIndex = &self->frameIndex;
    so->state = &self->state;
    so->type = COIN;
    so->frameTime = &self->frameTime;
    so->pos = &self->pos;
    so->self = self;
    return so;
}