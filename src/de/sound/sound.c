#include "sound.h"
#include "../common/common.h"
#include "../de_struct.h"

struct Dsnd_init_conf Dsnd_init_conf = {
	.mix_channels  = 512,
	.sample_rate  = 44100, // MIX_DEFAULT_FREQUENCY is 22050
	.bufsize  = 1024,
	.channels  = 2,
	.format  = AUDIO_F32SYS
};

// static int init_mix_channels = 512;

// static int init_sample_rate = 44100; // MIX_DEFAULT_FREQUENCY is 22050
// static int init_bufsize = 2048;
// static int init_bufsize = 128;
// static int init_bufsize = 16;
// static int init_channels = 2;
// static Uint16 init_format = AUDIO_F32SYS;
// static Uint16 init_format = AUDIO_S8;
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
	// if ( Mix_OpenAudio ( init_sample_rate, init_format, init_channels, init_bufsize ) == -1) {
	if ( Mix_OpenAudio ( Dsnd_init_conf.sample_rate, Dsnd_init_conf.format,
			Dsnd_init_conf.channels, Dsnd_init_conf.bufsize ) == -1) {
		printf("Mix_openAudio: %s\n", Mix_GetError());
		DE_LOGE("%s\n", Mix_GetError());
	}


	// Mix_AllocateChannels(init_mix_channels);
	Mix_AllocateChannels(Dsnd_init_conf.mix_channels);

	De.sample_rate = Dsnd_init_conf.sample_rate;
	De.audio_channels = Dsnd_init_conf.channels;
	De.audio_buffer_size = Dsnd_init_conf.bufsize;
	De.audio_format = Dsnd_init_conf.format;
	// De.sample_rate = init_sample_rate;
	// De.audio_channels = init_channels;
	// De.audio_buffer_size = init_bufsize;
	// De.audio_format = init_format;

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
	int chan = Mix_PlayChannel(-1, snd, 0);
	if(chan == -1){
		chan = -2;
		DE_LOG("%s\n", Mix_GetError());
	}
	return chan;
}



int dsnd_playl(dsnd* snd, int loops){ if(!snd) return -1;
	int chan = Mix_PlayChannel(-1, snd, loops);
	if(chan == -1){
		chan = -2;
		DE_LOG("%s\n", Mix_GetError());
	}
	return chan;
}



int dsnd_playf(dsnd* snd, int loops, int fade){ if(!snd) return -1;
	int chan = Mix_FadeInChannel(-1, snd, loops, fade);
	if(chan == -1){
		chan = -2;
		DE_LOG("%s\n", Mix_GetError());
	}
	return chan;
}



dsnd* dsnd_new(void* data, Uint32 len){ if(!data) return NULL;
	return Mix_QuickLoad_RAW(data, len);
}



void dsnd_fade(int channel, int ms){ // -1 to fade out all channels
	Mix_FadeOutChannel(channel, ms);
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


