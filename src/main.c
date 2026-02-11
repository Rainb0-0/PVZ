#include "Chomper.h"
#include "Font.h"
#include "LawnMower.h"
#include "LevelSelector.h"
#include "Pea.h"
#include "Peashooter.h"
#include "PlantSelection.h"
#include "SceneManager.h"
#include "Sun.h"
#include "Sunflower.h"
#include "Zombie.h"
#include "raylib.h"
#include "resource_dir.h"

bool GameShouldQuit = false;

// TODO modify level save info for level 4
// TODO win and lose things
// TODO game music based on scene

int main(void) {
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    InitWindow(960, 600, "Plants Vs. Zombies");
    SearchAndSetResourceDir("resources");
    SceneManager_Init(SCENE_MAINMENU);
    Font_Init();
    LawnMower_Init();
    Zombie_Init();
    Peashooter_Init();
    Chomper_Init();
    Potato_Init();
    Sunflower_Init();
    Pea_Init();
    Sun_Init();
    Coin_Init();
    LevelSelector_Init();
    PlantSelection_Init();
    while (!GameShouldQuit) {
        if (WindowShouldClose())
            if (IsKeyDown(KEY_ESCAPE) == false)
                GameShouldQuit = true;
        SceneManager_UpdateScene();
        SceneManager_Update();
        BeginDrawing();
        ClearBackground(BLACK);
        SceneManager_Draw();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
