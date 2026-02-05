#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

typedef struct {
    void (*init)();
    void (*draw)();
    void (*update)();
} Scene;

typedef enum SceneType {
    SCENE_MAINMENU,
    SCENE_LEVELSELECT,
    SCENE_SHOP,
    SCENE_SETTINGS,
    SCENE_GAME,
    QUIT,
    SCENE_COUNT
} SceneType;

void SceneManager_Init(SceneType startScene);
void SceneManager_Change(SceneType next);
void SceneManager_UpdateScene();
void SceneManager_Update();
void SceneManager_Draw();

#endif