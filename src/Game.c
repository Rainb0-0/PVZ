#include "Game.h"
#include "Chomper.h"
#include "Coin.h"
#include "Font.h"
#include "GameGrid.h"
#include "LawnMower.h"
#include "Level.h"
#include "LevelSelector.h"
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

// TODO add score count, save for level 4

int SunCount = 100;

const Vector2 POSITION_TOLERANCE = {10, 10};

float weights[GRID_ROWS];
bool lawnMowers[GRID_ROWS];
int chompers[GRID_ROWS];

const char *GAME_START_SOUND = "Sounds/Level/Start/";
const char *GAME_WON_SOUND = "Sounds/Level/Win/";
const char *GAME_LOST_SOUND = "Sounds/Level/Lose/";

bool GamePaused = false;
// 0 means ongoing, 1 win, -1 lost
int GameState = 0;

float GameDuration = 0;

const float OVERLAY_SCALE = 1.2;
const float OVERLAY_FONT_SIZE = 30 * OVERLAY_SCALE;
const float OVERLAY_BUTTON_MARGIN = 16 * OVERLAY_SCALE;

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

typedef struct OverlayButton {
    Texture2D *texture;
    void (*onClick)();
    bool hovered;
    char *text;
    Rectangle bounds;
} OverlayButton;

void BackToMenu() {
    Level_Destroy();
    GamePaused = false;
    GameState = 0;
    SceneManager_Change(SCENE_MAINMENU);
}

void ResumeGame() {
    GamePaused = false;
}

OverlayButton BackToMenuButton = {
    &BUTTON_NORMAL_TEXTURE,
    BackToMenu,
    false,
    "MAIN MENU",
};

OverlayButton ResumeButton = {
    &BUTTON_NORMAL_TEXTURE,
    ResumeGame,
    false,
    "RESUME",
};

OverlayButton *PauseButtons[] = {&ResumeButton, &BackToMenuButton};
OverlayButton *GameOverButtons[] = {&BackToMenuButton};

void Overlay_Draw() {
    Color c = BLACK;
    c.a = 200;
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), c);
    char text[20];
    if (GameState == -1) {
        char temp[20] = "YOU LOST D:";
        strcpy(text, temp);
    } else if (GameState == 0) {
        char temp[20] = "GAME PAUSED";
        strcpy(text, temp);
    } else {
        char temp[20] = "YOU WON :D";
        strcpy(text, temp);
    }
    Vector2 textSize = MeasureTextEx(FONT, text, OVERLAY_FONT_SIZE, 1);
    Vector2 textPos = {(GetScreenWidth() - textSize.x) / 2, 200};
    Vector2 origin = {0, 0};
    DrawTextPro(FONT, text, textPos, origin, 0, OVERLAY_FONT_SIZE, 1, WHITE);
    int length = 0;
    OverlayButton **list;
    if (GameState == 0) {
        list = PauseButtons;
        length = 2;
    } else {
        list = GameOverButtons;
        length = 1;
        char currentScore[20] = "score : ";
        sprintf(currentScore + strlen(currentScore), "%.2f", GameDuration);
        Vector2 textSizeCurScore = MeasureTextEx(FONT, currentScore, OVERLAY_FONT_SIZE, 1);
        Vector2 textPosCurScore = {(GetScreenWidth() - textSizeCurScore.x) / 2,
                                   textPos.y + textSize.y};
        DrawTextPro(FONT, currentScore, textPosCurScore, origin, 0,
                    OVERLAY_FONT_SIZE, 1, WHITE);
        char maxScore[20] = "max score : ";
        int scoreIndex = 0;
        if (currentLevel == &LEVEL1) {
            scoreIndex = 0;
        } else if (currentLevel == &LEVEL2) {
            scoreIndex = 1;
        } else if (currentLevel == &LEVEL3) {
            scoreIndex = 2;
        } else {
            scoreIndex = 3;
        }
        if (LEVELSTATS[scoreIndex] < GameDuration) {
            LEVELSTATS[scoreIndex] = GameDuration;
            LevelStats_SaveState();
            LevelStats_ReadFile();
        }
        sprintf(maxScore + strlen(maxScore), "%.2f", LEVELSTATS[scoreIndex]);
        Vector2 textSizeMaxScore = MeasureTextEx(FONT, maxScore,
                                                 OVERLAY_FONT_SIZE, 1);
        Vector2 textPosMaxScore = {(GetScreenWidth() - textSizeMaxScore.x) / 2,
                                   textPosCurScore.y + textSizeMaxScore.y};
        DrawTextPro(FONT, maxScore, textPosMaxScore,
                    origin, 0, OVERLAY_FONT_SIZE, 1, WHITE);
    }
    for (int i = 0; i < length; i++) {
        OverlayButton *cur = list[i];
        Color o = WHITE;
        if (cur->hovered)
            o = LIGHTGRAY;
        Rectangle src = {0, 0, cur->texture->width, cur->texture->height};
        DrawTexturePro(*cur->texture, src, cur->bounds, origin, 0, o);
        textSize = MeasureTextEx(FONT, cur->text, OVERLAY_FONT_SIZE, 1);
        textPos.x = cur->bounds.x + (cur->bounds.width - textSize.x) / 2;
        textPos.y = cur->bounds.y + (cur->bounds.height - textSize.y) / 2;
        DrawTextPro(FONT, cur->text,
                    textPos, origin, 0, OVERLAY_FONT_SIZE, 1, BLACK);
    }
}

void Overlay_Update() {
    int length = 0;
    OverlayButton **list;
    if (GameState == 0) {
        list = PauseButtons;
        length = 2;
    } else {
        list = GameOverButtons;
        length = 1;
    }
    Vector2 mousePos = GetMousePosition();
    bool isClicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    for (int i = 0; i < length; i++) {
        OverlayButton *cur = list[i];
        Color o = WHITE;
        if (cur->hovered)
            o = LIGHTGRAY;
        if (IsPositionInsideRect(cur->bounds, mousePos)) {
            cur->hovered = true;
            if (isClicked) {
                cur->onClick();
            }
        } else {
            cur->hovered = false;
        }
    }
}

void Game_End(bool won) {
    GamePaused = true;
    GameState = (won == true ? 1 : -1);
}

void Game_Init() {
    Level_Init();
    PlantSelection_Init();
    for (int i = 0; i < GRID_ROWS; i++) {
        float y = GetPlayfieldRect().y +
                  i * GetCellDimensions().y + GetCellDimensions().y / 2;
        calcluateWeight(y);
    }
    GameDuration = 0;
    for (int i = 0; i < 2; i++) {
        OverlayButton *cur = PauseButtons[i];
        float width = cur->texture->width * OVERLAY_SCALE;
        float height = cur->texture->height * OVERLAY_SCALE;
        float x = (GetScreenWidth() - width) / 2;
        float y = (GetScreenHeight() - height) / 2 +
                  i * (height + OVERLAY_BUTTON_MARGIN);
        Rectangle bounds = {x, y, width, height};
        cur->bounds = bounds;
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
    if (GamePaused) {
        Overlay_Draw();
    }
}

void Game_Update() {
    if (IsKeyPressed(KEY_E)) {
        Game_End(true);
    }
    if (IsKeyPressed(KEY_ESCAPE) && GameState == 0)
        GamePaused = !GamePaused;
    if (GamePaused) {
        Overlay_Update();
        return;
    }
    GameDuration += GetFrameTime();
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