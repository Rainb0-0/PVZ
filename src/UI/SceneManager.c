#include "SceneManager.h"
#include "Game.h"
#include "MainMenu.h"
#include "Shop.h"
#include "main.h"
#include <stdlib.h>

static Scene scenes[SCENE_COUNT] = {
    [SCENE_MAINMENU] = {MainMenu_Init, MainMenu_Draw, MainMenu_Update},
    [SCENE_GAME] = {Game_Init, Game_Draw, Game_Update},
    [SCENE_SHOP] = {Shop_Init, Shop_Draw, Shop_Update},
    [QUIT] = {NULL, Quit_Game, Quit_Game},
};

static SceneType currentScene;
static SceneType nextScene;

void SceneManager_Init(SceneType startScene) {
    currentScene = startScene;
    nextScene = startScene;
    scenes[currentScene].init();
    for (int i = 0; i < SCENE_COUNT; i++) {
        if (scenes[i].init != NULL) {
            scenes[i].init();
        }
    }
}

void SceneManager_Change(SceneType next) {
    nextScene = next;
}

void SceneManager_UpdateScene() {
    if (nextScene != currentScene) {
        currentScene = nextScene;
        if (scenes[currentScene].init != NULL)
            scenes[currentScene].init();
    }
}

void SceneManager_Draw() {
    scenes[currentScene].draw();
}

void SceneManager_Update() {
    scenes[currentScene].update();
}
