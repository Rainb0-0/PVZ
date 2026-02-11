#include "raylib.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SOUNDS 256

typedef struct {
    char path[256];
    Sound sound;
    int loaded;
} SoundCacheEntry;

static SoundCacheEntry soundCache[MAX_SOUNDS];
static int soundCacheCount = 0;

Sound *GetCachedSound(const char *filePath) {
    for (int i = 0; i < soundCacheCount; i++) {
        if (strcmp(soundCache[i].path, filePath) == 0) {
            return &soundCache[i].sound;
        }
    }

    if (soundCacheCount >= MAX_SOUNDS) {
        printf("Sound cache full!\n");
        return NULL;
    }

    Sound newSound = LoadSound(filePath);
    if (newSound.frameCount == 0)
        return NULL;

    strcpy(soundCache[soundCacheCount].path, filePath);
    soundCache[soundCacheCount].sound = newSound;
    soundCache[soundCacheCount].loaded = 1;
    soundCacheCount++;

    return &soundCache[soundCacheCount - 1].sound;
}

void PlayRandomOggWithPitch(const char *directoryPath, float volume) {
    if (!IsAudioDeviceReady()) {
        InitAudioDevice();
    }

    DIR *dir = opendir(directoryPath);
    if (!dir)
        return;

    int maxNumber = 0;
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        const char *filename = entry->d_name;
        if (strstr(filename, ".ogg")) {
            char *dot = strchr(filename, '.');
            if (dot) {
                *dot = '\0';
                int num = atoi(filename);
                if (num > maxNumber)
                    maxNumber = num;
                *dot = '.';
            }
        }
    }
    closedir(dir);

    if (maxNumber == 0)
        return;

    int randomNum = (rand() % maxNumber) + 1;
    char filePath[256];
    snprintf(filePath, sizeof(filePath), "%s/%d.ogg", directoryPath, randomNum);

    Sound *sound = GetCachedSound(filePath);
    if (!sound)
        return;

    float randomPitch = 0.85f + ((float)rand() / RAND_MAX) * 0.5f;
    SetSoundPitch(*sound, randomPitch);
    SetSoundVolume(*sound, volume);
    PlaySound(*sound);
}
