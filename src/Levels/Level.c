#include "Level.h"
#include "Game.h"
#include "GameGrid.h"
#include "Object.h"
#include "Zombie.h"
#include "raylib.h"
#include <stdlib.h>
#include <time.h>

Texture2D DAY_TEXTURE;

const char *DAY_PATH = "Sprites/Levels/day.png";

// TODO add sun decay

const float SUN_SPAWN_COOLDOWN = 5;
const float ZOMBIE_SPAWN_COOLDOWN = 5;

Level *currentLevel;

// texture, init sun, infinite?,
// normal, flag, cooldown, lawnmower, naturla sun, name
Level LEVEL1 = {
    &DAY_TEXTURE, 50, false, 30, 0, ZOMBIE_SPAWN_COOLDOWN, true, true, "DAY"};

// level2 bg

float sinceSunSpawn;
float sinceZombieSpawn;
int normalZombiesSpawned;
int flagZombiesSpawned;
int zombiesKilled;

void SpawnZombie(bool flag) {
    int row = rand() % GRID_ROWS;
    float yOffset = GetPlayfieldRect().y;
    Vector2 cellDim = GetCellDimensions();
    Vector2 pos = {
        GetScreenWidth() - 100,
        row * cellDim.y + yOffset + cellDim.y / 2,
    };
    Zombie *nz = newZombie(pos, flag);
    nz->active = true;
    Object *zo = newZombieObject(nz);
    if (flag) {
        flagZombiesSpawned++;
    } else {
        normalZombiesSpawned++;
    }
    AddObject(zo);
    sinceZombieSpawn = 0;
}

void SpawnLawnMowers() {
    float xOffset = GetPlayfieldRect().x;
    float yOffset = GetPlayfieldRect().y;
    float cellHeight = GetCellDimensions().y;
    for (int i = 0; i < GRID_ROWS; i++) {
        if (lawnMowers[i] == true)
            continue;
        Vector2 pos = {xOffset, i * cellHeight + cellHeight / 2 + yOffset};
        LawnMower *nl = newLawnMower(pos);
        Object *lo = newLawnMowerObject(nl);
        lawnMowers[GetRowIndex(pos.y)] = 1;
        AddObject(lo);
    }
}

void Level_Init() {
    SunCount = currentLevel->initSunCount;
    if (currentLevel->lawnMowersActive) {
        SpawnLawnMowers();
    }
    calcluateWeight;
}

void Level_Draw() {
    float sw = GetScreenWidth();
    float sh = GetScreenHeight();
    float aspectRatio = sw / sh;
    float height = currentLevel->background->height * 0.95;
    float width = height * aspectRatio;
    Rectangle src = {190, 20, width, height};
    Rectangle dst = {0, 0, sw, sh};
    Vector2 origin = {0, 0};
    DrawTexturePro(*(currentLevel->background),
                   src, dst, origin, 0, WHITE);
}

void SpawnSun() {
    srand(time(NULL));
    float xOffset = GetPlayfieldRect().x;
    float yOffset = GetPlayfieldRect().y;
    Vector2 cellDim = GetCellDimensions();
    int x = rand() % GRID_COLS;
    int y = rand() % GRID_ROWS;
    Vector2 pos = {x * cellDim.x + xOffset + cellDim.x / 2,
                   y * cellDim.y + yOffset + cellDim.y / 2};
    Sun *s = newSun(pos);
    Object *so = newSunObject(s);
    AddObject(so);
}

void Level_Update() {
    float dt = GetFrameTime();
    if (currentLevel->naturalSunSpawns &&
        SUN_SPAWN_COOLDOWN <= sinceSunSpawn) {
        SpawnSun();
        sinceSunSpawn = 0;
    }
    if (currentLevel->spawnCooldown < sinceZombieSpawn)
        if (currentLevel->infiniteZombies ||
            (normalZombiesSpawned + flagZombiesSpawned <
             currentLevel->normalZombieCount + currentLevel->flagZombieCount)) {
            if (normalZombiesSpawned < currentLevel->normalZombieCount) {
                if (flagZombiesSpawned < currentLevel->flagZombieCount) {
                    // both types can spawn
                    int chance = rand() % 2;
                    SpawnZombie(chance);
                } else {
                    // only normal
                    SpawnZombie(false);
                }
            } else {
                // only flag
                SpawnZombie(true);
            }
        }
    sinceSunSpawn += dt;
    sinceZombieSpawn += dt;
}

void Level_Destroy() {
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
    sinceSunSpawn = 0;
    sinceZombieSpawn = 0;
    normalZombiesSpawned = 0;
    flagZombiesSpawned = 0;
    zombiesKilled = 0;
}

// DEBUG
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