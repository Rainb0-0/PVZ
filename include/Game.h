#ifndef GAME_H
#define GAME_H

#include "GameGrid.h"
#include "Object.h"
#include "State.h"
#include "Zombie.h"
#include "raylib.h"
#include <ctype.h>

#define FRAME_TIME 0.08

extern int SunCount;

extern const Vector2 POSITION_TOLERANCE;
extern float weights[GRID_ROWS];
extern bool lawnMowers[GRID_ROWS];
extern int chompers[GRID_ROWS];

extern bool GamePaused;
extern int GameState;

void calcluateWeight(float y);

Zombie *ZombieAtPosition(Vector2 pos);
bool IsZombieInLaneOfPosition(Vector2 pos);
Plant *PlantAtPosition(Vector2 pos);

bool IsPositionInsideRect(Rectangle rect, Vector2 pos);
bool IsPositionInsideCircle(Vector2 center, float radius, Vector2 pos);

void DamagePlant(Plant *plant, float damage);
void KillZombiesInCircle(Vector2 center, float radius);

void Game_Init();
void Game_Draw();
void Game_Update();
void Game_End(bool won);

#endif