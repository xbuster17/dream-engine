#ifndef __MGAME_BULLETS_H__
#define __MGAME_BULLETS_H__
// #include "mgame.h"
#include "../../de/de.h"

typedef struct bullet {
	bool inited;
// OBJ_STRUCT
	v4f pos;
	v4c col0;
	v4c col1;

	v4f vel;
	v4f acc;
	v4f force;

	float rad;
	float tvel;
	float tacc;
	float lifetime;
	int frame;
	// bool expired;
	float dmg;
	struct bullets* buf;
	uint type;
	void (*run)(struct bullet*);

	// struct bullet* (*end)(struct bullet);
} bullet;


typedef struct bullets{
	bullet* b;
	uint len;
	dvao* vao;
	dfbo* shadow_map;

	uint cursor; // increases with each add, %= size
} bullets;


extern bullet bullet_0;


#define def_bullet_blank \
	pos:{0,0,0,0}, col0:{0,0,0,0}, col1:{0,0,0,0},    \
	vel:{0,0,0,0}, acc:{0,0,0,0}, force:{0,0,0,0},    \
	rad:0, tvel:0, tacc:0, lifetime:0, dmg:0,         \
	inited:false, frame:0, buf:NULL

#define decl_bullet(name) \
	extern bullet name;   \
	void name ## __run(bullet* in);

#define def_bullet(name) \
bullet name = { def_bullet_blank, run: name ## __run, type: __COUNTER__ };\
void name ## __run(bullet* in)

//creator helpers
// #define bullet_default {}

#define def_bullet_init(name) \
	if(in)\
		if(!in->inited){                                             \
			name.inited = true;                                             \
			name.pos = in->pos;                                             \
			name.vel = in->vel;                                             \
			name.acc = in->acc;                                             \
			name.col0 = (v4c){255,0,0,255};                                             \
			name.col1 = (v4c){255,255,255,255};                                             \
			name.rad = .3;                                             \
			name.dmg = 1;                                             \
			name.tvel = 5.0;                                             \
			name.tacc = 5.0;                                             \
			name.lifetime = 60*1000;                                             \
			name.frame = 0;                                             \
			bullets* buf = in->buf;                                             \
			*in = name;                                             \
			in->buf = buf;                                             \
		}                                             \



//demos
#include "bullet_type/types.h"
// decl_bullet(joe);
// decl_bullet(joe_spark);





void bullets_init(uint count); //you should not call bullets new with an int higher than this
void bullets_quit(void);
bullets* bullets_new(uint count);

void bullets_update(bullets*, float delta_time);

void bullets_draw(bullets* bs);

bullet* bullets_add(bullets* bs, bullet* b);
// bullet* bullet_add(bullet* b);
void bullet_clear( bullet* b );









#endif
