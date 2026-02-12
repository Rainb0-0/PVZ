#include "Music.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const char *GAME_MUSIC_PATH = "Music/game/";
const char *MENU_MUSIC_PATH = "Music/menu/";

MusicHandler mh;

static void BuildTrackPath(char *out, const char *dir, int index) {
    sprintf(out, "%s/%d.mp3", dir, index);
}

static int CountTracks(const char *dir) {
    int count = 0;
    char path[256];
    FILE *f;
    while (1) {
        sprintf(path, "%s/%d.mp3", dir, count + 1);
        f = fopen(path, "r");
        if (!f)
            break;
        fclose(f);
        count++;
    }
    return count;
}

void NextTrack(MusicHandler *mh) {
    if (!mh->playing)
        return;
    StopMusicStream(mh->current);
    UnloadMusicStream(mh->current);
    int newTrack;
    do {
        newTrack = (rand() % mh->trackCount) + 1;
    } while (mh->trackCount > 1 && newTrack == mh->trackIndex);
    mh->trackIndex = newTrack;
    char trackPath[256];
    BuildTrackPath(trackPath, mh->playlistPath, mh->trackIndex);
    mh->current = LoadMusicStream(trackPath);
    PlayMusicStream(mh->current);
    mh->volume = 1.0f;
    mh->fadingIn = false;
    mh->fadingOut = false;
}

void InitMusicHandler(MusicHandler *mh, float fadeSpeed) {
    mh->volume = 0.0f;
    mh->fadeSpeed = fadeSpeed;
    mh->playing = false;
    mh->fadingOut = false;
    mh->fadingIn = false;
    mh->stopping = false;
    mh->trackIndex = 1;
    mh->trackCount = 0;
    PlayPlaylist(mh, MENU_MUSIC_PATH);
}

void PlayPlaylist(MusicHandler *mh, const char *path) {
    strcpy(mh->playlistPath, path);
    mh->trackCount = CountTracks(path);
    mh->trackIndex = 1;

    char trackPath[256];
    BuildTrackPath(trackPath, path, mh->trackIndex);
    mh->current = LoadMusicStream(trackPath);
    PlayMusicStream(mh->current);

    mh->volume = 0.0f;
    mh->playing = true;
    mh->fadingIn = true;
}

void SwitchPlaylist(MusicHandler *mh, const char *path) {
    strcpy(mh->nextPlaylistPath, path);
    mh->fadingOut = true;
}

void StopMusicSmooth(MusicHandler *mh) {
    mh->stopping = true;
    mh->fadingOut = true;
}

void UpdateMusicHandler(MusicHandler *mh, float dt) {
    if (!mh->playing)
        return;
    UpdateMusicStream(mh->current);
    if (mh->fadingOut) {
        mh->volume -= mh->fadeSpeed * dt;
        if (mh->volume <= 0.0f) {
            mh->volume = 0.0f;
            mh->fadingOut = false;
            StopMusicStream(mh->current);
            UnloadMusicStream(mh->current);
            if (mh->stopping) {
                mh->playing = false;
                mh->stopping = false;
                return;
            }
            strcpy(mh->playlistPath, mh->nextPlaylistPath);
            mh->trackCount = CountTracks(mh->playlistPath);
            mh->trackIndex = (rand() % mh->trackCount) + 1;
            char trackPath[256];
            BuildTrackPath(trackPath, mh->playlistPath, mh->trackIndex);
            mh->current = LoadMusicStream(trackPath);
            PlayMusicStream(mh->current);
            mh->fadingIn = true;
        }
    }
    if (mh->fadingIn) {
        mh->volume += mh->fadeSpeed * dt;
        if (mh->volume >= 1.0f) {
            mh->volume = 1.0f;
            mh->fadingIn = false;
        }
    }
    SetMusicVolume(mh->current, mh->volume);
    if (IsKeyDown(KEY_F)) {
        float current = GetMusicTimePlayed(mh->current);
        float jump = 30.0f;
        SeekMusicStream(mh->current, current + jump);
    }
    if (GetMusicTimePlayed(mh->current) >= GetMusicTimeLength(mh->current)) {
        int newTrack;
        do {
            newTrack = (rand() % mh->trackCount) + 1;
        } while (mh->trackCount > 1 && newTrack == mh->trackIndex); // avoid repeating
        mh->trackIndex = newTrack;
        char trackPath[256];
        BuildTrackPath(trackPath, mh->playlistPath, mh->trackIndex);
        UnloadMusicStream(mh->current);
        mh->current = LoadMusicStream(trackPath);
        PlayMusicStream(mh->current);
    }
}
