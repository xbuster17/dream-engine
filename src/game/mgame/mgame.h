#ifndef __MGAME_H__
#define __MGAME_H__
#include "../../de/de.h"
#ifdef __cplusplus
extern "C" {
#endif

// #define num_enemy_bullets 10922
#define num_enemy_bullets 4096
#define num_player_bullets 512

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

	struct player player;
	// float player_rad;
	dfbo* pbulfbo;

	struct enemy enm[128];
	v4f clear_color;
	// m4f proj;
	// m4f view;
	// m4f mvp;
	int frame;
	float dt;
	struct lvl* curr_lvl;
	dsnd* phitsnd;
	float* screensmp;
} Game;
#define G Game
void mgame_main(void);

#ifdef __cplusplus
}
#endif
#endif
