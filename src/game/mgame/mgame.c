// #include "../de/de.h"

#include "mgame.h"

struct game game;

// float* sndchunk;
// #include "bullets.h"
void mgame_init(void);
void mgame_quit(void);

// m4f view, mvp;

void mgame_quit(void){
	// free(game.enemy_bullets);
	// free(sndchunk);
	// free(game.player_bullets);
}

bullets* extra[5];

#define hmap_res 64
// #define hmap_res 256
// #define hmap_res 128
// #define hmap_res 80
// #define hmap_res 64
// #define hmap_res 512


float hfunc(float,float);





void mgame_init(void){
	bullets_init( num_enemy_bullets );
	game.enemy_bullets = bullets_new(num_enemy_bullets);
	game.player_bullets = bullets_new(num_player_bullets);

	//init map
	game.player.pos = v4f_0;


	// game.hmap = hmap_new(hmap_res,hmap_res, hfunc, 0);
	game.hmap = hmap_new(hmap_res,hmap_res, hfunc, HMAP_DYNAMIC | HMAP_FIXED);
	// game.hmap = hmap_new(hmap_res,hmap_res, hfunc, HMAP_DYNAMIC);


// 	dshd_unif(mesh_shd, "M", );


// dsnd* testsnd = dsnd_open(DE_ASSET("sfx1.ogg"));
// dsnd_play(testsnd);

// int smplen = 44100;
int smplen = De.sample_rate;
// float sndchunk[ smplen * 2 ];
// float* sndchunk = alloca(smplen * 2 * sizeof(float));
float* sndchunk = malloc(smplen * 2 * sizeof(float));

for (int i = 0; i < smplen*2; ++i){
	sndchunk[i] = 0.0;
}

for (int i = 0; i < smplen*2; ++i){
	float c = (float)i;
	c /= De.sample_rate;
	c = PI*c;
	float r = sin(440*c);
	float m = sin(2*c);
	r = r*m;
	sndchunk[i] = r;
	i++;
	sndchunk[i] = r;
}

dsnd* testpsnd = dsnd_new(sndchunk, smplen * 2 * sizeof(float));
// dsnd_playf(testpsnd, 0, 100);
// dsnd_playf(testpsnd, -1, 300);
// int chan = ;
dsnd_fade(dsnd_playl(testpsnd, -1), 10000);
// dsnd_play(testpsnd);
// dsnd_free(testpsnd);
// free(sndchunk);
}



