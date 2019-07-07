#include "types.h"

// some bullets:
bullet bullet_0 = {
	pos:{0,0,0,0}, col0:{0,0,0,0}, col1:{0,0,0,0},
	vel:{0,0,0,0}, acc:{0,0,0,0}, force:{0,0,0,0},
	rad:0, tvel:0, tacc:0, lifetime:0, dmg:0,
	inited:0, frame:0,
	run: NULL
};
bullet bullet_default = {
	pos:{0,0,0,0}, col0:{255,0,0,255}, col1:{255,255,255,255},
	vel:{0,0,0,0}, acc:{0,-9,0,0}, force:{0,0,0,0},
	rad:0, tvel:50, tacc:50, lifetime:0, dmg:0,
	inited:0, frame:0,
	run: NULL
};


#include "../../../de/de.h"
#include "../mgame.h"


def_bullet(glider){
	if(!in->inited){
		glider.inited = true;
		// glider.pos = in->pos;
		// glider.vel = in->vel;
		// glider.acc = in->acc;
		// glider.col0 = (v4c){255,0,0,255};
		// glider.col1 = (v4c){255,255,255,255};
		// glider.rad = in->rad;
		// glider.dmg = 1;
		glider.tvel = 100.0;
		glider.tacc = 100.0;
		glider.lifetime = 60*1000;
		glider.frame = 0;
		bullets* buf = in->buf;
		// *in = glider;
		in->buf = buf;
	}
	if (in->lifetime - in->frame < 6) in->col1[3]/=2;
	// float h = G.hmap0->f(in->pos[0], in->pos[2]);
	float h = hmap_eval(G.hmap0, in->pos[0], in->pos[2]);

	// if(h + in->rad + G.player.height/2 > in->pos[1]){
	if(h + G.player.height + in->rad > in->pos[1]){
		// in->pos[1] = h + in->rad + G.player.height/2;
		in->pos[1] = h + G.player.height + in->rad;
		in->vel[1] = 0;
	} else {
		// in->vel[1] = in->grav;
	}
// player collision
	if(v3f_len( in->pos - G.player.pos ) < G.player.rad /*.2/4*/ + in->rad*0.5 ){
		in->col0 *= 0;
		bullet* b = bullets_add(in->buf, &playertrace);
		b->vel = in->vel;
		// b->vel[1] = -in->vel[1]/10.0;
		b->vel[1] = 0;
		b->acc[1] = 1;
		b->pos = in->pos;
		b->frame = -2000;
		// b->col1 = playertrace.col1;
		in->frame = in->lifetime+1;
		dsnd_play(G.phitsnd);
	}

}

def_bullet(playertrace){
	if(!in->inited){
		playertrace.inited = true;
		playertrace.pos = in->pos;
		playertrace.vel = in->vel;
		playertrace.acc = in->acc;
		playertrace.col0 = (v4c){0,0,255,64};
		playertrace.col1 = (v4c){64,255,255,255};
		playertrace.rad = .2;
		playertrace.dmg = 1;
		playertrace.tvel = 500.0;
		playertrace.tacc = 5.0;
		playertrace.lifetime = 10;
		playertrace.frame = 0;
		bullets* buf = in->buf;
		*in = playertrace;
		in->buf = buf;
	}
	// in->rad = MAX(in->rad-0.01, 0);
	in->rad = MAX(in->rad - 0.01, 0.03);
	in->vel += v4f_rand()-.5;
	// in->vel *= 2;
	// in->col0[3] = MAX(in->col0[3] - 256 / in->lifetime, 0);
	// in->col1[3] = MAX(in->col1[3] - 256 / in->lifetime, 64);
	// in->col1[3] -= 256 / in->lifetime ;
}

