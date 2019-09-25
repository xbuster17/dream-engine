#ifndef __MCYL_H__
#define __MCYL_H__
/* mega cylinder, a cylinder wrapped grid */
#include "../../de/de.h"
#ifdef __cplusplus
extern "C" {
#endif

// #include "../../de/de.h"
// #include "mgame.h" // extern struct G
// #include "hmap.h" // hmap_eval
// #define num_mpts (1024*2)

typedef struct mcyl{
	dmesh* mesh;
	int x;//slices
	int y;//disks
	float* v;
	bool sph;
	// dvao* vao;
	// dshd* shd;
} mcyl;

// void mcyl_init(void);
// void mcyl_quit(void);

mcyl* mcyl_new(float* mxn, int x, int y);
void mcyl_free(mcyl*);
void mcyl_update(mcyl*);
void mcyl_draw(mcyl*);
void mcyl_rand(mcyl*);
void mcyl_set(mcyl*, int x, int y, float val);
float mcyl_get(mcyl*, int x, int y);
dvao* mcyl_vao(float* mxn, int x, int y);
void* mcyl_vaob(float* mxn, int x, int y, bool cyl);
void mcyl_regen(mcyl* m, float* v);
void mcyl_bvo_usage(mcyl*, GLint u); // GL_STREAM_DRAW, GL_DYNAMIC_DRAW, GL_STATIC_DRAW


#ifdef __cplusplus
}
#endif
#endif
