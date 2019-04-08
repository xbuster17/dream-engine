#include "../../de/de.h"

#define NUM_BALLS 10

struct shdtest{
	dvao* quad;
	dshd* shd0;
	dshd* shd1;
	dshd* shd2;
	char *vert, *frag;

	v4f u0;
	dfbo* t0fbo;
	v4f ball[NUM_BALLS];
	v4f ballc[NUM_BALLS];

} Shdtest = {
	.t0fbo=NULL,
	.shd0=NULL,
	.shd1=NULL,
	.shd2=NULL,
};


void shdt_make_texture(void){
	if(Shdtest.t0fbo == NULL){
		Shdtest.t0fbo = dfbo_new(512,512);
		// Shdtest.t0fbo = dfbo_new(128,128);
	}
	dfbo_bind(Shdtest.t0fbo);
	dclear(0);
		// dcam_update();
		// dshd_bind(Shdtest.shd0);
		dshd_update(Shdtest.shd0);
		dvao_draw(Shdtest.quad, 0, 0, 0);
	dfbo_bind(0);
}

void shdt_make_shaders(void);
v2f mouse;
static int _zero = 0;




void ex_shd_metaball(void){
	Shdtest.u0 = v4f_0;
	dmouse_hide(1);
	dblend(0);
	ddepth(0,0);

	shdt_make_shaders();


	v4f qpos[]={
		{-1,-1,0,1},
		{-1,1,0,1},
		{1,-1,0,1},
		{1,1,0,1}
	};
	void* quadverts[] = {qpos};
	Shdtest.quad = dvao_new("f4", quadverts, 4, GL_TRIANGLE_STRIP);


	DE_GAME_LOOP(){ if(dusek(DK_BACK) || dusek(DK_ESC) ) break;
		// if(_recompileshd) {
		if(dusek('r'))
			shdt_make_shaders();
			// _recompileshd = false;
		// }

		struct dfinger* touch = dfinger_in_area(0,0, 1,1);
		if(touch){
			mouse = touch->abs;
			De.mouse.abs = v2_cast(touch->abs * v2_cast(De.size, v2f) , v2i);
		} else {
			mouse = (v2f){De.mouse.abs[0], De.mouse.abs[1]};
			mouse[0]/=De.size[0]-1;
			mouse[1]/=De.size[1]-1;
		}

		Shdtest.u0[0] = De.ctx_size[0];
		Shdtest.u0[1] = De.ctx_size[1];
		Shdtest.u0[2] = De.ctx_res;
		Shdtest.u0[3] = De.game_loop_frame;

		De.cam.pos[0] = sin(De.game_loop_frame / 10.f)*2.0;
		De.cam.pos[2] += !!dgetk('w') - !!dgetk('s');
		// De.cam.pos[2] = -0.9;
// shdt_make_texture();

	for (int i = 0; i<NUM_BALLS; i++){
		// Shdtest.ball[i] = (v4f){sinf(i*2.f), cosf(i*2.f), 0, .41};
		Shdtest.ball[i][0] = noise1((i+1)*500.293+De.game_loop_frame/100.0);
		Shdtest.ball[i][1] = noise1((i+1)*92500.231+De.game_loop_frame/100.0);
		Shdtest.ball[i][2] = 0;
		// Shdtest.ball[i][3] = noise1((i+1)*4125.231+De.game_loop_frame/1000.0)/4.0;
		// Shdtest.ball[i][3] = (1)/40.0;
		// Shdtest.ball[i][3] = (i+5)/80.0;
		Shdtest.ball[i][3] = .02;

		// float n = noise1((i+1)*92500.231+De.game_loop_frame/100.0);
		if(dgetk(' '))
			Shdtest.ballc[i] += sinf(Shdtest.ball[i][0]) * v4f_rand()/10.f;
			// Shdtest.ballc[i] = v4f_rand();

		Shdtest.ballc[i][3] = 1;
	}

		// dcam_reset();
		dcam_update();

	// dshd_bind(Shdtest.shd);
		// dtex_bind(tfbo->color, _zero);

		// dshd_update(Shdtest.shd1);

		dshd_update(Shdtest.shd0);
		dvao_draw(Shdtest.quad, 0, 0, 0);

		dshd_update(Shdtest.shd2);
		dblend(1);
		dvao_draw(Shdtest.quad, 0, 0, 0);

		// dtex_bind(Shdtest.t0fbo->color, _zero);
// dtex_filter(Shdtest.t0fbo->color, GL_NEAREST, GL_NEAREST);
// dtex_draw(Shdtest.t0fbo->color);

	}
}