def_bullet(playeraura){
	if(!in->inited){
		playeraura.inited = true;
		playeraura.pos = in->pos;
		playeraura.vel = in->vel;
		playeraura.acc = in->acc;
		playeraura.col0 = (v4c){0,0,255,255};
		playeraura.col1 = (v4c){64,255,255,255};
		playeraura.rad = in->rad;
		playeraura.dmg = 1;
		playeraura.tvel = 5.0;
		playeraura.tacc = 5.0;
		playeraura.lifetime = 1;
		playeraura.frame = 0;
		bullets* buf = in->buf;
		*in = playeraura;
		in->buf = buf;
	}
	in->rad*=2-in->frame;
	// in->rad = MAX(in->rad-0.01, 0);
}




// bgen0_fnc
// bgen0_
def_bullet(joe_spark){
	if(!in->inited){
		joe_spark.inited = true;
		// joe_spark.pos = (v4f){0,0,500,1};
		joe_spark.pos = in->pos;
		joe_spark.vel = in->vel;
		joe_spark.acc = in->acc;
		joe_spark.col0 = (v4c){255,0,0,255};
		joe_spark.col1 = (v4c){255,255,255,255};
		joe_spark.rad = .2;
		joe_spark.dmg = 1;
		joe_spark.tvel = 5.0;
		joe_spark.tacc = 5.0;
		joe_spark.lifetime = 60*5;
		joe_spark.frame = 0;
		bullets* buf = in->buf;
		*in = joe_spark;
		in->buf = buf;
	}
	//player colision
	// if(v3f_len( in->pos - G.player.pos ) < /*G.player.rad*/ .2/4 + in->rad/2 ){
	if(v3f_len( in->pos - G.player.pos ) < G.player.rad + in->rad/2 ){
		in->col0 *= 0;
		bullet* b = bullets_add(in->buf, &playertrace);
		b->vel = in->vel;
		// b->vel[1] = -in->vel[1]/10.0;
		b->vel[1] = 0;
		b->acc[1] = 1;
		b->pos = in->pos;
		b->frame = -2000;
		// b->col1 = playertrace.col1;
		in->frame = in->lifetime+1;
	}
	// if(G.hmap0->f(in->pos[0], in->pos[2]) >= in->pos[1] - in->rad) {in->vel[1]*=-1;in->pos+=in->vel*0.016f;/*in->acc*=0;*/ in->lifetime += 100;}
	// if(in->pos[1] < -2 ) {in->vel[1]*=-1;in->pos[1]=-2;/*in->acc*=0;*/ in->lifetime += 100;}
	// in->rad = MAX(in->rad-.05, 0);
	// in->rad = lerp(joe_spark.rad, 0, in->frame*1.f / in->lifetime );
}



def_bullet(joe){
	if(!in->inited){
		joe.inited = true;
		joe.pos = in->pos;
		joe.vel = in->vel;
		joe.acc = (v4f){0,-9,0,1};
		joe.col0 = (v4c){0,255,0,255};
		joe.col1 = (v4c){255,255,255,255};
		joe.rad = 1;
		joe.dmg = 1;
		joe.tvel = 5.0;
		joe.tacc = 5.0;
		joe.lifetime = 60*2;
		joe.frame = 0;
		bullets* buf = in->buf;
		*in = joe;
		in->buf = buf;
	}
	int cut = 60*1+30;
	if (in->frame % 15 == 0){
		bullet* child;
		in->rad/=2;
		bool repeat = in->frame < cut;
		v4f pos = in->pos;

		in->vel += v4f_rand()*50-25;
		child = bullets_add(in->buf, repeat ? in : &joe_spark );
		if(repeat){
			child->col0 = v4c_rand();
		} else {
			child->frame = 0;
			child->pos = pos;
			child->vel = in->vel;
			child->acc[1] = -90;
		}

		in->vel += v4f_rand()*50-25;
		child = bullets_add(in->buf, repeat ? in : &joe_spark );
		if(repeat){
			child->col0 = v4c_rand();
		} else {
			child->frame = 0;
			child->pos = pos;
			child->vel = in->vel;
			child->acc[1] = -90;
		}

		in->vel += v4f_rand()*50-25;
		child = bullets_add(in->buf, repeat ? in : &joe_spark );
		if(repeat){
			child->col0 = v4c_rand();
		} else {
			child->frame = 0;
			child->pos = pos;
			child->vel = in->vel;
			child->acc[1] = -90;
		}
		if(!repeat) bullet_clear(in);
		in->vel += v4f_rand()*50-25;

	}
}
























