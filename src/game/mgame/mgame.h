#ifndef __MGAME_H__
#define __MGAME_H__
#include "../../de/de.h"

// #define num_enemy_bullets 10922
// #define num_enemy_bullets 8192
#define num_enemy_bullets 4096
#define num_player_bullets 1000

#include "bullets.h"
// typedef struct bullet {
// 	bool inited;
// 	v4f pos;
// 	v4c col0;
// 	v4c col1;

// 	v4f vel;
// 	v4f acc;
// 	v4f force;

// 	float rad;
// 	float tvel;
// 	float tacc;
// 	float lifetime;
// 	int frame;
// 	// bool expired;
// 	float dmg;
// 	struct bullets* buf;
// 	void (*run)(struct bullet*);

// 	// struct bullet* (*end)(struct bullet);
// } bullet;


// typedef struct bullets{
// 	bullet* b;
// 	uint len;
// 	dvao* vao;

// 	uint cursor; // increases with each add, %= size
// } bullets;

#include "hmap.h"

#define OBJ_STRUCT \
v4f pos;            \
v4f rot;            \
v4f sca;            \
v4f vel, acc;       \


typedef struct obj{
	OBJ_STRUCT
	// v4f size;

	// float life; ??

	// int shape;
	// int (*update)(struct obj*);
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

// void bullets_init(uint count); //you should not call bullets new with an int higher than this
// void bullets_quit(void);
// bullets* bullets_new(uint count);

// void bullets_update(bullets*, float delta_time);

// void bullets_draw(bullets* bs);

// bullet* bullets_add(bullets* bs, bullet* b);
// // bullet* bullet_add(bullet* b);
// void bullet_clear( bullet* b );

#endif