#define SHDT_BASE_VERT \
DSHD_QUOTE(\
	attribute vec4 p;\
	varying vec4 vp;\
	varying vec2 vtc;\
	void main(){\
		vp = p;\
		vtc = p.xy/2.0+vec2(1.0,1.0);\
		gl_Position = p;\
	}\
)
// #define SHDT_BASE_FRAG



void shdt_make_shaders(void){
	Shdtest.vert = SHDT_BASE_VERT;
	Shdtest.frag = DSHD_QUOTE(
		uniform vec4 u0;
		uniform vec2 umouse;
		varying vec4 vp;
		float hash(vec2 p){ return fract(1e4*sin(17.*p.x + p.y*.1) * (.1+abs(sin(p.y*13.+p.x)))); }
		float noise(vec2 p){
			const vec2 d = vec2(0.0,1.0);
			vec2 b = floor(p);
			vec2 f = smoothstep(vec2(0.0), vec2(1.0), fract(p));
			return mix(
				mix(
					hash(b), hash(b+d.yx), f.x
				),
				mix(
					hash(b+d.xy), hash(b+d.yy), f.x
				), f.y);
		}

		void main(){
			// vec4 c0 = /*mod(vp*4.0,1.0)*/vp*sin(dot(vp,vp)*PI+u0.a/10.);
			vec4 c0;// = mod(vp*4.0,1.0)*sin(dot(vp,vp)*PI+u0.a/10.);
			vec2 p = vp.xy;
			p *= umouse+vec2(.011);
			//8713.2533
			// float val = fract(sin(dot(p,vec2(12.9898, 4.1414)) * 43758.5453));
			float val = hash(p * u0.a/1000.0);
			// float val = noise(p * u0.a);
			// float val = fract(sin(dot(p*p,vec2(8713.2533, 8713.2533))*u0.a * 43758.5453));

			c0 = vec4(1.,1.,1.,1.) *
			vec4(
				// fract(vp.x*vp.y*floor(u0.a/100.0)*1234.123+u0.a/100.0),
				// fract(vp.x*vp.y*floor(u0.a/100.0)*1234.133+u0.a/200.0),
				// fract(vp.x*vp.y*floor(u0.a/100.0)*1234.143+u0.a/300.0),
				fract(p.x*p.y*floor(u0.a/100.0)*8713.2533+u0.a/100.0),
				fract(p.x*p.y*floor(u0.a/100.0)*2955.2491+u0.a/100.0),
				fract(p.x*p.y*floor(u0.a/100.0)*7153.7553+u0.a/100.0),
				// val,val,val,
				1.0
				);
			gl_FragColor = c0;
		}
	);


	if(Shdtest.shd0) dshd_free(Shdtest.shd0);
	Shdtest.shd0 = dshd_new(Shdtest.vert, Shdtest.frag);
	// Shdtest.shd0 = dshd_open(DE_ASSET("base.vert"),DE_ASSET("demo0.frag"));

		// dshd_unif(Shdtest.shd0, "cam", &De.cam.vp);
	dshd_unif(Shdtest.shd0, "u0", &Shdtest.u0);
	dshd_unif(Shdtest.shd0, "umouse", &mouse);






	Shdtest.frag = DSHD_QUOTE(
		// uniform mat4 cam;
		uniform vec4 u0;
		uniform sampler2D t0;
		varying vec4 vp;
		varying vec2 vtc;
		void main(){
			// vec2 vtc = vp.xy/2.0 + vec2(1.,1.);
			float a = atan(vp.x, vp.y);
			float r = length( vp );
			vec2 uv = vec2(1.0/r + u0.a , a/PI);
			gl_FragColor = texture2D(t0, uv);
		}
	);
	if(Shdtest.shd1) dshd_free(Shdtest.shd1);
	Shdtest.shd1 = dshd_new(Shdtest.vert, Shdtest.frag);

	dshd_unif(Shdtest.shd1, "u0", &Shdtest.u0);
	dshd_unif(Shdtest.shd1, "t0", &_zero);





	Shdtest.frag = DSHD_QUOTE(
		// uniform vec4 u0;
		uniform mat4 mvp;
		uniform vec4 ball[NUM_BALLS];
		uniform vec4 ballc[NUM_BALLS];
		varying vec4 vp;
		varying vec2 vtc;
		void main(){
			float v = 0.0;
			vec4 col = vec4(0.0,0.0,0.0,0.0);
			for(int i=0; i<NUM_BALLS; i++){
				vec4 b = vec4(ball[i].xyz/2.0+.5, 1.0);
				// vec4 b = ball[i];
				float rad = ball[i].a;

				vec4 uvp = vp ;
				float a = abs(atan(uvp.x, uvp.y));
				float r = pow(length(uvp/**1.5*/),6.0);
				// float r = length(uvp/**1.5*/)*2.5;
				vec2 uv = vec2(1.0/r /*+ u0.a */, a/PI);

				// vec2 uv = (vp.xy + 0.5) ;
				// vec2 uv = vp.xy ;

				float dx = b.x - uv.x;
				float dy = b.y - uv.y;
				// float dx = b.x - vp.x;
				// float dx = b.x - (gl_FragCoord.x/1024.0-.5)*2.0;
				// float dy = b.y - vp.y;
				// float dy = b.y - (gl_FragCoord.y/768.0 -.5)*2.0;
				v += rad*rad / (dx*dx+dy*dy);
				// col += ballc[i] * v*mod(v,5.1);
				// col += ballc[i] * sin((v)/10.0);
				// col.a = ballc[i].a * sin((v-1.0)*10.0);
				col += ballc[i] * v;
				// col = (clamp(col.a, 0.0, 1.0));
				col.a = (clamp(col.a, 0.0, 1.0));
				// col.a = sin(clamp(col.a, 0.0, 1.0));
				// col = mod(col, 2.0);
			}
			// gl_FragColor = vec4(v,v,v,v);
			// gl_FragColor = pow(col, vec4(2.13));
			gl_FragColor = col;
		}
	);
	if(Shdtest.shd2) dshd_free(Shdtest.shd2);
	Shdtest.shd2 = dshd_new(Shdtest.vert, Shdtest.frag);

	// dshd_unif(Shdtest.shd2, "mvp", &De.cam.vp);
	char ballus[] = "ball[0]";
	for (int i = 0; i<NUM_BALLS; i++){
		dshd_unif(Shdtest.shd2, ballus, &Shdtest.ball[i]);
		ballus[5]++;
	}

	char ballusc[] = "ballc[0]";
	for (int i = 0; i<NUM_BALLS; i++){
		dshd_unif(Shdtest.shd2, ballusc, &Shdtest.ballc[i]);
		Shdtest.ballc[i] = v4f_rand();
		// Shdtest.ballc[i] = (v4f){1,1,1,1};
		// Shdtest.ballc[i] = (v4f){(i)%2, pow(sin(i),2), (i+1)%2, 1}/8.0;
		Shdtest.ballc[i][3] = 1;

		ballusc[6]++;
	}



}
