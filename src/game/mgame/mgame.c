// #include "../de/de.h"
#include "mgame.h"

#define hmap_res 32
// #define hmap_res 40
// #define hmap_res 64

struct game G;
float* sndchunk;

void mgame_init(void);
void mgame_quit(void);
void mplayer_update(void);
float hfunc(float,float); // hma function

void mgame_quit(void){
	bullets_free(G.enemy_bullets);
	bullets_free(G.player_bullets);
	bullets_quit();
	free(sndchunk);
}




// player
bool track_enm = false;
float fric = 1.33;
float Grav = -9*2 * 0.016f;
// float tvel = 8.f;
float tvely = 10.f;
float rotspeed = (PI/90);
bool mouse_grab=1;
float Pjumpspeed = 9;//5; //14*4
int Pgrounded = 1;
int Pdashing_frames = 7;
float Pdashing_speed = 3;
int Pdashing = 0;
float pspeed = 1.25f/3;
float pslowspeed = 1.25f/5; // pspeed/2;
struct dfinger* lthumb = NULL;
struct dfinger* rthumb = NULL;

void mgame_init(void){
	bullets_init( num_enemy_bullets );
	G.enemy_bullets = bullets_new(num_enemy_bullets,true,   0);
	G.player_bullets = bullets_new(num_player_bullets,false,0);
	G.dt = 0.016f;
	G.player.pos = v4f_0;
	G.player.vel = v4f_0;
	G.player.rad = .075/2;
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

		sndchunk[i] = r;
		i++;
		sndchunk[i] = r;
	}

	dsnd* testpsnd = dsnd_new(sndchunk, smplen * 2 * sizeof(float));

	dsnd_fade(dsnd_playl(testpsnd, -1), 100000);

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
		s*=.25; // volume
		blip[i] = s;
		blip[++i] = s;
	}
	G.phitsnd = dsnd_new(blip, smplen *4*2);

	G.screensmp = malloc(De.size[0]* De.size[1]* sizeof(float) * 2);
}





void mgame_main(void){
	mgame_init();
	int frame = 0;

	struct enemy enemy;
	v4f spinning_dmk_dir = v4f_0;
	spinning_dmk_dir[0] = 1;
	spinning_dmk_dir[1] = -10;
	bool dmktsel = 0;
	enemy.pos = (v4f){0,0,3,0};//v4f_0;
	enemy.vel = v4f_0;





	DE_GAME_LOOP(){
		if(dusek(DK_ESC) || dusek(DK_BACK)) break;
		frame++;
		G.clear_color = (v4f){.2, .08, .202}/2.0;
		// G.clear_color = (v4f){0,0,0,0};
		// G.clear_color = (v4f){0.5,0,0.5,0};
		dclear_color(G.clear_color);





/* move to bullet type*/
//sphere dmk
if (frame%160 == 0) {

	dmktsel = (dmktsel+1) % 2;

	enemy.vel = dmktsel? (enemy.pos - G.player.pos) : v4f_0;
	enemy.vel[1] = 0;
	enemy.vel = v4f_normalize(enemy.vel)*-.5f;

	// enemy.vel[0] = dmktsel? (((1.f*rand())/INT_MAX)-.5f)*2 : 0;
	// enemy.vel[2] = dmktsel? (((1.f*rand())/INT_MAX)-.5f)*2 : 0;

#if 0
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
				b->pos=enemy.pos;
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
	b->pos=enemy.pos;
	// b->pos[1]+=.75;
	// b->vel = v4f_rand()*2-1;
	float t = G.frame/PI*(3+dmktsel+1);
	t*=dmktsel+1;
	// t*=4;
	b->vel = (v4f){cos(t),0,sin(t),0};
	b->vel *= fabsf(sin(t)) + .5f;
	b->vel /= 2;
	b->vel[1] = 1;
	b->acc[1]=-1;
	b->rad = .075;
	b->col1 = (v4c){255,255,255,255};
	// b->col1 = v4c_rand();
	b->col0 = (v4c){(dmktsel%2)*255,255,0,255};
}


// spinning dmk
// if(!(frame%2)){
if(true){
	for (int i = 0; i < 1; ++i){
		bullet* tor = bullets_add(G.enemy_bullets, &glider);
		tor->pos = enemy.pos;
		tor->vel = spinning_dmk_dir *0.7f;//* 1.5;
		tor->acc[1] = -2;
		tor->rad = 0.04;
		tor->col0[2] = (dmktsel%2)*255;
		spinning_dmk_dir = v4f_mmul(spinning_dmk_dir, m4f_rotation_y(PI/6.0));
	}
	if(dmktsel) spinning_dmk_dir = v4f_mmul(spinning_dmk_dir, m4f_rotation_y(-PI/(6.0/4)+0.12));
	else spinning_dmk_dir = v4f_mmul(spinning_dmk_dir, m4f_rotation_y(-PI/(6.0/3)+0.11));
}



enemy.pos += enemy.vel * G.dt;
enemy.pos[1] = MAX(G.player.pos[1], hmap_eval(G.hmap0, enemy.pos[0], enemy.pos[2])) + 0.1;

G.enm[0] = enemy;





		mplayer_update();

		bullets_update(G.enemy_bullets, .016f);
		bullets_update(G.player_bullets, .016f);

		hmap_update(G.hmap0, G.player.pos);

		bullets_draw(G.enemy_bullets);

		hmap_draw(G.hmap0);

		bullets_draw(G.player_bullets);

		G.frame++;
	}

	mgame_quit();

}



















