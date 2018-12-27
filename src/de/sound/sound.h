#ifndef __DE_SOUND_H__
#define __DE_SOUND_H__

#include "../conf.h"
#include "../libs.h"

typedef Mix_Chunk dsnd;
typedef Mix_Music dmus;

int dsnd_init(void);
void dsnd_quit(void);

dsnd* dsnd_open(char* filename);
void dsnd_free(dsnd*);
int dsnd_play(dsnd* snd);
int dsnd_playl(dsnd* snd, int loops);

dmus* dmus_open(char* filename);
void dmus_free(dmus*);
void dmus_play(dmus* mus);
void dmus_playl(dmus* mus, int loops);



//todo
// dsnd* dsnd_newr(void* data, int size); // already in output format
// dsnd* dsnd_new(void* data, int samples, int channels, int rate, Uint16 format);
// auto resample to output format


#endif
