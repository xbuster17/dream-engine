#ifndef __MGAME_BULLETS_H__
#define __MGAME_BULLETS_H__
// #include "mgame.h"
#include "../../de/de.h"

typedef struct bullet {
	// OBJ_STRUCT
	bool inited;
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
	float dmg;
	struct bullets* buf;
	uint type;
	void (*run)(struct bullet*);
} bullet;


typedef struct bullets{
	bullet* b;
	uint len;
	dvao* vao;

	bool shadow;
	dfbo* shadow_map;
	float dt;

	SDL_Thread* thread;
	long unsigned int threadID;
	bool tlogic_done;
	bool tshould_finish;
	// bool tmust_wait; // if true, wait until thread finish and upload, else skip upload call
	uint cursor; // increases with each add, %= len
	SDL_mutex* bmutex; // i better save it's own buffer...
	v4f* ppos;
	v4f* pctr;
	v4c* pcol0;
	v4c* pcol1;
} bullets;


extern bullet bullet_0;


#define def_bullet_blank \
	.pos = {0,0,0,0}, .col0 = {0,0,0,0}, .col1 = {0,0,0,0},    \
	.vel = {0,0,0,0}, .acc = {0,0,0,0}, .force = {0,0,0,0},    \
	.rad = 0, .tvel = 0, .tacc = 0, .lifetime = 0, .dmg = 0,         \
	.inited = false, .frame = 0, .buf = NULL

#define def_bullet_def \
	.pos = {0,0,0,0}, .col0 = {255,0,0,255}, .col1 = {255,255,255,255},    \
	.vel = {0,0,0,0}, .acc = {0,0,0,0}, .force = {0,0,0,0},    \
	.rad = 1, .tvel = 0, .tacc = 0, .lifetime = 1, .dmg = 0,         \
	.inited = false, .frame = 0, .buf = NULL

#define decl_bullet(name) \
extern bullet name;\
void name ## __run(bullet* in);

#define def_bullet(name) \
bullet name = { def_bullet_def, .run =  name ## __run, .type =  __COUNTER__ };\
void name ## __run(bullet* in)/*{your run code here}*/

//creator helpers
// #define bullet_default {}

#define def_bullet_init(name) \
	if(in)\
		if(!in->inited){\
			name.inited = true;                      \
			name.pos = in->pos;                      \
			name.vel = in->vel;                      \
			name.acc = in->acc;                      \
			name.col0 = in->col0;                    \
			name.col1 = in->col1;                    \
			name.rad = in->rad;                      \
			name.dmg = in->dmg;                      \
			name.tvel = in->tvel;                    \
			name.tacc = in->tacc;                    \
			name.lifetime = i->lifetime;             \
			name.frame = in->frame;                  \
			/*copy everything, keep buffer pointer*/ \
			bullets* buf = in->buf;                  \
			*in = name;                              \
			in->buf = buf;                           \
		}


//demos
#include "bullet_type/types.h"




void bullets_init(uint max); //you should not call bullets new with an int higher than this
void bullets_quit(void);
bullets* bullets_new(uint count, bool use_shadow, int threaded);
void bullets_free(bullets*);
int bullets_thread_fn(void* data);
void bullets_update(bullets*, float delta_time);

void bullets_update_logic(bullets* bs, float dt);
void bullets_upload(bullets* bs); // to opengl

void bullets_draw(bullets* bs);

bullet* bullets_add(bullets* bs, bullet* b);
// bullet* bullet_add(bullet* b);
void bullet_clear( bullet* b );



EMSCRIPTEN_KEEPALIVE void bullet_rng( float x, float y, float z );









#endif