def_bullet(Pbul1){
	if(!in->inited){
		Pbul1.inited = true;
		Pbul1.pos = in->pos;
		Pbul1.vel = in->vel;
		Pbul1.acc = v4f_0;//(v4f){0,-9,0,1};
		Pbul1.col0 = (v4c){0,0,255,55};
		Pbul1.col1 = (v4c){255,255,255,55};
		Pbul1.rad = .2;
		Pbul1.dmg = 1;
		Pbul1.tvel = 50.0;
		Pbul1.tacc = 1000.0;
		Pbul1.lifetime = 60*8;
		Pbul1.frame = 0;
		bullets* buf = in->buf;
		*in = Pbul1;
		in->buf = buf;
	}

	// in->rad = MIN(in->rad+0.01, 0.06);
	float dist2pl = v3f_len2(G.player.pos - in->pos);
	if(dist2pl<0.1){
		in->rad=.01;
		in->col1[3] = 255;
	} else {
		in->rad = MIN(dist2pl/25.0+0.01, 0.06);
		in->col1[3] = MIN(dist2pl*25, 255);
	}

	if (v3f_len2(G.enm[0].pos - in->pos) < .1){//hit enemy
		in->frame=in->lifetime+1;
		G.enm[0].hp -= in->dmg;

		if(in->col0[2]){
			bullet* b = bullets_add(in->buf, &Pbul1_hit);
			b->pos = in->pos;
		}
	}

	// float h = G.hmap0->f(in->pos[0], in->pos[2]);
	// float h = hmap_eval(G.hmap0, in->pos[0], in->pos[2]);
	//track enemy
	// in->vel = v4f_normalize(G.enm[0].pos - in->pos) * 4;

	float h = hmap_eval(G.hmap0, in->pos[0]+in->vel[0]*G.dt, in->pos[2]+in->vel[2]*G.dt) - in->vel[1]*G.dt;
	// h += v4f_len(in->vel)*G.dt;
//	// if(in->pos[1] < h) in->frame += in->lifetime+1;
//	if(in->pos[1] < h) in->rad = 0;
//	// if(h + G.player.height + G.player.rad > in->pos[1]){
//	if(in->frame <= 25)
//		in->col0[2] -= 10;
//	else in->vel = v4f_normalize(G.enm[0].pos - in->pos) * 10;

	in->pos[1] = MAX(h+in->rad, in->pos[1]);


}




def_bullet(Pbul1_hit){
	if(!in->inited){
		Pbul1_hit.inited = true;
		Pbul1_hit.pos = in->pos;
		Pbul1_hit.vel = v4f_0;
		Pbul1_hit.acc = v4f_0;//(v4f){0,-9,0,1};
		Pbul1_hit.col0 = (v4c){0,0,255,5};
		Pbul1_hit.col1 = (v4c){255,255,255,5};
		// Pbul1_hit.col1 = v4c_0;
		Pbul1_hit.rad = .25;
		Pbul1_hit.dmg = 1;
		Pbul1_hit.tvel = 50.0;
		Pbul1_hit.tacc = 1000.0;
		Pbul1_hit.lifetime = 3;
		Pbul1_hit.frame = 0;
		bullets* buf = in->buf;
		*in = Pbul1_hit;
		in->buf = buf;
	}
	// in->col0 = v4c_rand();
	// in->rad -= .2;
	in->col0[3] = 55/(in->frame+1);
	in->col1[3] = 55/(in->frame+1);
	// in->col0[0] = 255;

}
