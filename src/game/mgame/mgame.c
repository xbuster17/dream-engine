// #include "../../de/de.h"
#include "mgame.h"
#include "hfunc0.h"
// #include "3rdparty/mongoose.h"
#include "../../3rdparty/mongoose.h"
void wschat(void );

  struct mg_mgr mgr;
  struct mg_connection *nc;
v4c sweetie16[16]={ // by GrafxKid 
	{0x1a, 0x1c, 0x2c, 0xFF},
	{0x5d, 0x27, 0x5d, 0xFF},
	{0xb1, 0x3e, 0x53, 0xFF},
	{0xef, 0x7d, 0x57, 0xFF},
	{0xff, 0xcd, 0x75, 0xFF},
	{0xa7, 0xf0, 0x70, 0xFF},
	{0x38, 0xb7, 0x64, 0xFF},
	{0x25, 0x71, 0x79, 0xFF},
	{0x29, 0x36, 0x6f, 0xFF},
	{0x3b, 0x5d, 0xc9, 0xFF},
	{0x41, 0xa6, 0xf6, 0xFF},
	{0x73, 0xef, 0xf7, 0xFF},
	{0xf4, 0xf4, 0xf4, 0xFF},
	{0x94, 0xb0, 0xc2, 0xFF},
	{0x56, 0x6c, 0x86, 0xFF},
	{0x33, 0x3c, 0x57, 0xFF}	
};

// #define hmap_res 22
#define hmap_res 40
// #define hmap_res 400
// #define hmap_res 64
// #define hmap_res 512
// #define hmap_res 80
// #define hmap_res 100
// #define hmap_res 128
struct game G;
float* sndchunk;
mpts* pts_test=NULL;
void mgame_init(void);
void mgame_quit(void);
void mplayer_update(void);
// float hfunc(float,float); // hma function

dfbo* game_fbo=NULL;
int MainFboMinScale=1;
// int MFBO_FILTER=GL_NEAREST;
int MFBO_FILTER=GL_LINEAR;


void mgame_quit(void){
	bullets_free(G.enemy_bullets);
	bullets_free(G.player_bullets);
	bullets_quit();
	free(sndchunk);
	pts_free(pts_test);
	pts_quit();
  mg_mgr_free(&mgr);

}




// player
mcyl* plym=NULL;
float plymf[40*40];

bool track_enm = false;
float fric = 1.33;
float Grav = -9;
// float tvel = 8.f;
float tvely = 10.f;
float rotspeed = (PI/90);
bool mouse_grab=0;
float Pjumpspeed = 5;//9;//5; //14*4
int Pgrounded = 1;
int Pdashing_frames = 12;
float Pdashing_speed = 3;
int Pdashing = 0;
// float pspeed = 1.25f/3;
float pspeed = 1.25f/2.0;
float pslowspeed = 1.25f/5; // pspeed/2;
struct dfinger* lthumb = NULL;
struct dfinger* rthumb = NULL;

mcyl* skysph=NULL;
dshd* skyshd=NULL;
char *skyvs, *skyfs;
static struct {
	v4f pos;
	v4f col;
} Lsky[4];
v4f Lskytar;

mcyl* monk=NULL;
float monkrad[20*40];
void remonk(void){
	float v=0; int x = 40; int y=20;
	for(int i=0;i<y;i++) for(int j=0;j<x;j++){
		v=sin(((float)j+G.frame/30.0)/x * 10.f*M_PI) /2.0;
		v*=v;
		v+=.5;
		v*=2+sin((float)i/(y/16.0) + G.frame/40.0 /*- y/2.0*/);
		v/=2.0;
		// if(i>y/2.0)
		// v*=-sin(((float)i+G.frame/2.0)/(y-1) *PI);
		// else
		// v*=sin(((float)i-G.frame/2.0)/(y-1) *PI);
		// v*=v;
		// v+=.15;
		if(i==0 || i==y-1) v=0;
		v=CLAMP(v, .1, 1.1);
		// if(j<x/2.0 +1) v/=3.0;
		// v+=((float)j)/x;
		// v*=fabs((j-x/2.0))/25.0;
		// v*=fabs((i-y/2.0))/25.0+1;
		// v=lerp(v, 0.0, fabs(i-y/2.0)/(y/2.0));
		// if(j==x/2) v=1/(j+1);
		// if(i>75)v/=(i-70)/5.f;
		monkrad[i*x+j] = v;
	}
	if(!monk) 
		monk=mcyl_new(monkrad, x,y);
	else
		mcyl_regen(monk, monkrad);
}






