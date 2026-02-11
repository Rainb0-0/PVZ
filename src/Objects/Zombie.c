#include "Zombie.h"
#include "Chomper.h"
#include "Coin.h"
#include "Game.h"
#include "Level.h"
#include "Sound.h"
#include "State.h"
#include "raylib.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static Texture2D NORMAL_WALKING;
static Texture2D NORMAL_EATING;
static Texture2D NORMAL_DYING;
static Texture2D FLAG_WALKING;
static Texture2D FLAG_EATING;
static Texture2D FLAG_DYING;

const float ZOMBIE_HP = 100;
const Vector2 ZOMBIE_VEL = {-70, 0};
const float ATTACK_DAMAGE = 10;
const float ATTACK_COOLDOWN = 1;

const char *ZOMBIE_EATING_SOUND_PATH = "Sounds/Zombie/Eating/";
const char *ZOMBIE_GROAN_SOUND_PATH = "Sounds/Zombie/Talking/";
const char *ZOMBIE_DIE_SOUND_PATH = "Sounds/Zombie/Dying/";

// NORMAL
char *NORMAL_WALKING_PATH = "Sprites/NormalZombie/walking.png";
char *NORMAL_EATING_PATH = "Sprites/NormalZombie/eating.png";
char *NORMAL_DYING_PATH = "Sprites/NormalZombie/dying.png";
// FLAG
char *FLAG_WALKING_PATH = "Sprites/FlagZombie/walking.png";
char *FLAG_EATING_PATH = "Sprites/FlagZombie/eating.png";
char *FLAG_DYING_PATH = "Sprites/FlagZombie/dying.png";

// WALKING
const int ZOMBIE_WALKING_FRAME_WIDTH = 148;
const int ZOMBIE_WALKING_FRAME_HEIGHT = 148;
const int ZOMBIE_WALKING_MAX_FRAMES = 230;
const int ZOMBIE_WALKING_TEXTURE_ROWS = 23;
const float ZOMBIE_WALKING_FRAME_TIME = 0.01;
// EATING
const int ZOMBIE_EATING_FRAME_WIDTH = 148;
const int ZOMBIE_EATING_FRAME_HEIGHT = 148;
const int ZOMBIE_EATING_MAX_FRAMES = 196;
const int ZOMBIE_EATING_TEXTURE_ROWS = 49;
const float ZOMBIE_EATING_FRAME_TIME = 0.01;
// DYING
const int ZOMBIE_DYING_FRAME_WIDTH = 148;
const int ZOMBIE_DYING_FRAME_HEIGHT = 148;
const int ZOMBIE_DYING_MAX_FRAMES = 165;
const int ZOMBIE_DYING_TEXTURE_ROWS = 11;
const float ZOMBIE_DYING_FRAME_TIME = 0.01;

// WALKING
State WALKING = {
    ZOMBIE_WALKING_FRAME_WIDTH,
    ZOMBIE_WALKING_FRAME_HEIGHT,
    ZOMBIE_WALKING_MAX_FRAMES,
    ZOMBIE_WALKING_TEXTURE_ROWS,
    ZOMBIE_WALKING_FRAME_TIME,
    &NORMAL_WALKING};
State WALKING_FLAG = {
    ZOMBIE_WALKING_FRAME_WIDTH,
    ZOMBIE_WALKING_FRAME_HEIGHT,
    ZOMBIE_WALKING_MAX_FRAMES,
    ZOMBIE_WALKING_TEXTURE_ROWS,
    ZOMBIE_WALKING_FRAME_TIME,
    &FLAG_WALKING};
// EATING
State EATING = {
    ZOMBIE_EATING_FRAME_WIDTH,
    ZOMBIE_EATING_FRAME_HEIGHT,
    ZOMBIE_EATING_MAX_FRAMES,
    ZOMBIE_EATING_TEXTURE_ROWS,
    ZOMBIE_EATING_FRAME_TIME,
    &NORMAL_EATING};
State EATING_FLAG = {
    ZOMBIE_EATING_FRAME_WIDTH,
    ZOMBIE_EATING_FRAME_HEIGHT,
    ZOMBIE_EATING_MAX_FRAMES,
    ZOMBIE_EATING_TEXTURE_ROWS,
    ZOMBIE_EATING_FRAME_TIME,
    &FLAG_EATING};
// DYING
State DYING = {
    ZOMBIE_DYING_FRAME_WIDTH,
    ZOMBIE_DYING_FRAME_HEIGHT,
    ZOMBIE_DYING_MAX_FRAMES,
    ZOMBIE_DYING_TEXTURE_ROWS,
    ZOMBIE_DYING_FRAME_TIME,
    &NORMAL_DYING};
State DYING_FLAG = {
    ZOMBIE_DYING_FRAME_WIDTH,
    ZOMBIE_DYING_FRAME_HEIGHT,
    ZOMBIE_DYING_MAX_FRAMES,
    ZOMBIE_DYING_TEXTURE_ROWS,
    ZOMBIE_DYING_FRAME_TIME,
    &FLAG_DYING};

Zombie *newZombie(Vector2 pos, bool isFlag) {
    Zombie *z = (Zombie *)malloc(sizeof(Zombie));
    z->hp = ZOMBIE_HP;
    z->vel = ZOMBIE_VEL;
    z->frameIndex = 0;
    z->isFlagZombie = isFlag;
    z->draw = Zombie_Draw;
    z->update = Zombie_Update;
    z->attackDamage = ATTACK_DAMAGE;
    z->sinceAttack = 0;
    z->frameTime = 0;
    z->pos.x = pos.x;
    z->pos.y = pos.y;
    z->active = false;
    z->destY = pos.y;
    if (isFlag) {
        z->state = &WALKING_FLAG;
    } else {
        z->state = &WALKING;
    }
    return z;
}

