#include "Level.h"
#include "Game.h"
#include "GameGrid.h"
#include "Object.h"
#include "Zombie.h"
#include "raylib.h"
#include <stdlib.h>
#include <time.h>

Texture2D DAY_TEXTURE;
Texture2D NIGHT_TEXTURE;

const char *DAY_PATH = "Sprites/Levels/day.png";
const char *NIGHT_PATH = "Sprites/Levels/night.png";

const float SUN_SPAWN_COOLDOWN = 5;
const float ZOMBIE_SPAWN_COOLDOWN = 5;

Level *currentLevel;

Level LEVEL1 = {
    &DAY_TEXTURE,
    100,
    false,
    30,
    0,
    ZOMBIE_SPAWN_COOLDOWN,
    true,
    true,
    "DUMB ZOMBIES",
};

Level LEVEL2 = {
    &DAY_TEXTURE,
    50,
    false,
    0,
    30,
    ZOMBIE_SPAWN_COOLDOWN,
    false,
    true,
    "SMART ZOMBIES",
};

Level LEVEL3 = {
    &NIGHT_TEXTURE,
    1000,
    false,
    15,
    15,
    ZOMBIE_SPAWN_COOLDOWN,
    true,
    false,
    "NIGHT",
};

Level LEVEL4 = {
    &DAY_TEXTURE,
    1000,
    true,
    0,
    0,
    ZOMBIE_SPAWN_COOLDOWN,
    true,
    true,
    "ULTIMATE DAY",
};

Level *LEVELS[] = {&LEVEL1, &LEVEL2, &LEVEL3, &LEVEL4};

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
        GetScreenWidth() - 50,
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
    if (!IsTextureValid(DAY_TEXTURE)) {
        DAY_TEXTURE = LoadTexture(DAY_PATH);
    }
    if (!IsTextureValid(NIGHT_TEXTURE)) {
        NIGHT_TEXTURE = LoadTexture(NIGHT_PATH);
    }
    if (currentLevel != NULL) {
        SunCount = currentLevel->initSunCount;
        if (currentLevel->lawnMowersActive) {
            SpawnLawnMowers();
        }
    }
    for (int i = 0; i < GRID_ROWS; i++) {
        calcluateWeight(GetCellDimensions().y * i +
                        GetCellDimensions().y / 2 +
                        GetPlayfieldRect().y);
    }
}

void Level_Draw() {
    float sw = GetScreenWidth();
    float sh = GetScreenHeight();
    float aspectRatio = sw / sh;
    float height = currentLevel->background->height * 0.95;
    float width = height * aspectRatio;
    Rectangle src = {220, 20, width, height};
    Rectangle dst = {0, 0, sw, sh};
    Vector2 origin = {0, 0};
    DrawTexturePro(*(currentLevel->background),
                   src, dst, origin, 0, WHITE);
    // Draw_Grid();
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
    if (currentLevel->spawnCooldown < sinceZombieSpawn) {
        if (currentLevel->infiniteZombies) {
            int chance = rand() % 4;
            SpawnZombie(chance == 1);
            // 25% of zombies will be flag
        } else if (normalZombiesSpawned + flagZombiesSpawned <
                   currentLevel->normalZombieCount + currentLevel->flagZombieCount) {
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
    }
    sinceSunSpawn += dt;
    sinceZombieSpawn += dt;
    if (zombiesKilled ==
            currentLevel->flagZombieCount + currentLevel->normalZombieCount &&
        currentLevel->infiniteZombies == false) {
        Game_End(true);
    }
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
        DrawLineEx(start, end, 5, RED);
    }
    for (int i = 0; i <= GRID_ROWS; i++) {
        Vector2 start = {pf.x, pf.y + i * colH};
        Vector2 end = {pf.x + pf.width, pf.y + i * colH};
        DrawLineEx(start, end, 5, RED);
    }
}