void mgame_init(void){
 wschat();

	dvsync(1);
	G.timePrev=0;
	G.time=SDL_GetPerformanceCounter();
G.main_fbo_scale = 1;

/*
 fbo, stsize, slen, hmsize, bscount, ptscount, shapelod */
// {4,   256,    5,    20,      2000,   200,      1        }
// {2,   512,    6,    20,      4000,   400,      1        } // mobile
// {2,   512,    6,    32,      4000,   500,      1        } // mobile
// {1,   1024,   8,    40,      4000,   500,      2        }
// {1,   1024,   8,    64,      10000,  1000,     2        } 
// {1,   2048,   12,   100,     10000,  1000,     4        }
// {1,   2048,   12,   128,     60000,  5000,     10       }

	pts_init();
	pts_test = pts_new();

game_fbo = dfbo_new(De.size[0]/G.main_fbo_scale, De.size[1]/G.main_fbo_scale);
v2i_print(De.size);
v2f_print(De.fsize);
assert(game_fbo);
dtex_filter(game_fbo->color, MFBO_FILTER, MFBO_FILTER);

// G.bg=dfbo_new(De.size[0]/G.main_fbo_scale, De.size[1]/G.main_fbo_scale);
G.bg=dfbo_new(De.size[0]/2, De.size[1]/2);
dtex_filter(G.bg->color, MFBO_FILTER, MFBO_FILTER);
G.bg_index=7;


G.cam_in_hmap=false;

remonk(); // test cyl

float skyv[40*40];
{	
	float v=0; int x = 40; int y=40;
	for(int i=0;i<y;i++) for(int j=0;j<x;j++){
		v=sin(((float)j+G.frame/30.0)/x * 10.f*M_PI) /2.0;
		v*=v;
		v+=.5;
		v*=2+sin((float)i/(y/16.0) + G.frame/40.0 /*- y/2.0*/);
		v/=2.0;
		// if(i>y/2.0)
		// v*=-sin(((float)i+G.frame/2.0)/(y-1) *PI);
		// else
		// v*=sin(((float)i-G.frame/2.0)/(y-1) *PI);
		// v*=v;
		// v+=.15;
		if(i==0 || i==y-1) v=0;
		v=CLAMP(v, .1, 1.1);
		// if(j<x/2.0 +1) v/=3.0;
		// v+=((float)j)/x;
		// v*=fabs((j-x/2.0))/25.0;
		// v*=fabs((i-y/2.0))/25.0+1;
		// v=lerp(v, 0.0, fabs(i-y/2.0)/(y/2.0));
		// if(j==x/2) v=1/(j+1);
		// if(i>75)v/=(i-70)/5.f;
		// monkrad[i*x+j] = v;
		skyv[i*x+j] = -100;
	}
}
skysph=mcyl_new(skyv, 40, 40);
skysph->sph = 1;
skysph->mesh->pos = v4f_0;
skysph->mesh->pos[3] = 1;
skysph->mesh->sca *= 1.f;
skysph->mesh->lp1 = (v4f){0,1,0,0};
skysph->mesh->lp2 = (v4f){0,1,0,0};
skysph->mesh->lp1[3] = 1;
skysph->mesh->lc1 = (v4f){1.0,1.0,1.0,1.0};
skysph->mesh->lc2 = (v4f){1.0,1.0,1.0,1.0};
skysph->mesh->li1 = 1.0;
skysph->mesh->li2 = 1.0;
skyshd=dshd_new(skyvs, skyfs);
mcyl_update(skysph);
G.suncol=(v4f){.85, .75, .71, 1};


dshd_unif(skyshd, "l[0].pos", &Lsky[0].pos);
dshd_unif(skyshd, "l[0].col", &Lsky[0].col);
dshd_unif(skyshd, "l[1].pos", &Lsky[1].pos);
dshd_unif(skyshd, "l[1].col", &Lsky[1].col);
dshd_unif(skyshd, "l[2].pos", &Lsky[2].pos);
dshd_unif(skyshd, "l[2].col", &Lsky[2].col);
dshd_unif(skyshd, "l[3].pos", &Lsky[3].pos);
dshd_unif(skyshd, "l[3].col", &Lsky[3].col);

dshd_unif(skyshd, "tar", &Lskytar);
dshd_unif(skyshd, "eye", &De.cam.pos);
dshd_unif(skyshd, "fov", &De.cam.fov);
dshd_unif(skyshd, "time", &G.ftime);
dshd_unif(skyshd, "sunp", &G.sunpos);
dshd_unif(skyshd, "sunc", &G.suncol);

dshd_unif(skyshd, "VP", &De.cam.vp);
dshd_unif(skyshd, "M", &skysph->mesh->m);
dshd_unif(skyshd, "view", &De.cam.view);
dshd_unif(skyshd, "proj", &De.cam.proj);

	bullets_init( num_enemy_bullets );
	G.enemy_bullets  = bullets_new(num_enemy_bullets, true, 0);
	G.player_bullets = bullets_new(num_player_bullets,false,0);
	G.dt = 0.016f;
	G.player.pos = (v4f){1,1,1,0}*0;
	G.player.rot = (v4f){0,0,0,0};
	G.player.vel = v4f_0;
	G.player.rad = .075/2;
	// G.player.rad = .25/4.0;
	// G.player.height = .025f;
	G.player.height = .025f;

	// G.hmap0 = hmap_new(hmap_res,hmap_res, hfunc, 0);
	G.hmap0 = hmap_new(hmap_res,hmap_res, hfunc, HMAP_DYNAMIC | HMAP_FIXED);



// gen sounds
	int smplen = De.sample_rate;

	sndchunk = malloc(smplen * 2 * sizeof(float));

	for (int i = 0; i < smplen*2; ++i){
		sndchunk[i] = 0.0;
	}

	for (int i = 0; i < smplen*2; ++i){
		float c = (float)i;
		c /= De.sample_rate;
		c = PI*c;
		float r = sin(440*c);
		// float m = fmodf(2*c, 1.f) * sin(1.3*c);
		float m = sin(3*c);
		r = r*m;
		// r*=r*r;
		r=pow(r,131)/8;


		sndchunk[i] = r;
		i++;
		sndchunk[i] = r;
	}

	dsnd* testpsnd = dsnd_new(sndchunk, smplen * 2 * sizeof(float));

	dsnd_fade(dsnd_playl(testpsnd, -1), 1000);

	smplen/=8;
	float* blip = malloc(4 * smplen*2);
	for (int i=0; i<smplen*2; i++){
		float s = i;
		s /= De.sample_rate;
		s *= PI;
		float fq = ((smplen)-i)/2;
		s *= fq;
		s=sin(s);
		s=s*s;
		// s=fabs(s);
		// s-=.5; s*=2;// range[-1,1]
		s*=(smplen*2.0-i)/smplen; // falloff
		s*=.025; // volume
		blip[i] = s;
		blip[++i] = s;
	}
	G.phitsnd = dsnd_new(blip, smplen *4*2);

	// G.screensmp = malloc(De.size[0]* De.size[1]* sizeof(float) * 2);
}










void mgame_loop(void);

void mgame_main(void){
	mgame_init();

// de_run(mgame_loop);
// de_main_loop(mgame_loop);
#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(mgame_loop, 0, 1);
#else
	mgame_loop();
#endif

	mgame_quit();
}





