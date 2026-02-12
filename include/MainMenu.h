#ifndef MAINMENU_H
#define MAINMENU_H

#include "raylib.h"

extern Texture2D BUTTON_NORMAL_TEXTURE;
extern const char *BUTTON_CLIKC_SOUND_PATH;

extern const float BUTTON_SCALE;
extern const float MENU_BUTTON_MARGIN;
extern const float BUTTON_FONT_SIZE;
extern float MENU_BUTTON_WIDTH;
extern float MENU_BUTTON_HEIGHT;

extern Color BG_OVERLAY;

void MainMenu_Init();
void MainMenu_Draw();
void MainMenu_Update();

void Quit_Game();

#endif