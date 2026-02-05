#include "Coin.h"
#include "Game.h"
#include "Object.h"
#include "Shop.h"
#include "Sound.h"
#include "State.h"
#include "raylib.h"
#include <stdlib.h>

// TODO add sound for coin collection
// TODO ??? add some kind of coin count indicator

Texture2D COIN_TEXTURE;

const char *COIN_PATH = "Sprites/Coin.png";
const char *COIN_COLLECTION_SOUND_PATH = "Sounds/Coin/";

const int COIN_WORTH = 5;

const int COIN_FRAME_WIDTH = 80;
const int COIN_FRAME_HEIGHT = 80;
const int COIN_MAX_FRAMES = 20;
const float COIN_FRAME_TIME = FRAME_TIME;

const float COIN_SCALE = 3;

State COIN_IDLE = {
    COIN_FRAME_WIDTH,
    COIN_FRAME_HEIGHT,
    COIN_MAX_FRAMES,
    COIN_FRAME_TIME,
    &COIN_TEXTURE,
};

Coin *newCoin(Vector2 pos) {
    Coin *nc = (Coin *)malloc(sizeof(Coin));
    nc->frameIndex = 0;
    nc->frameTime = 0;
    nc->pos = pos;
    nc->state = &COIN_IDLE;
    nc->draw = Coin_Draw;
    nc->update = Coin_Update;
    return nc;
}

void Coin_Init() {
    COIN_TEXTURE = LoadTexture(COIN_PATH);
}

void Coin_Draw(Coin *self) {
    Vector2 offset = {-105, -105};
    int index = FindObjectIndex(self, false);
    DrawObject(Objects[index], COIN_SCALE * 1.15, offset, WHITE);
}

void Coin_Update(Coin *self) {
    Vector2 mousePos = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
        IsPositionInsideCircle(self->pos,
                               self->state->frameWidth,
                               mousePos)) {
        PlayRandomOggWithPitch(COIN_COLLECTION_SOUND_PATH, 1);
        CoinCount += COIN_WORTH;
        Shop_SaveState();
        RemoveObject(self, false);
    }
}