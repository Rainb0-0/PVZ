#include "SceneManager.h"
#include "raylib.h"
#include "resource_dir.h"

bool GameShouldQuit = false;

int main(void) {
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
    const int WIDTH = GetScreenWidth();
    const int HEIGHT = GetScreenHeight();
    InitWindow(WIDTH, HEIGHT, "Plants Vs. Zombies");
    ToggleFullscreen();
    SearchAndSetResourceDir("resources");
    SceneManager_Init(SCENE_MAINMENU);
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
