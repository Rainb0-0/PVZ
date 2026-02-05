#include "raylib.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void PlayRandomOggWithPitch(const char *directoryPath, float volume) {
    if (!IsAudioDeviceReady()) {
        InitAudioDevice();
    }
    DIR *dir = opendir(directoryPath);
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

    Sound sound = LoadSound(filePath);
    if (sound.frameCount > 0) {
        float randomPitch = 0.85 + ((float)rand() / RAND_MAX) / 2.0;
        SetSoundPitch(sound, randomPitch);
        SetSoundVolume(sound, volume);
        PlaySound(sound);
    }
}