#define HFS 256
bool _once = false;
int hfv_size = HFS;
// char hfv[128*128];
char hfv[HFS*HFS];
#undef HFS

void init_hfunc( void );

char hfv_read(float x, float y){
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
	v2i w = {hfv_size/2, hfv_size/2};
	for (int i = 0; i < steps; ++i){
		d=rand()%4;
		if(d==0) w[0] = MIN(w[0]+1, hfv_size-1);
		if(d==1) w[0] = MAX(w[0]-1, 0);
		if(d==2) w[1] = MIN(w[1]+1, hfv_size-1);
		if(d==3) w[1] = MAX(w[1]-1, 0);
		hfv[ w[0]*hfv_size + w[1] ]=2;
		hfv[ MIN(w[0]+1,hfv_size-1)*hfv_size + w[1] ]=2;
		hfv[ w[0]*hfv_size + MIN(w[1]+1,hfv_size-1) ]=2;
		hfv[ MIN(w[0]+1,hfv_size-1)*hfv_size + MIN(w[1]+1,hfv_size-1) ]=2;
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

	if (x>-2 && x<6 && y>-1 && y<6) return 0;

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

	n1= n1-v2d_len(p)/2;
if (fmodf(v2f_len(
	(v2f){x,y} - (v2f){0,3}
), 5) <2) n1=n1 + 1;
	n1 += v2d_len(p)>15? 10: 0;
	if(hfvv != 0.0)
		return hfvv / 8.0;
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

































void mplayer_update(void){
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
	if(dusek('q')) { mouse_grab = (mouse_grab+1) %2;}

	if(mouse_grab) dmouse_grab(1);
	else dmouse_grab(0);

	bool jump_input = 0;
	bool slow_input = 0;


//player input
	v4f pdir = v4f_0;
	if     (dgetk('w') > dgetk('s')) pdir[2] =  1;
	else if(dgetk('s') > dgetk('w')) pdir[2] = -1;
	if     (dgetk('d') > dgetk('a')) pdir[0] =  1;
	else if(dgetk('a') > dgetk('d')) pdir[0] = -1;
	if     (dgetk(' ') > dgetk('x')) pdir[1] =  1;
	else if(dgetk('x') > dgetk(' ')) pdir[1] = -1;

// if(mouse_grab){
	if     (dgetk(DK_UP) > dgetk(DK_DOWN)) pdir[2] =  1;
	else if(dgetk(DK_DOWN) > dgetk(DK_UP)) pdir[2] = -1;
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
			pdir[2] = -lthumb->rel[1];

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



	// G.player.rot[0] = clamp(G.player.rot[0], -PI/2, PI/2);
	G.player.rot[0] = clamp(G.player.rot[0], -PI/4, PI/2);

	// G.player.rot = v4f_mmul((v4f){0,0,1,1}, m4f_look_at(G.player.pos, enemy.pos, (v4f){0,1,0,0}) );


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
	pdir[0] = clamp(pdir[0], minx, maxx);
	pdir[2] = clamp(pdir[2], minz, maxz);


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
			// dusek(' ');
			pdir[1] = pjumpspeed;
			Pgrounded = 0;
		} else{
			pdir[1] = 0;
			// pdir = Pdashing_speed*pdir;
			Pdashing = Pdashing_frames;
			// Pdashing=clamp(Pdashing, 0, 160);
		}
		// dsnd_play(G.phitsnd);
	}

	if (Pdashing) {
		Pdashing -=1;
		pdir *= Pdashing_speed;
	}
//gravity
	else G.player.vel[1] += Grav;
	if(pjumpspeed) //stop falling
		G.player.vel[1] = MAX(0, G.player.vel[1]);




// apply rotation to input
	pdir = v4f_mmul(pdir, m4f_rotation_y(G.player.rot[1]) );




//slow
	if(De.mouse.b2 || dgetk(DK_LSHIFT) || dgetk(DK_RSHIFT) || dgetk(DK_RCTRL) || dgetk(DK_LCTRL)){
		slow_input = 1;
	}


// apply speed
	float pyd = pdir[1];
	pdir *= slow_input? pslowspeed : pspeed;
	pdir[1]=pyd*pspeed;




	G.player.vel += pdir;
	G.player.vel += G.player.acc * G.dt;

	// G.player.vel[0] = clamp(G.player.vel[0], -tvel, tvel);
	G.player.vel[1] = clamp(G.player.vel[1], -tvely, tvely);
	// G.player.vel[2] = clamp(G.player.vel[2], -tvel, tvel);

	G.player.pos += G.player.vel * G.dt;
// ground
	float h = hmap_eval(G.hmap0, G.player.pos[0], G.player.pos[2]);

	if(G.player.pos[1] <= h + G.player.rad+G.player.height){
		G.player.pos[1] = h + G.player.rad+G.player.height;
		G.player.acc[1] = 0;
		Pgrounded+=1;
		// G.player.vel[1] = 0;
	}

	G.player.vel[0] /= fric;
	G.player.vel[2] /= fric;




// CAMERA
//fps view
	// De.cam.pos = G.player.pos + .5;
	// De.cam.rot = G.player.rot;
	De.cam.fov = PI/3.0;
//tps view
	// v4f camtrans = {0,.5,-16,0};
	v4f camtrans = {0,1.75,-4.96,0};
	camtrans /= 1.5;


	v4f camrot = {G.player.rot[0], G.player.rot[1], 0,0};

	De.cam.pos = G.player.pos + v4f_mtmul(camtrans, m4f_rotation(-camrot));

	// cam collides with hmap fixme todo
	De.cam.pos[1] = MAX(De.cam.pos[1], hmap_eval(G.hmap0, De.cam.pos[0],De.cam.pos[2])+.05);
	// De.cam.pos = G.player.pos + v4f_mtmul(camtrans, m4f_rotation(-G.player.rot);
	// De.cam.rot = G.player.rot * (v4f){0,1,0,0};
	De.cam.rot = camrot;

	dcam_update();











// shoot
	// if(!Pgrounded) goto skip_shoot;
	if(G.frame % 2) goto skip_shoot;

	float pe_len2 = v4f_len2(G.player.pos - G.enm[0].pos);
bool pshoot = false;
	if (dgetk(DK_VOL_UP)) pshoot=true;
	if(pe_len2 < 14*14 || pshoot){
		bullet* b = bullets_add(G.player_bullets, &Pbul1);
		b->pos = G.player.pos;
		// b->pos[1] += .2;
		// b->rad = 0.00;
		b->vel = (v4f){0,0,10,0} + (v4f_rand() -.5)*0;
		b->vel = v4f_mmul(b->vel, m4f_rotation_y(G.player.rot[1]));
	}
	if(pe_len2 < 5*5 /*&& Pgrounded>60*/ || pshoot){
v4f vel = (v4f){0,0,10,0} + (v4f_rand() -.5)*0;
vel = v4f_mmul(vel, m4f_rotation_y(G.player.rot[1]));

	v4f tp = {.1,0,0,0};
	tp = v4f_mmul(tp, m4f_rotation_y(G.player.rot[1]));
		float s = sin(G.frame/4.f);
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
	pt = bullets_add(G.enemy_bullets, &playeraura),
	pt->rad = G.player.rad;
	pt->pos = G.player.pos;
	pt->vel = v4f_0;
	// pt->lifetime=0;
}