int frame = 0;
// struct enemy enemy={.pos=v4f_0, .hp=500, .vel=v4f_0};
v4f spinning_dmk_dir = {1,-10,0,0};
bool dmktsel = 0;
// G.enm[0].pos = (v4f){0,0,3,0};//v4f_0;
// G.enm[0].hp = 500;//v4f_0;
// G.enm[0].vel = v4f_0;
void mgame_loop(void){
	DE_GAME_LOOP(){
mg_mgr_poll(&mgr, 0);

		G.timePrev=G.time;
		G.time=SDL_GetPerformanceCounter();
		G.dt = (float)((G.time - G.timePrev)*1000.0 / (double)SDL_GetPerformanceFrequency() )/1000.f;
		G.ftime+=G.dt;

		if(dusek(DK_ESC) || dusek(DK_BACK)) De.should_quit=1;
		frame++;
		G.clear_color = (v4f){.2, .08, .202}/2.0;
		G.clear_color = (v4f){0,0,0,0}*1;
		// G.clear_color = (v4f){0.5,0,0.5,0};
		dclear_color(G.clear_color);

//sky light struct test
// for(int i=0;i<4;i++){
// Lsky[i].pos = v4f_rand();
// Lsky[i].col = v4f_rand();
// }
// Lsky[G.frame%4].pos[0] = sin(G.frame/1000.0);

		// v2i_print(De.size/2);
		// dfbo_resize(game_fbo, De.size[0]/2, De.size[1]/2);
	G.main_fbo_scale=1.0;
	if(De.size[0]/G.main_fbo_scale!=game_fbo->x || De.size[1]/G.main_fbo_scale!=game_fbo->y){
		dfbo_free(game_fbo);
		game_fbo= dfbo_new(De.size[0]/G.main_fbo_scale, De.size[1]/G.main_fbo_scale);
		dtex_filter(game_fbo->color, MFBO_FILTER, MFBO_FILTER);
	}
	// if(G.main_fbo_scale > MainFboMinScale) G.main_fbo_scale /= G.frame%10 ? 1: 2;// test


		dfbo_bind(game_fbo);
		// dcam_update();
		dclear(0);
		// ddepth(1,0);
		// dblend(1);
		// dtex_draw(G.bg->color);
/*if(!(G.frame%60)) */remonk();

if(G.enm[0].hp<0) {
	pts_add(pts_test, G.enm[0].pos, 1000);
	G.enm[0].pos=v4f_rand()*10;
	G.enm[0].pos[3]=0;
	G.enm[0].hp=500;
	G.enm[0].hp=500;
	// G.enm[0].pos*=0;
}


// if(dmktsel) { // pull player
// 	G.player.acc += v4f_normalize((G.enm[0].pos-G.player.pos)/5);
// } else 
// 	G.player.acc *=0;/* move to bullet type*/

//sphere dmk
if (frame%160 == 0) {

	dmktsel = (dmktsel+1) % 2;

	G.enm[0].vel = dmktsel? (G.enm[0].pos - G.player.pos) : v4f_0;
	G.enm[0].vel[1] = 0;
	G.enm[0].vel = v4f_normalize(G.enm[0].vel)*-.5f;

	// G.enm[0].vel[0] = dmktsel? (((1.f*rand())/INT_MAX)-.5f)*2 : 0;
	// G.enm[0].vel[2] = dmktsel? (((1.f*rand())/INT_MAX)-.5f)*2 : 0;

#if 1
	if(dmktsel) {
		int il = 16; // density
		int jl = il;
		m4f rot = m4f_rotation_y(rand()/PI);
		for (int i = 0; i < il; ++i){
			for (int j = 0; j < jl; ++j){
				float ir = PI * i/(il-1);
				float jr = PI * j/(jl-1);
				bullet* b = bullets_add(G.enemy_bullets, &glider);
				b->inited=1; // avoid using default values
				b->pos=G.enm[0].pos;
				// b->pos[1]=1.75;
				v4f vel = v4f_0;
				vel[0]=cos(jr) * sin(ir);
				vel[1]=sin(jr) * sin(ir)*(rand()%256)/256.f;
				vel[2]=cos(ir);
				b->vel = v4f_mmul(vel, rot);
				// b->vel *= .1f;
				// b->tvel = 2;
				b->rad = .05;
				b->acc = v4f_0;
				b->col1 = (v4c){255,255,255,255};
				b->col0 = (v4c){255,2,255,255};
				b->frame = -999090;
			}
		}
	}
#endif
}

//3d
if(!(frame%2)){
	bullet* b = bullets_add(G.enemy_bullets, &glider);
	b->pos=G.enm[0].pos;
	b->pos[1]+=.57;
	// b->vel = v4f_rand()*2-1;
	float t = G.frame/PI*(3+dmktsel+1);
	t*=dmktsel+1;
	// t*=4;
	b->vel = (v4f){cos(t),0,sin(t),0};
	b->vel *= fabsf(sinf(t)) + .5f;
	b->vel /= 2;
	b->vel[1] = 1;
	b->acc[1]=-1;
	// b->acc[1]+=Grav;
	b->rad = .075;
	b->col1 = (v4c){255,255,255,255};
	// b->col1 = v4c_rand();
	// b->col0 = (v4c){(dmktsel%2)*255,255,0,255};
	b->col0 = sweetie16[(frame/3) % 16];
	// b->col1 = b->col0;

}


// spinning dmk
// if(!(frame%2)){
if(true){
	for (int i = 0; i < 1; ++i){
		bullet* tor = bullets_add(G.enemy_bullets, &glider);
		tor->pos = G.enm[0].pos;
		tor->vel = spinning_dmk_dir *0.7f;//* 1.5;
		tor->acc[1] = Grav;
		tor->rad = 0.03+(dmktsel%2)*0.008;
		tor->col0[2] = (dmktsel%2)*155;
		spinning_dmk_dir = v4f_mmul(spinning_dmk_dir, m4f_rotation_y(PI/6.0));
	}
	if(dmktsel) spinning_dmk_dir = v4f_mmul(spinning_dmk_dir, m4f_rotation_y(-PI/(6.0/4)+0.12));
	else spinning_dmk_dir = v4f_mmul(spinning_dmk_dir, m4f_rotation_y(-PI/(6.0/3)+0.11));
}



G.enm[0].pos += G.enm[0].vel * G.dt;
// G.enm[0].pos[1] = MAX(G.player.pos[1], hmap_eval(G.hmap0, G.enm[0].pos[0], G.enm[0].pos[2])) + 0.1;
G.enm[0].pos[1] = hmap_eval(G.hmap0, G.enm[0].pos[0], G.enm[0].pos[2])+0.115;

// G.enm[0].pos = G.enm[0].pos;
// G.enm[0].vel = G.enm[0].vel;
// G.enm[0] = enemy;
bullet*	pt = bullets_add(G.enemy_bullets, &playeraura);
	pt->rad = .115;
	pt->pos = G.enm[0].pos;
	pt->vel = v4f_0;
	pt->col0[0] = 255;  pt->col1[0] = 255;
	pt->col0[1] = 0;    pt->col1[1] = 0;
	pt->col0[2] = 0;    pt->col1[2] = 0;
	pt->col0[3] = 120;  pt->col1[3] = 255;

//end enemy tmp





		pts_update(pts_test);

		mplayer_update();

// monk->pos=(v4f){5,5,5,1};
monk->mesh->pos = G.enm[0].pos;
monk->mesh->pos[1] += .25;
// monk->mesh->pos = v4f_0;
// monk->mesh->pos[1] = 5.5;
// monk->mesh->pos[3] = 1;

monk->mesh->lp1 = G.sunpos; monk->mesh->lp1[3] = 0;
// monk->mesh->lp2 = G.player.pos; monk->mesh->lp1[3] = 1;
// monk->mesh->lp1[1] += 1;
monk->mesh->li1 = 1.0;
// monk->mesh->lp1 = G.enm[0].pos; monk->mesh->lp2[3] = 1;
// monk->mesh->lp1[1]+=2;
// monk->mesh->lp2 = G.enm[0].pos; monk->mesh->lp2[3] = 0;
// monk->mesh->lp2[1]+=10.0;
monk->mesh->lp2 = G.sunpos;
monk->mesh->li2 = 1.0;
// monk->mesh->rot[1] += .01;
// monk->mesh->rot[2] = PI/2;
// monk->mesh->rot[0] = -atan2((G.player.pos-G.enm[0].pos)[0], (G.player.pos-G.enm[0].pos)[2]) + PI/2.0;
// monk->mesh->rot[0] = -atan2(G.enm[0].vel[0], G.enm[0].vel[2]);
// v3f_ang((G.player.pos-G.enm[0].pos) * (v4f){1,0,1,0}, (v4f){0,0,-1,0});
monk->mesh->sca = (v4f){.5,.5,.5,1};
// monk->mesh->l1s = 2.01;
monk->sph=false;
mcyl_update(monk);

		bullets_update(G.enemy_bullets, G.dt);
		bullets_update(G.player_bullets, G.dt);

		hmap_update(G.hmap0, G.player.pos);






// update background
	// ddepth(1,GL_GREATER);
	ddepth(1,0);
	dfbo_bind(G.bg);
	dcam_update();
	dclear(0);
	skysph->mesh->pos = G.player.pos;
	mcyl_update(skysph);

	dshd_bind(skyshd);
	dshd_update(skyshd);
	// dvao_draw_quad();

	dvao_draw(skysph->mesh->vao, 0,0,0);
	// ddepth(0,0);
	// dcullf(0);
	// mcyl_draw(skysph);
	// dfbo_bind(0);



// render

	dfbo_bind(game_fbo);
	dcam_update();
// Lsky[0].pos = v4f_mmul(G.enm[0].pos, De.cam.vp);
// Lsky[0].col = (v4f){1,0,0,0};

	// dclear_color(G.clear_color);
	// dclear(0);
	// dcullf('b');
	// ddepth(1,0);
	// dcullf(0);
	// ddepth(0,0);
	// dblend(0);

// dcullf(0);
dblend(1);
// mcyl_draw(plym);

	bullets_draw(G.enemy_bullets);
	// dfbo_bind(game_fbo);
	// dcam_update();
	// dcullf('b');
	// ddepth(1,0);
	// dblend(0);
	// dcullf(0);
	// ddepth(0,0);


		pts_draw(pts_test);


		hmap_draw(G.hmap0);

		bullets_draw(G.player_bullets);

dblend(0);
mcyl_draw(monk);



/*blit to screen*/
	dfbo_bind(0);
	dcam_update();
	dclear_color((v4f){1,0,1,1});
	dclear(0);

	dblend(1);
	// dcullf(0);
	ddepth(0,0);
	dtex_draw(G.bg->color);
	dfbo_draw(game_fbo);
	// dfbo_draw_depth(game_fbo, 1.0);

		G.frame++;
		// G.timePrev=G.time;
		// G.time=SDL_GetPerformanceCounter();
		// G.dt = (double)((G.time - G.timePrev)*1000 / (double)SDL_GetPerformanceFrequency() )/1000.0;
		// G.dt=0.016;
		if(G.dt>.9) DE_LOG("%f",G.dt);

// Uint64 NOW = SDL_GetPerformanceCounter();
// Uint64 LAST = 0;

   // LAST = NOW;
   // NOW = SDL_GetPerformanceCounter();

		#ifdef __EMSCRIPTEN__
		break;
		#endif

	}
	// de_main_tick();


}


















