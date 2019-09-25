#ifndef __MGAME_H__
#define __MGAME_H__
#include "../../de/de.h"
#ifdef __cplusplus
extern "C" {
#endif

// #define num_enemy_bullets 64000
// #define num_enemy_bullets 10922
#define num_enemy_bullets 4096
#define num_player_bullets 800

#include "bullets.h"

#include "hmap.h"

#define OBJ_STRUCT \
	v4f pos; \
	v4f rot; \
	v4f sca; \
	v4f vel, acc

typedef struct obj{
	OBJ_STRUCT;
} obj;

struct player{
	OBJ_STRUCT;
	float rad;
	float height;
};

struct enemy{
	OBJ_STRUCT;
	float hp;
};

extern struct game {
	bullets* enemy_bullets;
	bullets* player_bullets;

	hmap* hmap0;
	hmap* hmap1;

	v4f sunpos;
	v4f suncol;

	struct player player;
	// float player_rad;
	// dfbo* pbulfbo;
	dfbo* bg;
	int bg_index;

	struct enemy enm[128];
	v4f clear_color;
	// m4f proj;
	// m4f view;
	// m4f mvp;
	int frame;
	float dt;
	Uint64 timePrev;
	Uint64 time;
	float ftime;
	
	struct lvl* curr_lvl;

	dsnd* phitsnd;
	float* screensmp;
	bool cam_in_hmap;

// #define GQ_MAX 6
// 	struct{
// 		float hm_shadow_tex_size;// the size of the shadow texture for bullets over hmap
// 		float hm_shadow_len;// how far bullets shadow gets sampled, 
// 		int hm_size;// how far bullets shadow gets sampled, 
// 		int bs_count;// how far bullets shadow gets sampled, 
// 	} gconf[GQ_MAX];
// 	int gquality;
	
	int main_fbo_scale;

} Game;

#define G Game

void mgame_main(void);

EMSCRIPTEN_KEEPALIVE void sunc( float x, float y, float z );

#include "points.h"
#include "mcyl.h"

#ifdef __cplusplus
}
#endif
#endif
