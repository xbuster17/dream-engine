#include "sound.h"
#include "../common/common.h"

static int init_mix_channels = 1024;
static int init_sample_rate = 44100; // 48000 cracks on android
// MIX_DEFAULT_FREQUENCY is 22050
static int init_bufsize = 2048;
static int init_channels = 2;
static Uint16 init_format = AUDIO_F32SYS;
// AUDIO_S16SYS, AUDIO_S8, AUDIO_U16SYS, AUDIO_S32SYS ... Signed or Unsigned
// LSB or MSB can be used instead of SYS for little or big endian byte order

//sdl audio callback signature
// void MyAudioCallback(void* userdata, Uint8* stream, int length);

// SDL_AudioDeviceID Dsnd_dev;


void dsnd_quit(void){
	// SDL_CloseAudioDevice(Dsnd_dev);
	Mix_Quit();
}



int dsnd_init(void){
// sdl audio (without mixer)
	// SDL_Init(SDL_INIT_AUDIO);
	// SDL_AudioSpec want, have;

	// SDL_memset(&want, 0, sizeof(want)); /* or SDL_zero(want) */
	// want.freq = 48000;
	// want.format = AUDIO_F32;
	// want.channels = 2;
	// want.samples = 1024;
	// want.callback = MyAudioCallback; /* you wrote this function elsewhere -- see SDL_AudioSpec for details */
	// Dsnd_dev = SDL_OpenAudioDevice(NULL, 0, &want, &have, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
	// if (Dsnd_dev == 0) {
	//     SDL_Log("Failed to open audio: %s", SDL_GetError());
	// } else {
	//     if (have.format != want.format) { /* we let this one thing change. */
	//         SDL_Log("We didn't get Float32 audio format.");
	//     }

// example audio test
	//     SDL_PauseAudioDevice(Dsnd_dev, 0); /* start audio playing. */
	//     SDL_Delay(5000); /* let the audio callback play some sound for 5 seconds. */
	//     SDL_CloseAudioDevice(dev);
	// }

//using sdl_mixer
	Mix_Init( MIX_INIT_FLAC | MIX_INIT_MOD | MIX_INIT_MP3 | MIX_INIT_OGG );
	if ( Mix_OpenAudio ( init_sample_rate, init_format, init_channels, init_bufsize ) == -1) {
		printf("Mix_openAudio: %s\n", Mix_GetError());
		DE_LOGE("%s\n", Mix_GetError());
	}

	Mix_AllocateChannels(init_mix_channels);
	return 0;
}




dsnd* dsnd_open(char* filename){ if(!filename) return NULL;
	dsnd* snd = Mix_LoadWAV(filename);
	if(!snd){
		DE_LOGE("%s\n", Mix_GetError());
		return NULL;
	}
	return snd;
}


void dsnd_free(dsnd* snd){ if(!snd) return;
	Mix_FreeChunk(snd);
}



int dsnd_play(dsnd* snd){ if(!snd) return -1;
	int chan = Mix_PlayChannel(-1, snd, 1);
	if(chan == -1){
		DE_LOG("%s\n", Mix_GetError());
	}
	return chan;
}

int dsnd_playl(dsnd* snd, int loops){ if(!snd) return -1;
	int chan = Mix_PlayChannel(-1, snd, loops);
	if(chan == -1){
		DE_LOG("%s\n", Mix_GetError());
	}
	return chan;
}







// music

dmus* dmus_open(char* filename){ if(!filename) return NULL;
	dmus* mus = Mix_LoadMUS(filename);
	if(!mus){
		DE_LOG("%s\n", Mix_GetError());
		return NULL;
	}
	return mus;
}

void dmus_free(dmus* mus){ if(!mus) return;
	Mix_FreeMusic(mus);
}

void dmus_play(dmus* mus){ if(!mus) return;
	Mix_PlayMusic(mus, -1);
}

void dmus_playl(dmus* mus, int loops){ if(!mus) return;
	Mix_PlayMusic(mus, loops);
}