/*
#define HFS 256
bool _once = false;
int hfv_size = HFS;
// char hfv[128*128];
float hfv[HFS*HFS];
#undef HFS

void init_hfunc( void );

float hfv_read(float x, float y){
	x+=hfv_size/2;
	y+=hfv_size/2;
	if(x > hfv_size-1) return 0;
	else if(x < 0) return 0;
	else if(y > hfv_size-1) return 0;
	else if(y < 0) return 0;
	else
		return hfv[ (int)x * hfv_size + (int)y ];
}

void init_hfunc( void ){
	for (int i = 0; i < hfv_size; ++i){
		for (int j = 0; j < hfv_size; ++j){
			hfv[i * hfv_size + j] = 0;
		}
	}
	// drunk walk
	int steps = 5000;
	int d;
	double f;
	long unsigned int c;
	v2i w = {hfv_size/2, hfv_size/2};
	for (int i = 0; i < steps; ++i){
		// d=rand()%4;
		// d=hash2(i,steps)%4;
		f=tan(i*1.1)*19349663.0;
		d=(int)f %4;
		c=hash2(i,d);
		d=c %4;
		if(d==0) w[0] = MIN(w[0]+1, hfv_size-1);
		if(d==1) w[0] = MAX(w[0]-1, 0);
		if(d==2) w[1] = MIN(w[1]+1, hfv_size-1);
		if(d==3) w[1] = MAX(w[1]-1, 0);
		hfv[ w[0]*hfv_size + w[1] ]+=.15;
		hfv[ MIN(w[0]+1,hfv_size-1)*hfv_size + w[1] ]+=.15;
		hfv[ w[0]*hfv_size + MIN(w[1]+1,hfv_size-1) ]+=.15;
		hfv[ MIN(w[0]+1,hfv_size-1)*hfv_size + MIN(w[1]+1,hfv_size-1) ]+=.15;
	}
}


// function for the hmap
float hfunc(float x,float y){
	if (!_once){
		init_hfunc();
		_once = true;
	}
	// return 5* sin(x*y/10 + G.frame / 50.f);
	// return 10* sin(fmodf(x,7) * fmodf(y,11) / (7*11) );
//sample walker
	float hfvv = hfv_read(x,y);

	// if (x>-2 && x<6 && y>-1 && y<6) return 4;

	// return 0;

	v2d p = {x,y};
	// return -v2d_len(p)/2 + noise2((p+G.frame/50.f)/10.f);


	// v4f tp = v4f_mmul((v4f){x,0,y,0}, m4f_inverse(G.hmap0->m));
	// v4f tp = ((v4f){x,0,y,0})/G.hmap0->sca;
	// v2d p = {tp[0], tp[2]};

	float yf = G.frame/50.f;
	// float yf = 0;
	v4d p3 = {p[0], p[1], yf};
	float n1 = noise3(p3/10.f)*3;
	n1*=n1;

	// n1= n1-v2d_len(p)/2;
	if (fmodf(v2f_len(
			(v2f){x,y} - (v2f){0,3}
		// (v2f){x,y} - (v2f){G.enm[0].pos[0], G.enm[0].pos[1]}
			), 5) <2
		) n1 = n1 + 1;
	
	n1 += v2d_len(p)>15? -3: 0;
	if(hfvv != 0.0)
		return -2 + hfvv / 8.0;
	return n1;

	float h = n1;

	if(h<.5) {
		p[1] = y+G.frame/10.f;
	if(h<.05)
		h -= fabs(noise2(p/25));
	else h -= fabs(noise2(p/125));
	}


	return h;
}


*/






























