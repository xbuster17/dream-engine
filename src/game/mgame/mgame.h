#ifndef __MGAME_H__
#define __MGAME_H__
#include "../../de/de.h"

#define num_enemy_bullets 10922
// #define num_enemy_bullets 4096
#define num_player_bullets 1000

#include "bullets.h"

#include "hmap.h"

#define OBJ_STRUCT \
	v4f pos; \
	v4f rot; \
	v4f sca; \
	v4f vel, acc;

typedef struct obj{
	OBJ_STRUCT
} obj;

struct player{
	OBJ_STRUCT
	float rad;
	float height;
};

extern struct game {
	bullets* enemy_bullets;
	bullets* player_bullets;

	hmap* hmap;

	struct player player;
	float player_rad;
	m4f proj;
	m4f view;
	m4f mvp;
	int frame;
} game;


typedef struct scene{
	void (*init)(void);
	void (*update)(void);
	void (*free)(void);
} scn;

void mgame_main(void);

#endif