void mgame_main(void){
	mgame_init();
// dvsync(-1);
// test bullets
	bullet* ajoe = bullets_add(game.enemy_bullets, &joe);
	ajoe->pos[2] = 50;
	ajoe->vel[1] = 5;

	ajoe = bullets_add(game.player_bullets, &joe);
	ajoe->pos[2] = 50;
	ajoe->pos[0] = 10;
	ajoe->vel[1] = 5;

	int frame = 0;
	float fric = 1.33;
	float grav = -90 * 0.016f;
	float pspeed = 2.5f;
	// float tvel = 8.f;
	float tvely = 100.f;
	float rotspeed = (PI/90);
	bool mouse_grab=1;
	game.player.vel = v4f_0;
	game.player.rad = .25;
	game.player.height = .175;
	// game.player.vel = v4f_0;
	// dctrl_setup(&game.player.pos,
	// 	&game.player.rot, &game.player.vel,
	// 	pspeed, tvel );
	// dctrl_fps();


v4f tor_dir = v4f_0;

// v2f lthumb_center = v2f_0;
// v2f rthumb_center = v2f_0;
struct dfinger* lthumb = NULL;
struct dfinger* rthumb = NULL;

tor_dir[0] = 1;
tor_dir[1] = -10;
bool dmktsel = 0;
obj enemy;
enemy.pos = v4f_0;
enemy.vel = v4f_0;

	// DE_GAME_LOOP(){ if(dusek('q') || dusek(DK_BACK)) break;
	DE_GAME_LOOP(){ if(dusek(DK_ESC) || dusek(DK_BACK)) break;
		frame++;
// dclear_color((v4f){.02, .08, .202});
		float h = game.hmap->f(game.player.pos[0], game.player.pos[2]);

//test danmaku

if (frame%160 == 0) {
	dmktsel = (dmktsel+1) % 2;
	enemy.vel[1] = 0;
	enemy.vel[0] = dmktsel? (((1.f*rand())/INT_MAX)-.5f)*2 : 0;
	enemy.vel[2] = dmktsel? (((1.f*rand())/INT_MAX)-.5f)*2 : 0;

	if(dmktsel) { // sphere dmk
		int il = 16; // density
		int jl = il;
		m4f rot = m4f_rotation_y(rand()/PI);
		for (int i = 0; i < il; ++i){
			for (int j = 0; j < jl; ++j){
				float ir = PI * i/(il-1);
				float jr = PI * j/(jl-1);

				bullet* b = bullets_add(game.enemy_bullets, &joe_spark);
				b->pos=enemy.pos;
				b->pos[1]-=1.75;
				//trig magic
				v4f vel = v4f_0;
				vel[0]=cos(jr) * sin(ir);
				vel[1]=sin(jr) * sin(ir);
				vel[2]=cos(ir);
				b->vel = v4f_mmul(vel, rot);
				b->vel *= 10;
				b->tvel = 90;
				b->rad = .5;
				b->acc = v4f_0;
				b->col1 = (v4c){0,255,155,155};
				b->col0 = (v4c){0,255,0,155};
				b->frame = -999090;
			}
		}
	}

}
//main dmk
if(!(frame%2)){
	for (int i = 0; i < 4; ++i){
		/* code */
		bullet* tor = bullets_add(game.enemy_bullets, &glider);
		tor->pos = enemy.pos;
		tor->vel = tor_dir*8;
		tor->acc[1] = -90;
		tor->rad = 0.4;
		tor_dir = v4f_mmul(tor_dir, m4f_rotation_y(PI/6.0));
	// if(!(frame%(5*8)))
		// tor_dir = v4f_mmul(tor_dir, m4f_rotation_z(PI/3.0));
	}
	if(dmktsel) tor_dir = v4f_mmul(tor_dir, m4f_rotation_y(-PI/(6.0/4)+0.12));
	else tor_dir = v4f_mmul(tor_dir, m4f_rotation_y(-PI/(6.0/3)+0.11));
	// tor_dir = v4f_mmul(tor_dir, m4f_rotation_y(sin(frame/80.0)));
}
enemy.pos += enemy.vel * 0.016f;
enemy.pos[1] = hfunc(enemy.pos[0], enemy.pos[2]) + .5;
/* player fancy sprite */
// if(!(frame%4)){
	bullet* pt;
	pt = bullets_add(game.enemy_bullets, &playertrace),
	pt->rad = game.player.rad;
	pt->pos = game.player.pos;
	pt->vel = v4f_0;

// }



//touch test
	for (int i = 0; i < DE_FINGER_MAX; ++i){
		if(De.finger[i].down){
			bullet* b = bullets_add(game.enemy_bullets, &playertrace);
			b->pos[0] = De.finger[i].abs[0]*10;
			b->pos[1] = 0;
			b->pos[2] = De.finger[i].abs[1]*10;
		}
	}



// player_update();
	// if(dusek(DK_ESC)) { mouse_grab = (mouse_grab+1) %2;}
	if(dusek('q')) { mouse_grab = (mouse_grab+1) %2;}
	if(mouse_grab) dmouse_grab(1);
	else dmouse_grab(0);

	bool jump_input = 0;

	if(De.mouse.b1){
		// jump_input=1;
		// De.mouse.b1 =0;
		// bullet* ajoe = bullets_add(game.enemy_bullets, &joe);
		// ajoe = bullets_add(game.player_bullets, &joe);
		// ajoe = bullets_add(extra[0], &joe);
		// ajoe = bullets_add(extra[1], &joe);
	}

//player input
		v4f pdir = v4f_0;
		if     (dgetk('w') > dgetk('s')) /*game.player.vel*/pdir[2] =  1;
		else if(dgetk('s') > dgetk('w')) /*game.player.vel*/pdir[2] = -1;
		if     (dgetk('d') > dgetk('a')) /*game.player.vel*/pdir[0] =  1;
		else if(dgetk('a') > dgetk('d')) /*game.player.vel*/pdir[0] = -1;
		if     (dgetk(' ') > dgetk('x')) /*game.player.vel*/pdir[1] =  1;//
		else if(dgetk('x') > dgetk(' ')) /*game.player.vel*/pdir[1] = -1;//

		if(dgetk(DK_LEFT) > dgetk(DK_RIGHT)) game.player.rot[1] +=  rotspeed;
		else if(dgetk(DK_RIGHT) > dgetk(DK_LEFT)) game.player.rot[1] += -rotspeed;
		if(dgetk(DK_DOWN) > dgetk(DK_UP))    game.player.rot[0] +=  rotspeed;
		else if(dgetk(DK_UP) > dgetk(DK_DOWN))    game.player.rot[0] += -rotspeed;

//touch controls
		if(lthumb == NULL){
			lthumb = dfinger_in_area(0,0, .5,1); //left side
			if(lthumb) lthumb->captured = true;
		}
		if(rthumb == NULL){
			rthumb = dfinger_in_area(.5,0, 1,1); //right side
			if(rthumb) rthumb->captured = true;
		}

		if(lthumb){ // has a finger bound
			if(lthumb->down){
				pdir[0] = lthumb->rel[0];
				pdir[2] = -lthumb->rel[1];

				pdir[0] *= 10;
				pdir[2] *= 10;

				// pdir[0] *= pdir[0]*pdir[0];
				// pdir[2] *= pdir[2]*pdir[2];
			} else lthumb=NULL;
		}
		if(rthumb){
			if(rthumb->down){
				game.player.rot[1] +=-rthumb->rel[0] / 4;
				game.player.rot[0] += rthumb->rel[1] / 4;
			} else rthumb = NULL;
		}
		if(rthumb == lthumb){
			rthumb = NULL;
			lthumb = NULL;
		}



		// game.player.rot[0] = clamp(game.player.rot[0], -PI/2, PI/2);
		game.player.rot[0] = clamp(game.player.rot[0], -PI/4, PI/2);

		// game.player.rot = v4f_mmul((v4f){0,0,1,1}, m4f_look_at(game.player.pos, enemy.pos, (v4f){0,1,0,0}) );




// focus on enemy fixme
	// lerped does not autocorrect edge case
	// straight does extreme fixing and snaps when pushing towards enemy
		// game.player.rot[1] = lerp(game.player.rot[1] , -atan2(
		// 	enemy.pos[0] - game.player.pos[0] ,
		// 	enemy.pos[2] - game.player.pos[2]
		// ), .07);
		// game.player.rot[1] = -atan2(enemy.pos[0] - game.player.pos[0], enemy.pos[2] - game.player.pos[2]);

		if(mouse_grab){
			game.player.rot[0] += De.mouse.rel[1] / 5.f * 0.016f;
			game.player.rot[1] -= De.mouse.rel[0] / 5.f * 0.016f;
		}

		v4f npdir = v3f_normalize(pdir); // circular clipping
		float minx = min(-npdir[0], npdir[0]);
		float maxx = max(-npdir[0], npdir[0]);
		float minz = min(-npdir[2], npdir[2]);
		float maxz = max(-npdir[2], npdir[2]);
		pdir[0] = clamp(pdir[0], minx, maxx);
		pdir[2] = clamp(pdir[2], minz, maxz);

		pdir = v4f_mmul(pdir, m4f_rotation_y(game.player.rot[1]) );


		//slow
		pdir *= dgetk(DK_LSHIFT)? pspeed/2 : pspeed;

		//jump
		float pjumpspeed = 0;
		if (dusek(' ')) jump_input=true;
		if (De.finger_click == true) {
			De.finger_click = false;
			jump_input=true;
		}

		if(jump_input) pjumpspeed = 12*2;

		pdir[1] = pjumpspeed;
		// game.player.vel += pdir;
		// game.player.acc[1] += grav;
		game.player.vel[1] += grav;

		if(pjumpspeed) //stop falling
			game.player.vel[1] = max(0, game.player.vel[1]);

//only friction keeps velocity "clamped"
		game.player.vel += pdir;
		game.player.vel += game.player.acc * 0.016f;

		// game.player.vel[0] = clamp(game.player.vel[0], -tvel, tvel);
		game.player.vel[1] = clamp(game.player.vel[1], -tvely, tvely);
		// game.player.vel[2] = clamp(game.player.vel[2], -tvel, tvel);

		game.player.pos += game.player.vel * 0.016f;

		// float h = game.hmap->f(game.player.pos[0], game.player.pos[2]);
		if(game.player.pos[1] <= h + game.player.rad+game.player.height){
			game.player.pos[1] = h + game.player.rad+game.player.height;
			game.player.acc[1] = 0;
			game.player.vel[1] = 0;
		}
		game.player.vel[0] /= fric;
		game.player.vel[2] /= fric;

		// game.player.vel = v4f_0 + (v4f){0,game.player.vel[1],0,0};
//dctrl_fps

// player graphics
	// abbymesh->pos = game.player.pos;
	// abbymesh->pos = v4f_0;
	// abbymesh->pos[1] -= 1.0;
	// abbymesh->rot[1] = game.player.rot[1] + PI;
	// abbymesh->sca = (v4f){ 1.0, 1.0, 1.0, 1 } / 0.4f;
		// game.player.pos[2] += noise1(frame*.0123);
		// game.player.pos[1] = 100;
		// game.player.rot[0] = PI/12;

// CAMERA

//fps view
		// De.cam.pos = game.player.pos + .5;
		// De.cam.rot = game.player.rot;
		De.cam.fov = PI/3.0;
//tps view
		// v4f camtrans = {0,.5,-4,0};
		// v4f camtrans = {0,.5,-6,0};
		v4f camtrans = {0,5,-16,0};

		De.cam.pos = game.player.pos + v4f_mtmul(camtrans, m4f_rotation(-game.player.rot));
		// De.cam.pos = game.player.pos + v4f_mtmul((v4f){0,0,-12,0}, m4f_rotation(-game.player.rot));
		De.cam.pos[1] = max(De.cam.pos[1], hfunc(De.cam.pos[0],De.cam.pos[2])+1); // cam collides with hmap
		De.cam.rot = game.player.rot;

dcam_update();
		// De.cam.rot = v4f_mmul(v4f_normalize(game.player.pos -  De.cam.pos), m4f_rotation(game.player.rot));
		// dcam_update();
		// De.cam.view = m4f_view(game.player.pos, game.player.rot);
		// game.mvp = m4f_mul(De.cam.view, m4f_proj(PI/6.f, De.res, .5f, 5000.f) );
		game.mvp = De.cam.vp;
		// dview_getpos
		// dview_getrot
		// dview_getfov
		// dview_getfclip
		// dview_getnclip
		// De.cam.pos;
		// De.cam.V;
		// De.cam.VP;
		// De.cam.P;
		// De.view_near_clip

		// De.cam.mvp = m4f_mul(De.cam.view, De.cam.proj );






		v4f transfocus = game.player.pos + v4f_mtmul((v4f){0,0,16,0}, m4f_rotation(-game.player.rot));
		transfocus[1] = game.player.pos[1];
		hmap_update(game.hmap, transfocus);
		// hmap_update(game.hmap, game.player.pos);
		bullets_update(game.enemy_bullets, .016f);

dcullf('f');
ddepth(1,0);
dblend(0);
// dmesh_draw(abbymesh);
		bullets_draw(game.enemy_bullets);
		hmap_draw(game.hmap);

// dclear(0);


// mesh draw
// dshd_bind(abbyshd);
// dtex_bind(abbytex, 0);
//abby draw
// dshd_bind()
// dvao_draw(abbyvao, 0,0,0);

// dtex_draw(abbytex);
// dtex_draw(game.enemy_bullets->shadow_map->color);

// dfbo_draw(game.enemy_bullets->shadow_map);
// dtex_draw(game.enemy_bullets->shadow_map->color);
		game.frame++;

	}

	mgame_quit();
}





