void mplayer_update(void){
if(!plym) plym=mcyl_new(plymf, 40, 40);
plym->sph=false;
for(int i=0; i<40;i++)
for(int j=0; j<40;j++){
	if(i>=30)//head
		// plymf[i*40+j] = v2f_len((v2f){MAPVAL(i,0,40,0,1),MAPVAL(j,0,40,0,1)}/40.0 - (v2f){0, 35+3.0}/40.0) /20.0;
		plymf[i*40+j] = (.5f-fabs((i-30)/10.f -.5f)) /10.0 ;
	else if(i>25) // body
		plymf[i*40+j] = 0;//fabsf(noise2p((v2d){(j+sin(G.frame/10.0))/20.0,(i+v4f_len2(G.player.vel*2))/20.0}, (v2i){2,100}));
	else if (i==20)
		plymf[i*40+j] = 0;//fabsf(noise2p((v2d){(j+sin(G.frame/10.0))/20.0,(i+v4f_len2(G.player.vel*2))/20.0}, (v2i){2,100}));
	else //feet
		// plymf[i*40+j] = abs(sin(8*PI*j/40.0))*(1.0-G.player.vel[1])*MAPVAL(i,0,20,1.f,0.f);
		plymf[i*40+j] = ((pow(sin(5* (j/40.f)*PI ), 2.f)+1.5) * (pow(sin(1*(i/(30.f+v3f_len(G.player.vel)))*PI), 2.f))) /10.f;

	if(i==39)plymf[i*40+j]=0;
	if(i==0)plymf[i*40+j]=0;
}
mcyl_regen(plym, plymf);


// //touch test
// 	for (int i = 0; i < DE_FINGER_MAX; ++i){
// 		if(De.finger[i].down){
// 			bullet* b = bullets_add(G.enemy_bullets, &playertrace);
// 			b->pos[0] = De.finger[i].abs[0]*10;
// 			b->pos[1] = 0;
// 			b->pos[2] = De.finger[i].abs[1]*10;
// 		}
// 	}
	static int pflymode=0;
	if (dusek(DK_CAPSLOCK)) pflymode=(pflymode+1)%2;


// player_update();
	// if(dusek(DK_ESC)) { mouse_grab = (mouse_grab+1) %2;}
	if(dusek('q')) { 
		mouse_grab = (mouse_grab+1) %2;
		dmouse_grab(mouse_grab);
	}

//	#ifndef __EMSCRIPTEN__

	// if(mouse_grab) dmouse_grab(1);
	// else dmouse_grab(0);

//	#endif

	bool jump_input = 0;
	bool slow_input = 0;


//player input
	v4f pdir = v4f_0;
	if     (dgetk('w') > dgetk('s')) pdir[2] = -1;
	else if(dgetk('s') > dgetk('w')) pdir[2] =  1;
	if     (dgetk('d') > dgetk('a')) pdir[0] =  1;
	else if(dgetk('a') > dgetk('d')) pdir[0] = -1;
	if     (dgetk(' ') > dgetk('c')) pdir[1] =  1;
	else if(dgetk('c') > dgetk(' ')) pdir[1] = -1;

// if(mouse_grab){
	if     (dgetk(DK_UP) > dgetk(DK_DOWN)) pdir[2] = -1;
	else if(dgetk(DK_DOWN) > dgetk(DK_UP)) pdir[2] =  1;
	if     (dgetk(DK_RIGHT) > dgetk(DK_LEFT)) pdir[0] =  1;
	else if(dgetk(DK_LEFT) > dgetk(DK_RIGHT)) pdir[0] = -1;
// } else {
	// if(dgetk('j') > dgetk('l')) G.player.rot[1] +=  rotspeed;
	// else if(dgetk('l') > dgetk('j')) G.player.rot[1] += -rotspeed;
	if     (dgetk('k') > dgetk('i')) G.player.rot[0] +=  rotspeed;
	else if(dgetk('i') > dgetk('k')) G.player.rot[0] += -rotspeed;

	if     (dgetk('j') > dgetk('l') || dgetk('z') > dgetk('x')) G.player.rot[1] +=  rotspeed;
	else if(dgetk('l') > dgetk('j') || dgetk('x') > dgetk('z')) G.player.rot[1] += -rotspeed;
// }

	if(dusek('e')) track_enm = !track_enm;

//touch controls
	if(lthumb == NULL){
		lthumb = dfinger_in_area(0,0, .5,1); //left side of screen
		if(lthumb) lthumb->captured = true;
	}
	if(rthumb == NULL){
		rthumb = dfinger_in_area(.5,0, 1,1); //right side of screen
		if(rthumb) rthumb->captured = true;
	}

	if(lthumb){
		if(lthumb->down){
			pdir[0] = lthumb->rel[0];
			pdir[2] = lthumb->rel[1];

			pdir[0] *= 10;
			pdir[2] *= 10;

		} else lthumb=NULL;
	}
	if(rthumb){
		if(rthumb->down){
			G.player.rot[1] +=-rthumb->rel[0] / 4;
			G.player.rot[0] += rthumb->rel[1] / 4;
		} else rthumb = NULL;
	}
	if(rthumb == lthumb){
		rthumb = NULL;
		lthumb = NULL;
	}



	// G.player.rot[0] = CLAMP(G.player.rot[0], -PI/2, PI/2);
	G.player.rot[0] = CLAMP(G.player.rot[0], -PI/4, PI/2);

	// G.player.rot = v4f_mmul((v4f){0,0,1,1}, m4f_look_at(G.player.pos, G.enm[0].pos, (v4f){0,1,0,0}) );


// lerp focus on enemy fixme
// lerped does not autocorrect edge case
	// G.player.rot[1] = lerp(G.player.rot[1] , -atan2(
	// 	G.enm[0].pos[0] - G.player.pos[0] ,
	// 	G.enm[0].pos[2] - G.player.pos[2]
	// ), .07);

	if(track_enm)
	if (v2f_len((v2f){G.enm[0].pos[0] - G.player.pos[0], G.enm[0].pos[2] - G.player.pos[2]}) > .1f)
		G.player.rot[1] = -atan2(G.enm[0].pos[0] - G.player.pos[0], G.enm[0].pos[2] - G.player.pos[2]);

	if(mouse_grab){
		G.player.rot[0] += De.mouse.rel[1] / 5.f * G.dt;
		G.player.rot[1] -= De.mouse.rel[0] / 5.f * G.dt;
	}



// dont normalize or clamp jump to rotation of input
	v4f npdir = v3f_normalize(pdir); // circular clipping
	float minx = MIN(-npdir[0], npdir[0]);
	float maxx = MAX(-npdir[0], npdir[0]);
	float minz = MIN(-npdir[2], npdir[2]);
	float maxz = MAX(-npdir[2], npdir[2]);
	pdir[0] = CLAMP(pdir[0], minx, maxx);
	pdir[2] = CLAMP(pdir[2], minz, maxz);


//jump
	float pjumpspeed = 0;
	// if (dgetk(' ')) jump_input=true;

	if(De.mouse.b1){
		jump_input = 1;
		De.mouse.b1 = 0;
	}
	if(pflymode){
		if (dgetk(' ')) jump_input=true;
	} else
		if (dusek(' ')) jump_input=true;

	if (dusek(DK_VOL_DOWN)) jump_input=true;
	if (De.finger_click && (De.finger_click_pos[0]>.5)) {
		De.finger_click = 0;
		jump_input=true;
	}

//jump
	if(jump_input) {
		pjumpspeed = Pjumpspeed;
		if(Pgrounded){
			pdir[1] = pjumpspeed;
			Pgrounded = 0;
		} else{ // dash
			pdir[1] = 0;
			
			// pdir = Pdashing_speed*pdir;
			Pdashing = Pdashing_frames;
			// Pdashing=CLAMP(Pdashing, 0, 160);
		}
		// dsnd_play(G.phitsnd);
	}

	if (Pdashing) {
		Pdashing -=1;
		pdir *= Pdashing_speed;
	}
//gravity
	else G.player.vel[1] += Grav*G.dt;
	if(pjumpspeed) //stop falling
		G.player.vel[1] = MAX(0, G.player.vel[1]);




// apply rotation to input
	pdir = v4f_mtmul(pdir, m4f_rotation_y(G.player.rot[1]) );




//slow
	if(De.mouse.b2 || dgetk(DK_LSHIFT) || dgetk(DK_RSHIFT) || dgetk(DK_RCTRL) || dgetk(DK_LCTRL)){
		slow_input = 1;
	}


// apply speed
	float pyd = pdir[1];
	pdir *= slow_input? pslowspeed : pspeed;
	pdir[1]=pyd*pspeed; // jump normally while slow




	G.player.vel += pdir;
	G.player.vel += G.player.acc * G.dt;

	// G.player.vel[0] = CLAMP(G.player.vel[0], -tvel, tvel);
	G.player.vel[1] = CLAMP(G.player.vel[1], -tvely, tvely);
	// G.player.vel[2] = CLAMP(G.player.vel[2], -tvel, tvel);

	G.player.pos += G.player.vel * G.dt;


// ground
	// float h = hmap_eval(G.hmap0, G.player.pos[0], G.player.pos[2]);
	float nx,nz;
	float h = hmap_evald(G.hmap0, G.player.pos[0], G.player.pos[2], &nx, NULL, &nz);


// float prevh=G.player.pos[1];


	if(G.player.pos[1] <= h + G.player.rad+G.player.height){
		G.player.pos[1] = h + G.player.rad+G.player.height;
		
		// G.player.pos[1] += .25*(h + G.player.rad+G.player.height - G.player.pos[1]); // smooth
		// G.player.pos[1] += 1; // smooth

		G.player.acc[1] = 0;
		Pgrounded+=1;
		G.player.vel[1] = 0;
	} else Pgrounded=0;

	G.player.vel[0] /= fric;
	G.player.vel[2] /= fric;

	// if(Pgrounded)G.player.vel[1] /= fric;
	// if( Pgrounded ) G.player.acc[0] /= fric;
	// if( Pgrounded ) G.player.acc[2] /= fric;

	     if(Pgrounded && (nx > .75)) { /*if(G.player.vel[0]<0) G.player.vel[0]*=-1;*/ G.player.vel[0] += nx/**G.dt*/;}// * G.dt;
	else if(Pgrounded && (nx <-.75)) { /*if(G.player.vel[0]>0) G.player.vel[0]*=-1;*/ G.player.vel[0] += nx/**G.dt*/;}// * G.dt;
	     if(Pgrounded && (nz > .75)) { /*if(G.player.vel[2]<0) G.player.vel[2]*=-1;*/ G.player.vel[2] += nz/**G.dt*/;}// * G.dt;
	else if(Pgrounded && (nz <-.75)) { /*if(G.player.vel[2]>0) G.player.vel[2]*=-1;*/ G.player.vel[2] += nz/**G.dt*/;}// * G.dt;
	// else if(Pgrounded && (nx + nx + nz + nz < .08) ) G.player.vel[1] = 0;
	if(Pgrounded ) G.player.vel[1] /= fric;
	// v4f_print(G.player.pos);
	// printf("%f %f\n", nx,nz);

// CAMERA
//fps view
	// De.cam.pos = G.player.pos;
	// De.cam.pos[1] += .09f;
	// De.cam.rot = G.player.rot;
	// De.cam.fov = PI/2.0;
//tps view
#if 1
	De.cam.fov = PI*25.0;
	// v4f camtrans = {0,.5,-16,0};
	// v4f camtrans = {0,1.75,-4.96,0};
	v4f camtrans = {0,1.5,2.96,0};
	
	static float zoom = 0.975f;
	v4f phead ={0,.1,0,0};
	// zoom+=De.mouse.scroll[1]*.1;
	// if(De.mouse.scroll_total[1]>=3){
	// 	De.mouse.scroll_total[1]=3;
	// 	camtrans[1]=.25;
	// 	camtrans[0]=0;
	// 	camtrans[2]=.1;
	// } else 
	float zoom_target = (-De.mouse.scroll_total[1]+3)*.05;
	
	zoom = MAX(LERP(zoom, zoom_target, .5),.0001f) ;
	// De.mouse.scroll[1]=0;

	camtrans *= zoom;


	v4f camrot = {G.player.rot[0]+.25, G.player.rot[1], 0,0};

	De.cam.pos = G.player.pos+phead + v4f_mtmul(camtrans, m4f_rotation(camrot));
	
	// De.cam.pos = G.player.pos + (v4f){0,30,10,0};
	De.cam.tar = G.player.pos+phead;
	// De.cam.tar[1]+=10.0;
	// De.cam.pos = G.sunpos;

	if (De.cam.pos[1] < hmap_eval(G.hmap0, De.cam.pos[0], De.cam.pos[2]))
		G.cam_in_hmap=true;
	else 
		G.cam_in_hmap=false;

	// cam collides with hmap fixme todo
	// if(G.cam_in_hmap)
	// 	De.cam.pos[1] = MAX(De.cam.pos[1], hmap_eval(G.hmap0, De.cam.pos[0],De.cam.pos[2])+.15);
	// De.cam.pos = G.player.pos + v4f_mtmul(camtrans, m4f_rotation(-G.player.rot);
	// De.cam.rot = G.player.rot * (v4f){0,1,0,0};

	// De.cam.rot = camrot;
#endif
	dcam_update();


// Lskytar= De.cam.pos + v4f_mtmul((v4f){0,0,-10,0}, m4f_rotation(camrot));
// Lskytar= G.player.pos;
// Lskytar= De.cam.pos + De.cam.dir;
Lskytar= De.cam.tar;









// shoot
	// goto skip_shoot;
	if(!Pgrounded) goto skip_shoot;
	if(G.frame % 2) goto skip_shoot;

	float pe_len2 = v4f_len2(G.player.pos - G.enm[0].pos);
bool pshoot = false;
	if (dgetk(DK_VOL_UP)) pshoot=true;
	if(pe_len2 < 20 || pshoot){
		bullet* b = bullets_add(G.player_bullets, &Pbul1);
		b->pos = G.player.pos;
		// b->pos[1] += .2;
		// b->rad = 0.00;
		b->vel = (v4f){0,0,-7,0} /*+ (v4f_rand() -.5)*0.5*/;
		// b->vel = v4f_mmul(b->vel, m4f_rotation_y(G.player.rot[1]));
		// b->vel = v4f_mtmul(b->vel, m4f_rotation((v4f){G.player.rot[0]-.26, G.player.rot[1], 0,0}));
		b->vel = v4f_mtmul(b->vel, m4f_rotation((v4f){0, G.player.rot[1], 0,0}));
	}

	if(pe_len2 < 10 /*&& Pgrounded>60*/ || pshoot){
		v4f vel = (v4f){0,0,-7,0} /*+ (v4f_rand() -.5)*0.5*/;
		// vel = v4f_mmul(vel, m4f_rotation_y(G.player.rot[1]));
		// vel = v4f_mtmul(vel, m4f_rotation((v4f){G.player.rot[0]-.18, G.player.rot[1], 0,0}));
		vel = v4f_mtmul(vel, m4f_rotation((v4f){0, G.player.rot[1], 0,0}));

		v4f tp = {.1,0,0,0};
		tp = v4f_mtmul(tp, m4f_rotation_y(G.player.rot[1]));
		// float s = sin(G.frame/4.f);
		bullet* b = bullets_add(G.player_bullets, &Pbul1);
		// b->vel = (v4f){10,0,10,0} * (v4f_rand() -.5);
		// b->rad = 0.00;
		// b->col0[0]=255;
		b->pos = G.player.pos + tp;
		// b->vel = (v4f){s*5,0,0,0};
		// b->vel = v4f_mmul(b->vel, m4f_rotation_y(G.player.rot[1]));
		// b->vel[1] = 5;
		b->vel = vel;

		b = bullets_add(G.player_bullets, &Pbul1);
		// b->col0[0]=255;
		// b->rad = 0.00;
		b->pos = G.player.pos - tp;

		b->vel = vel;
		// b->vel = (v4f){-s*5,0,0,0};
		// b->vel = v4f_mmul(b->vel, m4f_rotation_y(G.player.rot[1]));
		// b->vel[1] = 5;
	}

skip_shoot:;
//player trace
	bullet* pt;
	// pt = bullets_add(G.enemy_bullets, &playertrace),
	// playeraura.pos = G.player.pos;
	// playeraura.vel = v4f_0;
	// playeraura.acc = v4f_0;
	// playeraura.rad = G.player.rad;
	// pt = bullets_add(G.enemy_bullets, &playeraura);
	pt = bullets_add(G.enemy_bullets, &playeraura);
	pt->pos=G.player.pos;
	pt->rad = G.player.rad;
	// pt->col0 = v4c_rand()*(char)(sin(G.ftime/60.0)*255);
	// pt->col0 = (v4c){5,5,(char)((pow(sin(G.ftime),2)*255)), 255};
	pt->col0 = (v4c){5,5, 255, 255};
	// pt->pos = G.player.pos;
	pt->vel = v4f_0;
	pt->acc = v4f_0;
	pt->lifetime=25;



// plym->mesh->lp1 = G.player.pos;
plym->mesh->lp1 = G.sunpos;
plym->mesh->lp2 = G.sunpos;
plym->mesh->lp1[1] += 5;
plym->mesh->sca = (v4f){.5,.5,.5,1}*.5f;
plym->mesh->sca = (v4f){.5,.5,.5,1}*.5f;
// plym->mesh->sca[3] = 1;

plym->mesh->pos = G.player.pos;
plym->mesh->pos[1] -= 0.075;
plym->mesh->rot[1] = -G.player.rot[1];
// plym->mesh->rot[1] = G.player.rot;
mcyl_update(plym);

	G.sunpos[1]=powf(sin(G.ftime*.2), 2.0)*100.f+1;
	G.sunpos[0]=sin(G.ftime*.03)*100.0;
	G.sunpos[2]=cos(G.ftime*.03)*100.0;
	



}



































