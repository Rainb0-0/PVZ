#include "MainMenu.h"
#include "Game.h"
#include "SceneManager.h"
#include "main.h"
#include "raylib.h"
#include "stdlib.h"

Texture2D MAINMENU_BACKGROUND;
Texture2D TITLE;
Texture2D BUTTON_NORMAL_TEXTURE;
Texture2D BUTTON_DOWN_TEXTURE;

Color BG_OVERLAY;

const char *MAINMENU_BACKGROUND_PATH = "Menu.jpg";
const char *BUTTON_NORMAL_TEXTURE_PATH = "Button.png";
const char *BUTTON_DOWN_TEXTURE_PATH = "ButtonDown.png";
const char *TITLE_PATH = "Title.png";

const float TITLE_SCALE = 3.3;
const float BUTTON_SCALE = 4;
const float BUTTONS_Y_OFFSET = 1000;
const float MENU_BUTTON_MARGIN = 10 * BUTTON_SCALE;
const float BUTTON_FONT_SIZE = 24 * BUTTON_SCALE;
float MENU_BUTTON_WIDTH;
float MENU_BUTTON_HEIGHT;

typedef struct Button {
    SceneType dest;
    char *text;
    bool hovered;
    Rectangle bounds;
} Button;

Button Play = {
    SCENE_GAME,
    "Play",
    false,
};
Button Shop = {
    SCENE_SHOP,
    "Shop",
    false,
};
Button Quit = {
    QUIT,
    "Quit",
    false,
};

Button *Buttons[] = {&Play, &Shop, &Quit};
int BUTTONS_SIZE = sizeof(Buttons) / sizeof(Buttons[0]);

void Buttons_Draw() {
    for (int i = 0; i < BUTTONS_SIZE; i++) {
        Rectangle src = {0, 0, BUTTON_NORMAL_TEXTURE.width,
                         BUTTON_NORMAL_TEXTURE.height};
        Vector2 origin = {0, 0};
        if (Buttons[i]->hovered)
            DrawTexturePro(BUTTON_DOWN_TEXTURE,
                           src, Buttons[i]->bounds, origin, 0, WHITE);
        else
            DrawTexturePro(BUTTON_NORMAL_TEXTURE,
                           src, Buttons[i]->bounds, origin, 0, WHITE);
        Vector2 textSize = MeasureTextEx(GetFontDefault(),
                                         Buttons[i]->text,
                                         BUTTON_FONT_SIZE, 1);
        Vector2 textPos = {Buttons[i]->bounds.x +
                               MENU_BUTTON_WIDTH / 2 - textSize.x / 2,
                           Buttons[i]->bounds.y +
                               MENU_BUTTON_HEIGHT / 2 - textSize.y / 2};
        if (Buttons[i]->hovered)
            textPos.y += 1 * BUTTON_SCALE;
        DrawText(Buttons[i]->text, textPos.x, textPos.y,
                 BUTTON_FONT_SIZE, BLACK);
    }
}

void MainMenu_Init() {
    MAINMENU_BACKGROUND = LoadTexture(MAINMENU_BACKGROUND_PATH);
    TITLE = LoadTexture(TITLE_PATH);
    BUTTON_DOWN_TEXTURE = LoadTexture(BUTTON_DOWN_TEXTURE_PATH);
    BUTTON_NORMAL_TEXTURE = LoadTexture(BUTTON_NORMAL_TEXTURE_PATH);
    MENU_BUTTON_WIDTH = BUTTON_NORMAL_TEXTURE.width * BUTTON_SCALE;
    MENU_BUTTON_HEIGHT = BUTTON_NORMAL_TEXTURE.height * BUTTON_SCALE;
    for (int i = 0; i < BUTTONS_SIZE; i++) {
        Button *cur = Buttons[i];
        cur->bounds.x = GetScreenWidth() / 2 - MENU_BUTTON_WIDTH / 2;
        cur->bounds.y = BUTTONS_Y_OFFSET +
                        i * (MENU_BUTTON_MARGIN + MENU_BUTTON_HEIGHT) +
                        MENU_BUTTON_HEIGHT / 2;
        cur->bounds.width = MENU_BUTTON_WIDTH;
        cur->bounds.height = MENU_BUTTON_HEIGHT;
    }
}

void MainMenu_Draw() {
    BG_OVERLAY.a = 'f';
    BG_OVERLAY.r = '0';
    BG_OVERLAY.g = '0';
    BG_OVERLAY.b = '0';
    BG_OVERLAY = ColorBrightness(BG_OVERLAY, -1);
    BG_OVERLAY = ColorAlpha(BG_OVERLAY, 0.6);
    float bgWidth = MAINMENU_BACKGROUND.width;
    float bgHeight = MAINMENU_BACKGROUND.height;
    Rectangle bgSrc = {0, 0, bgWidth, bgHeight};
    Rectangle bgDst = {0, 0, GetScreenWidth(), GetScreenHeight()};
    Vector2 origin = {0, 0};
    NPatchInfo npi;
    npi.source = bgSrc;
    npi.bottom = 0;
    npi.left = 0;
    npi.right = 0;
    npi.top = 0;
    npi.layout = NPATCH_NINE_PATCH;
    DrawTextureNPatch(MAINMENU_BACKGROUND, npi, bgDst, origin, 0, WHITE);
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BG_OVERLAY);
    Rectangle titleSrc = {0, 0, TITLE.width, TITLE.height};
    Rectangle titleDst = {
        (GetScreenWidth() - TITLE.width * TITLE_SCALE) / 2,
        300,
        TITLE.width * TITLE_SCALE,
        TITLE.height * TITLE_SCALE,
    };
    DrawTexturePro(TITLE, titleSrc, titleDst, origin, 0, WHITE);
    Buttons_Draw();
}

void MainMenu_Update() {
    Vector2 mousePos = GetMousePosition();
    for (int i = 0; i < BUTTONS_SIZE; i++) {
        if (IsPositionInsideRect(Buttons[i]->bounds, mousePos)) {
            Buttons[i]->hovered = true;
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                SceneManager_Change(Buttons[i]->dest);
            }
        } else {
            Buttons[i]->hovered = false;
        }
    }
}

void Quit_Game() {
    GameShouldQuit = true;
}