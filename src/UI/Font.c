#include "raylib.h"

Font FONT;
const char *FONT_PATH = "Fonts/Font.ttf";

void Font_Init() {
    FONT = LoadFont(FONT_PATH);
}