char* skyvs=DSHD_QUOTE(
	attribute vec4 apos;
	attribute vec4 anor;
	attribute vec2 atc;
	varying vec4 vp;
	varying vec4 sp; // screen position
	uniform mat4 VP;
	uniform mat4 M;
	uniform mat4 N;
	uniform vec4 eye;
	// uniform mat4 view;
	// uniform mat4 proj;
	void main(void){
		vp=M*apos;
		// sp=proj*view*vec4(pos.x, pos.y,0.99999, 0.0);
		// sp=proj/**view*/*vec4(pos.x, pos.y,-1.0, 1.0);
		// sp=vec4(pos.x, pos.y,0.0, 1.0);
		// gl_Position=sp;
		gl_Position=VP*M*apos;
		// gl_Position=vec2(pos.x, pos.y);
	}
);


char* skyfs=DSHD_QUOTE(
	varying vec4 vp;
	varying vec4 sp;
	uniform vec4 tar;
	uniform vec4 eye;
	uniform vec4 sunp;
	uniform vec4 sunc;
	uniform float fov;
	uniform float time;

// Based on "A Practical Analytic Model for Daylight" aka The Preetham Model, the de facto standard analytic skydome model
// http://www.cs.utah.edu/~shirley/papers/sunsky/sunsky.pdf
// Original implementation by Simon Wallner: http://www.simonwallner.at/projects/atmospheric-scattering
// Improved by Martin Upitis: http://blenderartists.org/forum/showthread.php?245954-preethams-sky-impementation-HDR
// Three.js integration by zz85: http://twitter.com/blurspline / https://github.com/zz85 / http://threejs.org/examples/webgl_shaders_sky.html
// Additional uniforms, refactoring and integrated with editable sky example: https://twitter.com/Sam_Twidale / https://github.com/Tw1ddle/Sky-Particles-Shader


// uniform vec3 cameraPos;
// uniform float depolarizationFactor;
// uniform float luminance;
// uniform float mieCoefficient;
// uniform float mieDirectionalG;
// uniform vec3 mieKCoefficient;
// uniform float mieV;
// uniform float mieZenithLength;
// uniform float numMolecules;
// uniform vec3 primaries;
// uniform float rayleigh;
// uniform float rayleighZenithLength;
// uniform float refractiveIndex;
// uniform float sunAngularDiameterDegrees;
// uniform float sunIntensityFactor;
// uniform float sunIntensityFalloffSteepness;
// uniform vec3 sunPosition;
// uniform float tonemapWeighting;
// uniform float turbidity;

float depolarizationFactor = 0.068;
float luminance = 1.0;
float mieCoefficient = 0.009;
float mieDirectionalG = 0.82;
vec3 mieKCoefficient = vec3(0.686,0.688,0.666);
float mieV = 8.35;
float mieZenithLength = 34000.0;
float numMolecules = 100000.0;
vec3 primaries = vec3(0.75,0.5,0.5)/10.0;
float rayleigh = 0.028;
float rayleighZenithLength = 8400.0;
float refractiveIndex = 1.0001;
float sunAngularDiameterDegrees = 0.004;
float sunIntensityFactor = 250.0;
float sunIntensityFalloffSteepness = 1.5;
// vec3 sunPosition = vec3(100.0,100.1,0.0);
float tonemapWeighting = 8.5;
float turbidity = 8.7;


// const float PI = 3.141592653589793238462643383279502884197169;
const vec3 UP = vec3(0.0, 1.0, 0.0);

vec3 totalRayleigh(vec3 lambda)
{
	return (8.0 * pow(PI, 3.0) * pow(pow(refractiveIndex, 2.0) - 1.0, 2.0) * (6.0 + 3.0 * depolarizationFactor)) / (3.0 * numMolecules * pow(lambda, vec3(4.0)) * (6.0 - 7.0 * depolarizationFactor));
}

vec3 totalMie(vec3 lambda, vec3 K, float T)
{
	float c = 0.2 * T * 10e-18;
	return 0.434 * c * PI * pow((2.0 * PI) / lambda, vec3(mieV - 2.0)) * K;
}

float rayleighPhase(float cosTheta)
{
	return (3.0 / (16.0 * PI)) * (1.0 + pow(cosTheta, 2.0));
}

float henyeyGreensteinPhase(float cosTheta, float g)
{
	return (1.0 / (4.0 * PI)) * ((1.0 - pow(g, 2.0)) / pow(1.0 - 2.0 * g * cosTheta + pow(g, 2.0), 1.5));
}

float sunIntensity(float zenithAngleCos)
{
	float cutoffAngle = PI / 1.95; // Earth shadow hack
	return sunIntensityFactor * max(0.0, 1.0 - exp(-((cutoffAngle - acos(zenithAngleCos)) / sunIntensityFalloffSteepness)));
}

// Whitescale tonemapping calculation, see http://filmicgames.com/archives/75
// Also see http://blenderartists.org/forum/showthread.php?321110-Shaders-and-Skybox-madness
const float A = 0.15; // Shoulder strength
const float B = 0.50; // Linear strength
const float C = 0.10; // Linear angle
const float D = 0.20; // Toe strength
const float E = 0.02; // Toe numerator
const float F = 0.30; // Toe denominator
vec3 Uncharted2Tonemap(vec3 W)
{
	return ((W * (A * W + C * B) + D * E) / (W * (A * W + B) + D * F)) - E / F;
}

void main()
{
vec3 sunPosition = sunp.xyz;
vec3 vWorldPosition=vp.xyz;
vWorldPosition.y+=20.0; // HORIZON HEIGHT

 vec3 cameraPos=eye.xyz;

	// Rayleigh coefficient
	float sunfade = 1.0 - clamp(1.0 - exp((sunPosition.y / 100.0)), 0.0, 1.0);
	float rayleighCoefficient = rayleigh - (1.0 * (1.0 - sunfade));
	vec3 betaR = totalRayleigh(primaries) * rayleighCoefficient;
	
	// Mie coefficient
	vec3 betaM = totalMie(primaries, mieKCoefficient, turbidity) * mieCoefficient;
	
	// Optical length, cutoff angle at 90 to avoid singularity
	float zenithAngle = acos(max(0.0, dot(UP, normalize(vWorldPosition - cameraPos))));
	float denom = cos(zenithAngle) + 0.15 * pow(93.885 - ((zenithAngle * 180.0) / PI), -1.253);
	float sR = rayleighZenithLength / denom;
	float sM = mieZenithLength / denom;
	
	// Combined extinction factor
	vec3 Fex = exp(-(betaR * sR + betaM * sM));
	
	// In-scattering
	vec3 sunDirection = normalize(sunPosition);
	float cosTheta = dot(normalize(vWorldPosition - cameraPos), sunDirection);
	vec3 betaRTheta = betaR * rayleighPhase(cosTheta * 0.5 + 0.5);
	vec3 betaMTheta = betaM * henyeyGreensteinPhase(cosTheta, mieDirectionalG);
	float sunE = sunIntensity(dot(sunDirection, UP));
	vec3 Lin = pow(sunE * ((betaRTheta + betaMTheta) / (betaR + betaM)) * (1.0 - Fex), vec3(1.5));
	Lin *= mix(vec3(1.0), pow(sunE * ((betaRTheta + betaMTheta) / (betaR + betaM)) * Fex, vec3(0.5)), clamp(pow(1.0 - dot(UP, sunDirection), 5.0), 0.0, 1.0));
	
	// Composition + solar disc
	float sunAngularDiameterCos = cos(sunAngularDiameterDegrees);
	float sundisk = smoothstep(sunAngularDiameterCos, sunAngularDiameterCos + 0.00002, cosTheta);
	vec3 L0 = vec3(0.1) * Fex;
	L0 += sunE * 19000.0 * Fex * sundisk;
	vec3 texColor = Lin + L0;
	texColor *= 0.04;
	texColor += vec3(0.0, 0.001, 0.0025) * 0.3;
	
	// Tonemapping
	vec3 whiteScale = 1.0 / Uncharted2Tonemap(vec3(tonemapWeighting));
	vec3 curr = Uncharted2Tonemap((log2(2.0 / pow(luminance, 4.0))) * texColor);
	vec3 color = curr * whiteScale;
	vec3 retColor = pow(color, vec3(1.0 / (1.2 + (1.2 * sunfade))));

	gl_FragColor = vec4(retColor, 1.0) * sunc;
	// gl_FragDepth = gl_FragColor.b;
}


);

