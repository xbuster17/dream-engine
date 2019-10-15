#ifndef __MGAME_HMAP_H__
#define __MGAME_HMAP_H__

#include "../../de/de.h"
#ifdef __cplusplus
extern "C" {
#endif

typedef struct hmap{
	float* cache;
	int x,y; //res
	int res; // must be square
	SDL_mutex* cache_mutex;
	float shadow_len; 
	v4f scale; //todo
	v4f focus;
	v4f ifocus; // integer, floor of focus

	dvao* vao;
	dibo* ibo;
	dshd* shd;
	dtex* tex;
	dfbo* shadow; // from bullets
	dfbo* shadow_map; // from sun
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
float hmap_evald(hmap*, float x, float y, float* normal_x, float* normal_y, float* normal_z); // get normal

v2i hmap_mouse(hmap*); // trace mouse pointer thru the map
// EMSCRIPTEN_KEEPALIVE v2f hmap_sspoint(hmap*, v4f);
// enum hmap_trans_effect{hmte_}
// void hmap_trans(hmap*, float(*dst_func)(float x,float y), float time, enum hmap_trans_effect);


#ifdef __cplusplus
}
#endif
#endif