// function for the hmap
float hfunc(float x,float y){
	// return -2;
	// #if 0
	// float rad = pow(noise1(game.frame/80.0)+1, 2.0) * 3;
	v2d p = {x,y};
	// if(noise2(p/5)<-0.5)
		// p[1] = y+game.frame/100.f;

	// float d = noise2(p/9);
	// return d *5;
	// return lerp( (float) ((((int)x>>2)%6) ^ (((int)y>>2)%6)) *d  ,   d*6.f  ,   d+.5) *d;

	// int res = ((((int)x>>2)%6) ^ (((int)y>>2)%6));
	// return res - 5 + noise1(x*game.frame/100.f);

	// int mx, my;
	// x=fmod(x,4.4);
	// y=fmod(y,4.4);
	// mx = *(int*)&x;
	// my = *(int*)&y;

	// return ( (mx)^(my) )/(INT_MAX/8.f);
	// return noise2(p) * ;
	// return noise2(p*0.1) * 15.f * noise2(p*.01) * 6;
	// float rad = 4;
	// if( (x<rad)&&(x>-rad)/* && (y<rad)&&(y>-rad)*/ )return -2;
	// return hash2((int)(x*3.33),(int)(y*2.32))/2.f/INT_MAX * 2.5f    +    (hash2((int)(x/2.f),(int)(y/2.f))/2.f/INT_MAX)
	// + noise1((x*y*100+game.frame)/500.f);
	// return noise2((v2d){x/9.f,y/9.f}) * 2.5f    +    noise2((v2d){x,y}+game.frame/80.0) * .5f
	// + noise2(p/64.f)*10.f ;
	// float n1 = noise2p(p/10.f, (v2i){2,2})*2 + noise2(p/1250)*8;
	// v2i per = (v2i){64,64};
		float yf = game.frame/50.f;
	v4d p3 = {p[0], p[1], yf};
	float n1 = noise3(p3/10.f)*3;
	n1*=n1;
	return n1;
	// float h = n1 + noise2(p/5)/(n1+1);
	float h = n1;
	// h;

	if(h<.5) {
		p[1] = y+game.frame/10.f;
	if(h<.05)
		h -= fabs(noise2(p/25));
	else h -= fabs(noise2(p/125));
	}
	return h;

	// #endif
}

