#if 0

char* skyfs=DSHD_QUOTE(
	varying vec2 vp;
	varying vec4 sp;
	uniform vec4 tar;
	uniform vec4 eye;
	uniform float fov;
	uniform float time;

	// uniform mat4 view;
	// uniform mat4 proj;
	// uniform struct {
	// 	vec4 pos;
	// 	vec4 col;
	// } l[4];
	// uniform mat4 view;
	// uniform mat4 proj;

	void main(void){
		vec2 uv = vp;
		vec3 ro = eye.xyz;
		vec3 ta = tar.xyz;
		// vec3 ww=normalize( ta-ro );
		vec3 ww=normalize( ta-ro );
		vec3 uu=normalize( cross(ww, vec3(0.0,1.0,0.0)));
		vec3 vv=normalize( cross(uu,ww));
		vec3 rd=normalize(uv.x*uu + uv.y*vv + fov*ww);

vec3 skycol=vec3(.6,.7,.8);//blue
// vec3 skycol=vec3(.5,.2,.7);//purple
		gl_FragColor=vec4( (skycol - .5*pow(abs(rd.y), .5)).rgb , 1.0 );
		
		// float dist = (length(vec3(0.0,-1000.0,0.0)-ro)-50.0);
		float dist = 1000.0;
		vec3 sdpos = ro+rd*dist;
		float r = 1.0; //sqrt(x*x+y*y+z*z);
		float theta = atan(sdpos.x, sdpos.y);
		float phi = atan(length(sdpos.xy),sdpos.z);
		// float r = 1.0; //sqrt(x*x+y*y+z*z);
		// float theta = atan(rd.x, rd.y);
		// float phi = atan(length(rd.xy),rd.z);
// r = sqrt(x*x+y*y+z*z);
// theta = atan2(y,x);
// phi = atan2(sqrt(x*x+y*y),z);

vec3 col=mix( skycol, vec3(0.0), vec3( sin(abs(theta)*24.0+time) + sin(phi*42.0)));
col=mix(skycol*.1, col, ( vec3(clamp(sdpos.y, 0.0, 1.0))) );
// vec3 col=mix( skycol, vec3(0.0), vec3( (normalize(sdpos)) ));
// vec3 col=mix( skycol, vec3(0.0), vec3(abs(sin(theta))) );
		gl_FragColor=vec4( r, abs(theta), abs(phi) , 1.0 );
		gl_FragColor=vec4(col, 1.0);
		
		// rd= normalize(vec3(uv.xy, .5));
		// gl_FragColor=vec4( uv.xxx , 1.0 );
		#if 0
		// gl_FragColor=vec4(1.0,1.0,1.0,1.0)-vec4(vp.y, vp.y, vp.y/2.0, 0.0);
		gl_FragColor=mix( vec4(.8,.66,.21,1.0), vec4(.5,.5,.9,1.0), (vec4(vp.y, vp.y, vp.y,1.0)/2.0 + .5)  ); // day dessert
		// gl_FragColor=mix( vec4(.95,.95,.1,1.0)*.15, vec4(.5,.5,.9,1.0)*.25, vec4(vp.y, vp.y, vp.y,1.0)/2.0 + .5  ); // night
		// gl_FragColor=vec4(.0,.0,.0,.0)*vp.yyyy; // night
		gl_FragColor*=1.0-length(vp.xy)*2.0;
		gl_FragColor=vec4(0.1);
		#endif
		gl_FragColor.a=1.0;
		// vec4 d=sp;
		// // vec4 d=vec4(1.0,1.0,1.0,0.0);
		// vec4 eye=vec4(vp.xy,-1.0,1.0);
		// vec4 c = eye*length(vp.xy)*.1;
		// gl_FragColor *= c;

#if 0
		vec4 c = vec4(0.0,0.0,0.0,0.0);
		for (int i = 0; i < 4; ++i){
			c += l[i].col;// + (.02*length(l[i].pos.xy));//(proj*view*vec4((vp-l[i].pos.xy), 0.0,1.0) );
		}
		gl_FragColor *= c;
		gl_FragColor.a=1.0;
		// gl_FragColor = vec4(1.0,0.0,0.0,1.0);
#endif
		
	}

);
#endif






