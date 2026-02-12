#include "Chomper.h"
#include "Font.h"
#include "LawnMower.h"
#include "LevelSelector.h"
#include "Music.h"
#include "Pea.h"
#include "Peashooter.h"
#include "PlantSelection.h"
#include "SceneManager.h"
#include "Sun.h"
#include "Sunflower.h"
#include "Wallnut.h"
#include "Zombie.h"
#include "raylib.h"
#include "resource_dir.h"

bool GameShouldQuit = false;

// TODO game music based on scene

int main(void) {
    SetConfigFlags(FLAG_VSYNC_HINT |
                   FLAG_WINDOW_HIGHDPI |
                   FLAG_WINDOW_UNDECORATED);
    InitWindow(960, 600, "Plants Vs. Zombies");
    SearchAndSetResourceDir("resources");
    SceneManager_Init(SCENE_MAINMENU);
    if (!IsAudioDeviceReady()) {
        InitAudioDevice();
    }
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
    Wallnut_Init();
    LevelSelector_Init();
    PlantSelection_Init();
    InitMusicHandler(&mh, 0.5);
    while (!GameShouldQuit) {
        if (WindowShouldClose())
            if (IsKeyDown(KEY_ESCAPE) == false)
                GameShouldQuit = true;
        SceneManager_UpdateScene();
        SceneManager_Update();
        BeginDrawing();
        ClearBackground(BLACK);
        SceneManager_Draw();
        UpdateMusicHandler(&mh, GetFrameTime());
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
