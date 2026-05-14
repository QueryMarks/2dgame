#ifndef __AUDIO_H__
#define __AUDIO_H__

#include <SDL.h>

#include "gfc_types.h"

void audio_init();

void audio_music_play(const char* path);
void audio_sfx_play(const char* path);

#endif