EMSCRIPTEN_KEEPALIVE void sunc( float x, float y, float z ){
	G.suncol[0]=x;
	G.suncol[1]=y;
	G.suncol[2]=z;
}

EMSCRIPTEN_KEEPALIVE float eval( float x, float y ){
	return hmap_eval(G.hmap0, x, y);
}














static int s_done = 0;
static int s_is_connected = 0;




static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
  (void) nc;
struct mbuf *io;// = &nc->recv_mbuf; // emcc only

  switch (ev) {
  	
    case MG_EV_CONNECT: {
      int status = *((int *) ev_data);
      if (status != 0) {
        printf("-- Connection error: %d\n", status);
      }
      break;
    }

    case MG_EV_WEBSOCKET_HANDSHAKE_DONE: {
      struct http_message *hm = (struct http_message *) ev_data;
      if (hm->resp_code == 101) {
        printf("-- Connected\n");
        s_is_connected = 1;
      } else {
        printf("-- Connection failed! HTTP code %d\n", hm->resp_code);
        /* Connection will be closed after this. */
      }
      break;
    }

    case MG_EV_POLL: {
      // char msg[500];
      // mg_send_websocket_frame(nc, WEBSOCKET_OP_TEXT, msg, n);

   // int32_t netpos[3] = {G.player.pos[0]*1000, G.player.pos[1]*1000, G.player.pos[2]*1000};
      // mg_send_websocket_frame(nc, WEBSOCKET_OP_BINARY, netpos, sizeof(int32_t)*3);
    
    int32_t bit = G.player.pos[1]*1000.f;
   	
   	#ifdef __EMSCRIPTEN__
      mg_send(nc, &bit, sizeof(int32_t));
    #else
      mg_send_websocket_frame(nc, WEBSOCKET_OP_BINARY, &bit, sizeof(int32_t));
    #endif
    
    	break;
    }


#ifdef __EMSCRIPTEN__
	case MG_EV_RECV:
	io = &nc->recv_mbuf;

		// printf("%s\n", io->buf);
	    printf("%.*s\n", (int) io->len, io->buf);
		mbuf_remove(io, io->len);
	break;

#else
    case MG_EV_WEBSOCKET_FRAME: { // 
      struct websocket_message *wm = (struct websocket_message *) ev_data;
      printf("%.*s\n", (int) wm->size, wm->data);
      break;
    }
#endif

    case MG_EV_CLOSE: {
      if (s_is_connected) printf("-- Disconnected\n");
      s_done = 1;
      break;
    }
  }
}

void wschat(void ){
  const char *chat_server_url = 
		#ifdef __EMSCRIPTEN__ 
		"tcp://" 
		#else
		"ws://"
		#endif
		// "192.168.0.220:1717";
  		"xb17.duckdns.org:1717";
  printf("%s\n", chat_server_url);
  // const char *chat_server_url = "ws://127.0.0.1:7777";
  // const char *chat_server_url = "ws://127.0.0.1:7777";
  // const char *chat_server_url = "ws://192.168.0.220:8000";
  // const char *chat_server_url = "ws://xb17.duckdns.org:7777";
  // const char *chat_server_url = "ws://192.168.0.220:1717";
  // const char *chat_server_url = "ws://xb17.duckdns.org:1717";
  // const char *chat_server_url = "tcp://xb17.duckdns.org:1717";
  // const char *chat_server_url = "tcp:127.0.0.1:1717";

// const char *chat_server_url = "ws://181.167.117.180:1717";
  // const char *chat_server_url = "ws://192.168.0.25:8000";

  mg_mgr_init(&mgr, NULL);

  // nc = mg_connect_ws(&mgr, ev_handler, chat_server_url_local, "ws_chat", NULL);
  // if (nc == NULL) {
    // fprintf(stderr, "Invalid address, trying non local server\n");
    // nc = mg_connect_ws(&mgr, ev_handler, chat_server_url, "ws_chat", NULL);
  #ifdef __EMSCRIPTEN__
    nc = mg_connect(&mgr, chat_server_url, ev_handler);
#else
    nc = mg_connect_ws(&mgr, ev_handler, chat_server_url, NULL, NULL);
#endif
  // }
  if (nc == NULL) {
    fprintf(stderr, "Invalid address\n");
    return ;
  }

  return ;

}









