// #include "../de/de.h"

#include "mgame.h"

struct game game;

float* sndchunk;

void mgame_init(void);
void mgame_quit(void);

void mgame_quit(void){
	bullets_free(game.enemy_bullets);
	bullets_free(game.player_bullets);
	free(sndchunk);
}


#define hmap_res 64

float hfunc(float,float);





void mgame_init(void){
	bullets_init( num_enemy_bullets );
	game.enemy_bullets = bullets_new(num_enemy_bullets);
	game.player_bullets = bullets_new(num_player_bullets);

	game.player.pos = v4f_0;


	// game.hmap = hmap_new(hmap_res,hmap_res, hfunc, 0);
	game.hmap = hmap_new(hmap_res,hmap_res, hfunc, HMAP_DYNAMIC | HMAP_FIXED);
	// game.hmap = hmap_new(hmap_res,hmap_res, hfunc, HMAP_DYNAMIC);



// dsnd* testsnd = dsnd_open(DE_ASSET("sfx1.ogg"));
// dsnd_play(testsnd);

// int smplen = 44100;
int smplen = De.sample_rate;

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

dsnd_fade(dsnd_playl(testpsnd, -1), 6000);
// dsnd_free(testpsnd);
}



void mgame_main(void){
	mgame_init();

	int frame = 0;

// player
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

	float pslowspeed = pspeed/2;

	struct dfinger* lthumb = NULL;
	struct dfinger* rthumb = NULL;


//enemy test
	obj enemy;
	v4f spinning_dmk_dir = v4f_0;
	spinning_dmk_dir[0] = 1;
	spinning_dmk_dir[1] = -10;
	bool dmktsel = 0;
	enemy.pos = v4f_0;
	enemy.vel = v4f_0;


	DE_GAME_LOOP(){ if(dusek(DK_ESC) || dusek(DK_BACK)) break;
		frame++;
		// dclear_color((v4f){.02, .08, .202});
		float h = game.hmap->f(game.player.pos[0], game.player.pos[2]);





//sphere dmk
if (frame%160 == 0) {
	dmktsel = (dmktsel+1) % 2;
	enemy.vel[1] = 0;
	enemy.vel[0] = dmktsel? (((1.f*rand())/INT_MAX)-.5f)*2 : 0;
	enemy.vel[2] = dmktsel? (((1.f*rand())/INT_MAX)-.5f)*2 : 0;

	if(dmktsel) {
		int il = 16; // density
		int jl = il;
		m4f rot = m4f_rotation_y(rand()/PI);
		for (int i = 0; i < il; ++i){
			for (int j = 0; j < jl; ++j){
				float ir = PI * i/(il-1);
				float jr = PI * j/(jl-1);
				bullet* b = bullets_add(game.enemy_bullets, &joe_spark);
				b->inited=1; // avoid using default values
				b->pos=enemy.pos;
				b->pos[1]-=1.75;
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

// spinning dmk
if(!(frame%2)){
	for (int i = 0; i < 4; ++i){
		bullet* tor = bullets_add(game.enemy_bullets, &glider);
		tor->pos = enemy.pos;
		tor->vel = spinning_dmk_dir*8;
		tor->acc[1] = -90;
		tor->rad = 0.4;
		spinning_dmk_dir = v4f_mmul(spinning_dmk_dir, m4f_rotation_y(PI/6.0));
	// if(!(frame%(5*8)))
		// spinning_dmk_dir = v4f_mmul(spinning_dmk_dir, m4f_rotation_z(PI/3.0));
	}
	if(dmktsel) spinning_dmk_dir = v4f_mmul(spinning_dmk_dir, m4f_rotation_y(-PI/(6.0/4)+0.12));
	else spinning_dmk_dir = v4f_mmul(spinning_dmk_dir, m4f_rotation_y(-PI/(6.0/3)+0.11));
	// spinning_dmk_dir = v4f_mmul(spinning_dmk_dir, m4f_rotation_y(sin(frame/80.0)));
}

enemy.pos += enemy.vel * 0.016f;
enemy.pos[1] = hfunc(enemy.pos[0], enemy.pos[2]) + .5;


/* player fancy sprite */
	bullet* pt;
	pt = bullets_add(game.enemy_bullets, &playertrace),
	pt->rad = game.player.rad;
	pt->pos = game.player.pos;
	pt->vel = v4f_0;



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
	bool slow_input = 0;

	if(De.mouse.b2){
		jump_input = 1;
		De.mouse.b2 = 0;
	}
	if(De.mouse.b1){
		slow_input = 1;
	}

//player input
		v4f pdir = v4f_0;
		if     (dgetk('w') > dgetk('s')) pdir[2] =  1;
		else if(dgetk('s') > dgetk('w')) pdir[2] = -1;
		if     (dgetk('d') > dgetk('a')) pdir[0] =  1;
		else if(dgetk('a') > dgetk('d')) pdir[0] = -1;
		if     (dgetk(' ') > dgetk('x')) pdir[1] =  1;
		else if(dgetk('x') > dgetk(' ')) pdir[1] = -1;

		if(dgetk(DK_LEFT) > dgetk(DK_RIGHT)) game.player.rot[1] +=  rotspeed;
		else if(dgetk(DK_RIGHT) > dgetk(DK_LEFT)) game.player.rot[1] += -rotspeed;
		if(dgetk(DK_DOWN) > dgetk(DK_UP))    game.player.rot[0] +=  rotspeed;
		else if(dgetk(DK_UP) > dgetk(DK_DOWN))    game.player.rot[0] += -rotspeed;

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
				pdir[2] = -lthumb->rel[1];

				pdir[0] *= 10;
				pdir[2] *= 10;

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
		// pdir *= dgetk(DK_LSHIFT)? pspeed/2 : pspeed;
		pdir *= slow_input? pslowspeed : pspeed;

		//jump
		float pjumpspeed = 0;
		if (dusek(' ')) jump_input=true;
		if (De.finger_click == true) {
			De.finger_click = false;
			jump_input=true;
		}

		if(jump_input) pjumpspeed = 12*2;

		pdir[1] = pjumpspeed;
		game.player.vel[1] += grav;

		if(pjumpspeed) //stop falling
			game.player.vel[1] = max(0, game.player.vel[1]);


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

// CAMERA

//fps view
		// De.cam.pos = game.player.pos + .5;
		// De.cam.rot = game.player.rot;
		De.cam.fov = PI/3.0;
//tps view
		v4f camtrans = {0,5,-16,0};

		De.cam.pos = game.player.pos + v4f_mtmul(camtrans, m4f_rotation(-game.player.rot));
		De.cam.pos[1] = max(De.cam.pos[1], hfunc(De.cam.pos[0],De.cam.pos[2])+1); // cam collides with hmap
		De.cam.rot = game.player.rot;

		dcam_update();
		game.mvp = De.cam.vp;





		v4f transfocus = game.player.pos + v4f_mtmul((v4f){0,0,16,0}, m4f_rotation(-game.player.rot));
		transfocus[1] = game.player.pos[1];
		hmap_update(game.hmap, transfocus);
		// hmap_update(game.hmap, game.player.pos);
		bullets_update(game.enemy_bullets, .016f);

		bullets_draw(game.enemy_bullets);
		hmap_draw(game.hmap);

		game.frame++;

	}

	mgame_quit();
}
























// function for the hmap
float hfunc(float x,float y){
	// return 0;

	v2d p = {x,y};

	float yf = game.frame/50.f;
	v4d p3 = {p[0], p[1], yf};
	float n1 = noise3(p3/10.f)*3;
	n1*=n1;
	return n1;
	float h = n1;

	if(h<.5) {
		p[1] = y+game.frame/10.f;
	if(h<.05)
		h -= fabs(noise2(p/25));
	else h -= fabs(noise2(p/125));
	}
	return h;
}

































