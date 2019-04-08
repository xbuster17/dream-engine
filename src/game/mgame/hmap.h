#ifndef __MGAME_HMAP_H__
#define __MGAME_HMAP_H__

#include "../../de/de.h"

typedef struct hmap{
	float* cache;
	int x,y; //res
	int res; // must be square
	SDL_mutex* cache_mutex;

	v4f focus;
	v4f ifocus; // integer, floor of focus

	dvao* vao;
	dibo* ibo;
	dshd* shd;
	dtex* tex;
	dfbo* shadow;
	int mode; // dynamic, follow, fixed, generated;
	bool generated;
	bool use_shadow;
	float(*f)(float x, float y); // height function
} hmap;

enum hmap_mode{ HMAP_DYNAMIC=1<<0, HMAP_FOLLOW=1<<1, HMAP_FIXED=1<<2 };
hmap* hmap_new(int xres, int yres, float(*func)(float x, float y), enum hmap_mode mode);
hmap* hmap_newt(int res, float(*func)(float x, float y));
// void hmap_set_pos(v4f p);
// void hmap_set_rot(v4f r);
// void hmap_set_sca(v4f s);
void hmap_free(hmap*);
void hmap_update(hmap*, v4f focus);
void hmap_draw(hmap*);
// void hmap_mode(hmap*, bool dynamic, bool follow, bool fixed);

float hmap_eval(hmap*, float x, float y); // approximates with cached heights

#endif
