#ifndef __MGAME_HMAP_H__
#define __MGAME_HMAP_H__

#include "../../de/de.h"

typedef struct hmap{
	float(*f)(float x, float y);
	int x,y; //res
	// dobj3d obj;
	// v4f pos,rot,sca;
	// m4f M, N, mvp;
	// bool obj_updated;

	v4f focus;
	v4f ifocus; // integer, floor of focus

	dvao* vao;
	dibo* ibo;
	dshd* shd;
	dtex* tex;
	int mode;
	bool dynamic, follow, fixed, generated;
} hmap;

enum hmap_mode{ HMAP_DYNAMIC=1<<0, HMAP_FOLLOW=1<<1, HMAP_FIXED=1<<2 };
hmap* hmap_new(int xres, int yres, float(*func)(float x, float y), enum hmap_mode mode);
// void hmap_set_pos(v4f p);
// void hmap_set_rot(v4f r);
// void hmap_set_sca(v4f s);
void hmap_free(hmap*);
void hmap_update(hmap*, v4f focus);
void hmap_draw(hmap*);
// void hmap_mode(hmap*, bool dynamic, bool follow, bool fixed);


#endif
