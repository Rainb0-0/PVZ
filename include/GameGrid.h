#ifndef GAMEGRID_H
#define GAMEGRID_H

#include "raylib.h"
#include <ctype.h>

#define GRID_ROWS 5
#define GRID_COLS 9

Rectangle GetPlayfieldRect();
Vector2 GetCellDimensions();

Vector2 SnapPosToGrid(Vector2 pos);

int GetRowIndex(float y);
int GetColIndex(float x);
#endif