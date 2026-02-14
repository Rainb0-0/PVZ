#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "State.h"
#include "raylib.h"

extern State WALKING;
extern State WALKING_FLAG;
extern State EATING;
extern State EATING_FLAG;
extern State DYING;
extern State DYING_FLAG;

typedef enum ZOMBIE_DEATH_HP {
    NORMAL_DEATH = -100,
    CHARRED_DEATH = -200,
} ZOMBIE_DEATH_HP;

typedef struct Zombie {
    float hp;
    float attackDamage;
    float sinceAttack;
    int frameIndex;
    float frameTime;
    bool active;
    Vector2 vel;
    Vector2 pos;
    State *state;
    bool isFlagZombie;
    float destY;
    void (*draw)(struct Zombie *self);
    void (*update)(struct Zombie *self);
} Zombie;

void Zombie_Init();
Zombie *newZombie(Vector2 pos, bool isFlag);
void ChangeZombieState(Zombie *self);
void DamageZombie(Zombie *self, float damage);
void KillZombie(Zombie *self, ZOMBIE_DEATH_HP hp);

void Zombie_Draw(Zombie *self);
void Zombie_Update(Zombie *self);

#endif