#ifndef MUSIC_H
#define MUSIC_H

#include "raylib.h"

typedef struct {
    Music current;
    float volume;
    float fadeSpeed;
    int trackIndex;
    int trackCount;
    char playlistPath[256];
    char nextPlaylistPath[256];
    bool playing;
    bool fadingOut;
    bool fadingIn;
    bool stopping;
} MusicHandler;

extern MusicHandler mh;

extern const char *GAME_MUSIC_PATH;
extern const char *MENU_MUSIC_PATH;

void NextTrack(MusicHandler *mh);
void InitMusicHandler(MusicHandler *mh, float fadeSpeed);
void PlayPlaylist(MusicHandler *mh, const char *path);
void SwitchPlaylist(MusicHandler *mh, const char *path);
void StopMusicSmooth(MusicHandler *mh);
void UpdateMusicHandler(MusicHandler *mh, float dt);

#endif