void ChangeZombieState(Zombie *self) {
    self->frameIndex = 0;
    if (self->isFlagZombie) {
        if (self->hp <= 0) {
            self->state = &DYING_FLAG;
        } else if (self->state == &WALKING_FLAG) {
            self->state = &EATING_FLAG;
        } else {
            self->state = &WALKING_FLAG;
        }
    } else {
        if (self->hp <= 0) {
            self->state = &DYING;
        } else if (self->state == &WALKING) {
            self->state = &EATING;
        } else {
            self->state = &WALKING;
        }
    }
}

Object *findZombie(Zombie *self) {
    for (int i = 0; i < ObjectsCount; i++) {
        if ((Zombie *)Objects[i]->self == self)
            return Objects[i];
    }
}

void DamageZombie(Zombie *self, float damage) {
    self->hp -= damage;
}

void Zombie_Init() {
if (!IsTextureValid(NORMAL_WALKING)){
    NORMAL_WALKING = LoadTexture(NORMAL_WALKING_PATH);
}
if (!IsTextureValid(NORMAL_EATING)){
    NORMAL_EATING = LoadTexture(NORMAL_EATING_PATH);
}
if (!IsTextureValid(NORMAL_DYING)){
    NORMAL_DYING = LoadTexture(NORMAL_DYING_PATH);
}
if (!IsTextureValid(FLAG_WALKING)){
    FLAG_WALKING = LoadTexture(FLAG_WALKING_PATH);
}
if (!IsTextureValid(FLAG_EATING)){
    FLAG_EATING = LoadTexture(FLAG_EATING_PATH);
}
if (!IsTextureValid(FLAG_DYING)){
    FLAG_DYING = LoadTexture(FLAG_DYING_PATH);
}
}

void Zombie_Draw(Zombie *self) {
    if (!self->active)
        return;
    int index = FindObjectIndex(self, false);
    Vector2 offset = {0, 0};
    Color tint = WHITE;
    if (chompers[GetRowIndex(self->pos.y)]) {
        tint = BLUE;
        tint = ColorBrightness(tint, 0.5);
    }
    DrawObject(Objects[index], 1, offset, tint);
}

void Zombie_Update(Zombie *self) {
    if (self->active == false)
        return;
    if (self->hp <= 0 &&
        !(self->state == &DYING || self->state == &DYING_FLAG)) {
        ChangeZombieState(self);
        PlayRandomOggWithPitch(ZOMBIE_DIE_SOUND_PATH, 1);
        int chance = rand() % 2;
        if (chance) {
            Coin *nc = newCoin(self->pos, self->isFlagZombie);
            Object *co = newCoinObject(nc);
            AddObject(co);
        }
    } else if (self->state == &WALKING || self->state == &WALKING_FLAG) {
        if (self->isFlagZombie) {
            int row = GetRowIndex(self->pos.y);
            int minIndex = row;
            if (0 < row) {
                minIndex = (weights[row] <= weights[row - 1]) ? row : row - 1;
            }
            if (row < GRID_ROWS - 1) {
                minIndex = (weights[minIndex] <= weights[row + 1]) ? minIndex : row + 1;
            }
            if (row != minIndex) {
                float cellheight = GetCellDimensions().y;
                float destY = cellheight * minIndex +
                              cellheight / 2 + GetPlayfieldRect().y;
                self->destY = destY;
            }
        }
        if (PlantAtPosition(self->pos)) {
            ChangeZombieState(self);
        }
        if (self->pos.x + self->state->frameWidth < 0) {
            Game_End();
        }
        float dt = GetFrameTime();
        float yDistance = fabs(self->pos.y - self->destY);
        if (POSITION_TOLERANCE.y < yDistance) {
            if (self->pos.y < self->destY) {
                self->vel.y = -self->vel.x;
            } else {
                self->vel.y = self->vel.x;
            }
        } else {
            self->vel.y = 0;
        }
        if (chompers[GetRowIndex(self->pos.y)]) {
            self->pos.x += self->vel.x * VEL_COEF.x * dt;
            self->pos.y += self->vel.y * VEL_COEF.y * dt;
        } else {
            self->pos.x += self->vel.x * dt;
            self->pos.y += self->vel.y * dt;
        }
        float chance = (float)rand() / RAND_MAX;
        if (chance < 0.00005) {
            PlayRandomOggWithPitch(ZOMBIE_GROAN_SOUND_PATH, 0.5);
        }
    } else if (self->state == &EATING || self->state == &EATING_FLAG) {
        Plant *current = PlantAtPosition(self->pos);
        float AC = ATTACK_COOLDOWN;
        if (chompers[GetRowIndex(self->pos.y)]) {
            AC = AC / VEL_COEF.x;
        }
        if (current) {
            if (ATTACK_COOLDOWN <= self->sinceAttack) {
                DamagePlant(current, self->attackDamage);
                PlayRandomOggWithPitch(ZOMBIE_EATING_SOUND_PATH, 1);
                self->sinceAttack = 0;
            }
        } else {
            ChangeZombieState(self);
        }
    } else {
        if (self->frameIndex == self->state->maxFrameIndex - 1) {
            zombiesKilled++;
            RemoveObject(self, false);
        }
    }
    self->sinceAttack += GetFrameTime();
}