#include "hfunc0.h"
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
		f=tan(i*1.1)*19343.0;
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

	// x=fmodf(x+G.ftime, 6);
	// y=fmodf(y+G.ftime, 6);
	float hfvv = hfv_read(x,y);

	if (x>-2 && x<6 && y>-1 && y<6) return 4;

	// return 0;

	v2d p = {x,y};

	// p += G.ftime;
	// p[0]=fmod(p[0], 6.0);
	// p[1]=fmod(p[1], 6.0);

	// return -v2d_len(p)/2 + noise2((p+G.frame/50.f)/10.f);
	v2i per={10,10};
	double bign=pow(noise2p(p*.01, per), 2.0)*91.0;
	// return hfvv+bign+noise2p(p/(bign+5.2+G.frame/300.0), per)*1.3;

	// v4f tp = v4f_mmul((v4f){x,0,y,0}, m4f_inverse(G.hmap0->m));
	// v4f tp = ((v4f){x,0,y,0})/G.hmap0->sca;
	// v2d p = {tp[0], tp[2]};

	float yf = G.frame/50.f;
	// float yf = 0;
	v4d p3 = {p[0], p[1], yf};
	float n1 = noise3(p3/10.f)*.3;
	n1*=n1;

	// n1= n1-v2d_len(p)/2;
	if (fmodf(v2f_len(
			(v2f){x,y} - (v2f){0,3}
		// (v2f){x,y} - (v2f){G.enm[0].pos[0], G.enm[0].pos[1]}
			), 5) <2
		) n1 = n1 + 1;
	
	n1 += v2d_len(p)>15? -3: 0;
	if(v2d_len(p-10.0)<25) n1+= CLAMP(noise2(p/100)+.5,0,100)*8;
	// if(v2d_len(p-10.0)<150) n1+= .1 * ( 2*pow(p[0]/10, 4.0)+pow(p[1]/10, 2.0) );
	if(hfvv > .1)
		n1 /= hfvv*4.0;
		// return hfvv / 2.0;
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


