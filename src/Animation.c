#include "Animation.h"
#include "Object.h"
#include "raylib.h"
#include <stdlib.h>

Animation *newAnimation(State *self, Vector2 pos, Vector2 offset, float scale) {
    Animation *a = (Animation *)malloc(sizeof(Animation));
    a->draw = Animation_Draw;
    a->update = Animation_Update;
    a->pos = pos;
    a->frameIndex = 0;
    a->frameTime = 0;
    a->offset = offset;
    a->scale = scale;
    a->state = self;
    return a;
}

void Animation_Draw(Animation *self) {
    int index = FindObjectIndex(self, false);
    DrawObject(Objects[index], self->scale, self->offset, WHITE);
}

void Animation_Update(Animation *self) {
    if (self->frameIndex == self->state->maxFrameIndex - 1) {
        RemoveObject(self, false);
        return;
    }
}