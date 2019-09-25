#ifndef __MPOINTS_H__
#define __MPOINTS_H__
#include "../../de/de.h"
#ifdef __cplusplus
extern "C" {
#endif
// #include "../../de/de.h"
// #include "mgame.h" // extern struct G
// #include "hmap.h" // hmap_eval
#define num_mpts (1024*4)

struct mpt{
	v4f pos;
	v4f vel;
	v4f acc;
	v4f rot;
	v4c col;
	float scale;
};
typedef struct mpoints{
	struct mpt* p;
	dvao* vao;
	int pivot;
} mpts;

void pts_init(void);
void pts_quit(void);

mpts* pts_new(void);
void pts_free(mpts*);
void pts_update(mpts*);

void pts_add(mpts* pts, v4f pos, int count);
void pts_draw(mpts* pts);


#ifdef __cplusplus
}
#endif
#endif
