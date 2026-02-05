#include "Level.h"
#include "Game.h"
#include "GameGrid.h"
#include "LawnMower.h"
#include "PlantSelection.h"
#include "Shop.h"
#include "Zombie.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static Texture2D LEVEL1;
static Texture2D LEVEL2;
static Texture2D LEVEL3;
static Texture2D LEVEL4;

const float SUN_SPAWN_COOLDOWN = 5;

Level currentLevel;
int currentNormalCount = 0;
int currentFlagCount = 0;
int zombiesKilled = 0;
float sinceLastSpawn = 0;
float sinceLastSun = 0;

const char *LEVEL1_PATH = "Sprites/Levels/1.png";

const Level l1 = {
    0, 40, 2, true, &LEVEL1};

const Level Levels[] = {l1};

void activateAZombie(bool shoudBeFlag) {
    for (int i = 0; i < ObjectsCount; i++) {
        if (Objects[i] == NULL)
            continue;
        if (Objects[i]->type == ZOMBIE) {
            Zombie *cur = (Zombie *)Objects[i]->self;
            if (cur->isFlagZombie == shoudBeFlag &&
                cur->active == false) {
                if (shoudBeFlag)
                    currentFlagCount++;
                else
                    currentNormalCount++;
                cur->active = true;
                return;
            }
        }
    }
}

void spawnLawnMowers() {
    float xOffset = GetPlayfieldRect().x;
    float yOffset = GetPlayfieldRect().y;
    float cellHeight = GetCellDimensions().y;
    for (int i = 0; i < GRID_ROWS; i++) {
        if (lawnMowers[i] == true)
            continue;
        Vector2 pos = {xOffset, i * cellHeight + cellHeight / 2 + yOffset};
        LawnMower *nl = newLawnMower(pos);
        Object *lo = newLawnMowerObject(nl);
        AddObject(lo);
    }
}

void spawnSun() {
    if (SUN_SPAWN_COOLDOWN <= sinceLastSun) {
        float xOffset = GetPlayfieldRect().x;
        float yOffset = GetPlayfieldRect().y;
        sinceLastSun = 0;
        Vector2 cellDim = GetCellDimensions();
        int x = rand() % GRID_COLS;
        int y = rand() % GRID_ROWS;
        Vector2 pos = {x * cellDim.x + xOffset + cellDim.x / 2,
                       y * cellDim.y + yOffset + cellDim.y / 2};
        Sun *s = newSun(pos);
        Object *so = newSunObject(s);
        AddObject(so);
    }
    sinceLastSun += GetFrameTime();
}

void spawnZombie() {
    if (currentLevel.spawnRate < sinceLastSpawn) {
        if (currentNormalCount < currentLevel.normalCount) {
            if (currentFlagCount < currentLevel.flagCount) {
                bool chance = rand() % 2;
                activateAZombie(chance);
            } else {
                activateAZombie(false);
            }
        } else if (currentFlagCount < currentLevel.flagCount) {
            activateAZombie(true);
        }
        sinceLastSpawn = 0;
    }
    sinceLastSpawn += GetFrameTime();
}

void Draw_Grid() {
    Rectangle pf = GetPlayfieldRect();
    float colW = pf.width / GRID_COLS;
    float colH = pf.height / GRID_ROWS;
    for (int i = 0; i <= GRID_COLS; i++) {
        Vector2 start = {pf.x + i * colW, pf.y};
        Vector2 end = {pf.x + i * colW, pf.y + pf.height};
        DrawLineEx(start, end, 5, BLUE);
    }
    for (int i = 0; i <= GRID_ROWS; i++) {
        Vector2 start = {pf.x, pf.y + i * colH};
        Vector2 end = {pf.x + pf.width, pf.y + i * colH};
        DrawLineEx(start, end, 5, BLUE);
    }
}

void Level_Draw() {
    float sw = GetScreenWidth();
    float sh = GetScreenHeight();
    float aspectRatio = sw / sh;
    float height = currentLevel.background->height * 0.95;
    float width = height * aspectRatio;
    Rectangle src = {190, 20, width, height};
    Rectangle dst = {0, 0, sw, sh};
    Vector2 origin = {0, 0};
    DrawTexturePro(*(currentLevel.background),
                   src, dst, origin, 0, WHITE);
}

void Level_Update() {
    spawnZombie();
    spawnSun();
    // TODO implement the new level selection
}

void Level_Init() {
    LEVEL1 = LoadTexture(LEVEL1_PATH);
    srand(time(NULL));
    Rectangle playField = GetPlayfieldRect();
    float colHeight = GetCellDimensions().y;
    for (int i = 0; i < currentLevel.normalCount; i++) {
        float x = WALKING.frameWidth + GetScreenWidth();
        float y = (rand() % 5) * colHeight +
                  playField.y + colHeight / 2;
        Vector2 pos = {x, y};
        Zombie *new = newZombie(pos, false);
        Object *newOb = newZombieObject(new);
        AddObject(newOb);
    }
    for (int i = 0; i < currentLevel.flagCount; i++) {
        float x = WALKING.frameWidth + GetScreenWidth();
        float y = (rand() % 5) * colHeight +
                  playField.y + colHeight / 2;
        Vector2 pos = {x, y};
        Zombie *new = newZombie(pos, true);
        Object *newOb = newZombieObject(new);
        AddObject(newOb);
    }
    spawnLawnMowers();
}

void Level_Reset() {
    for (int i = 0; i < ObjectsCount; i++) {
        if (Objects[i] == NULL)
            continue;
        if (Objects[i]->type == PLANT) {
            Plant *cur = (Plant *)Objects[i]->self;
            free(cur->self);
            free(Objects[i]->self);
            free(Objects[i]);
            Objects[i] = NULL;
        } else {
            free(Objects[i]->self);
            free(Objects[i]);
            Objects[i] = NULL;
        }
    }
    for (int i = 0; i < GRID_ROWS; i++) {
        lawnMowers[i] = 0;
        chompers[i] = 0;
    }
    SunCount = 0;
}