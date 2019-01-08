#ifndef __DE_SOUND_H__
#define __DE_SOUND_H__

#include "../conf.h"
#include "../libs.h"

// overwrite values before calling de_init
extern struct Dsnd_init_conf{
	int mix_channels;
	int sample_rate;
	int bufsize;
	int channels;
	Uint16 format;
} Dsnd_init_conf;

typedef Mix_Chunk dsnd;
typedef Mix_Music dmus;

int dsnd_init(void);
void dsnd_quit(void);

dsnd* dsnd_open(char* filename);
dsnd* dsnd_new(void* data, Uint32 len); // keep data in memory until it's no longer playing (while dsnd is Mix_Chunk)
void dsnd_free(dsnd*);
int dsnd_play(dsnd* snd);
int dsnd_playl(dsnd* snd, int loops); // -1 for infinite loop
int dsnd_playf(dsnd* snd, int loops, int fade_in_ms);

void dsnd_fade(int channel, int ms); // -1 to fade out all channels


dmus* dmus_open(char* filename);
void dmus_free(dmus*);
void dmus_play(dmus* mus);
void dmus_playl(dmus* mus, int loops);

#endif
