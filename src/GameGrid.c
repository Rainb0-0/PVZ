#include "GameGrid.h"
#include "raylib.h"
#include <ctype.h>

Rectangle GetPlayfieldRect() {
    float sw = (float)GetScreenWidth();
    float sh = (float)GetScreenHeight();
    return (Rectangle){sw * 0.04f, sh * 0.09f, sw * 0.8f, sh * 0.89f};
}

Vector2 GetCellDimensions() {
    Rectangle playfield = GetPlayfieldRect();
    float width = playfield.width / GRID_COLS;
    float height = playfield.height / GRID_ROWS;
    Vector2 res = {width, height};
    return res;
}

Vector2 SnapPosToGrid(Vector2 pos) {
    Rectangle playfield = GetPlayfieldRect();
    float cellWidth = playfield.width / GRID_COLS;
    float cellHeight = playfield.height / GRID_ROWS;
    int row = (pos.y - playfield.y) / cellHeight;
    int col = (pos.x - playfield.x) / cellWidth;
    float x = playfield.x + col * cellWidth + cellWidth / 2;
    float y = playfield.y + row * cellHeight + cellHeight / 2;
    return (Vector2){x, y};
}

int GetRowIndex(float y) {
    return (y - GetPlayfieldRect().y) / GetCellDimensions().y;
}

int GetColIndex(float x) {
    return (x - GetPlayfieldRect().x) / GetCellDimensions().x;
}