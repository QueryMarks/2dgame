#include "audio.h"
#include "SDL_mixer.h"

#include "simple_logger.h"


/////////
// I used https://lazyfoo.net/SDL_tutorials/lesson11/index.php to figure this out, mostly, along with the SDL mixer documentation
/////////
void audio_init() {
	Mix_Init(MIX_INIT_MP3);
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		slog("mixer audio failed to init");
		return;
	}
}

void audio_music_play(const char* path)
{
	Mix_Music* music = Mix_LoadMUS(path);
	if (music == NULL)return;
	Mix_PlayMusic(music, -1);

}

void audio_sfx_play(const char* path)
{
	Mix_Chunk* sfx = Mix_LoadWAV(path);
	if (sfx == NULL)return;
	Mix_PlayChannel(-1, sfx, 0);
}

void audio_sfx_play();