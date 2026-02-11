#include "Game.h"
#include "Chomper.h"
#include "Coin.h"
#include "Font.h"
#include "GameGrid.h"
#include "LawnMower.h"
#include "Level.h"
#include "MainMenu.h"
#include "Object.h"
#include "Pea.h"
#include "Peashooter.h"
#include "Plant.h"
#include "PlantSelection.h"
#include "SceneManager.h"
#include "Sound.h"
#include "Sun.h"
#include "Sunflower.h"
#include "Zombie.h"
#include "raylib.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int SunCount = 100;

const Vector2 POSITION_TOLERANCE = {10, 10};

float weights[GRID_ROWS];
bool lawnMowers[GRID_ROWS];
int chompers[GRID_ROWS];

const char *GAME_START_SOUND = "Sounds/Level/Start/";
const char *GAME_WON_SOUND = "Sounds/Level/Win/";
const char *GAME_LOST_SOUND = "Sounds/Level/Lose/";

bool GamePaused = false;

float calculateDet(float y) {
    int n = 0;
    float S = 0;
    float Q = 0;
    int row = GetRowIndex(y);
    for (int i = 0; i < ObjectsCount; i++) {
        if (Objects[i] == NULL)
            continue;
        if (Objects[i]->type == PLANT &&
            GetRowIndex(Objects[i]->pos->y) == row) {
            Plant *cur = (Plant *)Objects[i]->self;
            if (0 < *cur->hp &&
                cur->type != SUNFLOWER && cur->type != MARIGOLD) {
                n++;
                float temp = *cur->hp / cur->MAXHP;
                S += temp;
                Q += temp * temp;
            }
        }
    }
    return (n * n + S * S + Q * Q + 1);
}

void calcluateWeight(float y) {
    int row = GetRowIndex(y);
    weights[row] = 10 * lawnMowers[row] + calculateDet(y);
}

Zombie *ZombieAtPosition(Vector2 pos) {
    for (int i = 0; i < ObjectsCount; i++) {
        if (Objects[i] == NULL)
            continue;
        if (Objects[i]->type == ZOMBIE) {
            Zombie *current = (Zombie *)Objects[i]->self;
            float xDistance = fabs(current->pos.x - pos.x);
            float yDistance = fabs(current->pos.y - pos.y);
            if (xDistance < POSITION_TOLERANCE.x &&
                yDistance < POSITION_TOLERANCE.y &&
                0 < current->hp)
                return current;
        }
    }
    return NULL;
}

bool IsZombieInLaneOfPosition(Vector2 pos) {
    Vector2 snappedPos = SnapPosToGrid(pos);
    for (int i = 0; i < ObjectsCount; i++) {
        if (Objects[i] == NULL)
            continue;
        if (Objects[i]->type == ZOMBIE) {
            Zombie *current = (Zombie *)(Objects[i]->self);
            float xDistance = current->pos.x - snappedPos.x;
            if (SnapPosToGrid(current->pos).y == snappedPos.y &&
                0 < current->hp && 0 < xDistance < POSITION_TOLERANCE.x &&
                current->pos.x < X_OFFSET && current->active) {
                return true;
            }
        }
    }
    return false;
}

Plant *PlantAtPosition(Vector2 pos) {
    for (int i = 0; i < ObjectsCount; i++) {
        if (Objects[i] == NULL)
            continue;
        if (Objects[i]->type == PLANT) {
            float xDistance = fabs(Objects[i]->pos->x - pos.x);
            float yDistance = fabs(Objects[i]->pos->y - pos.y);
            if (xDistance < POSITION_TOLERANCE.x * 4 &&
                yDistance < POSITION_TOLERANCE.y)
                return (Plant *)Objects[i]->self;
        }
    }
    return NULL;
}

bool IsPositionInsideRect(Rectangle rect, Vector2 pos) {
    if (pos.x < rect.x)
        return false;
    if (rect.x + rect.width < pos.x)
        return false;
    if (pos.y < rect.y)
        return false;
    if (rect.y + rect.height < pos.y)
        return false;
    return true;
}

bool IsPositionInsideCircle(Vector2 center, float radius, Vector2 pos) {
    float xDistance = fabs(pos.x - center.x);
    float yDistance = fabs(pos.y - center.y);
    float distance = sqrt(xDistance * xDistance + yDistance * yDistance);
    if (distance <= radius)
        return true;
    return false;
}

void DamagePlant(Plant *plant, float damage) {
    if (*plant->hp < damage) {
        *plant->hp = 0;
    } else {
        *plant->hp -= damage;
    }
    int index = FindObjectIndex(plant, false);
    calcluateWeight(Objects[index]->pos->y);
}

void KillZombiesInCircle(Vector2 center, float radius) {
    for (int i = 0; i < ObjectsCount; i++) {
        if (Objects[i] == NULL)
            continue;
        if (Objects[i]->type == ZOMBIE) {
            Zombie *current = (Zombie *)Objects[i]->self;
            float dx = current->pos.x - center.x;
            float dy = current->pos.y - center.y;
            float distance = sqrt(dx * dx + dy * dy);
            if (distance <= radius) {
                current->hp = -1;
            }
        }
    }
}

void Game_End() {
    // TODO implement game end functionality
}

void Game_Init() {
    Level_Init();
    PlantSelection_Init();
    for (int i = 0; i < GRID_ROWS; i++) {
        float y = GetPlayfieldRect().y +
                  i * GetCellDimensions().y + GetCellDimensions().y / 2;
        calcluateWeight(y);
    }
}

void Game_Draw() {
    Level_Draw();
    for (int i = 0; i < ObjectsCount; i++) {
        if (Objects[i] == NULL)
            continue;
        void *self;
        if (Objects[i]->type != PLANT)
            self = Objects[i]->self;
        else {
            Plant *temp = (Plant *)Objects[i]->self;
            self = temp->self;
        }
        if (self == NULL)
            CloseWindow();
        Objects[i]->draw(self);
    }
    // for (int i = 0; i < GRID_ROWS; i++) {
    //     float y = GetPlayfieldRect().y +
    //               i * GetCellDimensions().y + GetCellDimensions().y / 2;
    //     char text[100];
    //     sprintf(text, "weight: %.2f\n chomper: %d\n lawnmower: %d",
    //             weights[i], chompers[i], lawnMowers[i]);
    //     DrawText(text, 200, y, 50, BLACK);
    // }
    PlantSelection_Draw();
}

void Game_Update() {
    Level_Update();
    for (int i = 0; i < ObjectsCount; i++) {
        if (Objects[i] == NULL)
            continue;
        void *self;
        if (Objects[i]->type != PLANT)
            self = Objects[i]->self;
        else {
            Plant *temp = (Plant *)Objects[i]->self;
            self = temp->self;
        }
        Objects[i]->update(self);
    }
    PlantSelection_Update();
}