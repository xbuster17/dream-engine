#include "bullets.h"


void bullet_set_pos(bullet, v4f pos);
void bullet_set_col(bullet, v4f col);

// void bullets_init(int count); //you should not call bullets new with an int higher than this

char* bullet_vshdsrc;
char* bullet_fshdsrc;

dshd* bullet_shd;
dibo* bullet_ibo;

v4f* bullet_ppos;
v4f* bullet_pctr;
v4c* bullet_pcol0;
v4c* bullet_pcol1;

v4c* bullet_tex_pixels;
int bullet_tex_size = 32;
int bullet_tex_size2 = 32*32;
dtex* bullet_tex;
dfbo* bullet_fbo;
static int zero = 0;

dshd* bullet_shadow_shd = NULL;
char* bullet_shadow_shd_vsrc;
char* bullet_shadow_shd_fsrc;


void bullets_init(uint count){
	//these are initialization buffers to avoid mallocs on _new()
	bullet_ppos = malloc(sizeof(v4f)*count*4);
	bullet_pctr =  calloc(sizeof(v4f), count*4);
	bullet_pcol0 = calloc(sizeof(v4c), count*4);
	bullet_pcol1 = calloc(sizeof(v4c), count*4);

//init bullet vertices
	// v4c col0 = {255,255,255,255};
	// v4c col1 = col0;
	for (uint i = 0; i < count*4; i++){
		// m4f rot = m4f_rotation((v4f){0,0,PI/14.0*i,0});
		// m4f rot = m4f_rotation((v4f){0,0,PI/4,0});
		m4f rot = m4f_rotation((v4f){0,0,0,0});
		bullet_ppos[i] = (v4f){-1,-1, 0, 0};bullet_ppos[i] = v4f_mmul(bullet_ppos[i], rot); i++;
		bullet_ppos[i] = (v4f){ 1,-1, 1, 0};bullet_ppos[i] = v4f_mmul(bullet_ppos[i], rot); i++;
		bullet_ppos[i] = (v4f){-1, 1, 0, 1};bullet_ppos[i] = v4f_mmul(bullet_ppos[i], rot); i++;
		bullet_ppos[i] = (v4f){ 1, 1, 1, 1};bullet_ppos[i] = v4f_mmul(bullet_ppos[i], rot);
		// bullet_ppos[i] = (v4f){-1,-1, 0, 0}; i++;
		// bullet_ppos[i] = (v4f){ 1,-1, 1, 0}; i++;
		// bullet_ppos[i] = (v4f){-1, 1, 0, 1}; i++;
		// bullet_ppos[i] = (v4f){ 1, 1, 1, 1};
	}


//make bullet texture
	bullet_tex_pixels = malloc(sizeof(v4c) * bullet_tex_size * bullet_tex_size);
	for (int i = 0; i < bullet_tex_size; i++){
		for (int j = 0; j < bullet_tex_size; j++){

			bullet_tex_pixels[i * bullet_tex_size + j] = v4c_0;
			float dist = v2f_len2((v2f){i,j} - (v2f){bullet_tex_size/2, bullet_tex_size/2});
			if(dist < bullet_tex_size2 / 4){
			// if(dist > bullet_tex_size2 / 8)
				bullet_tex_pixels[i * bullet_tex_size + j] = (v4c){255,255,255,255};
			}
		}
	}

	bullet_tex = dtex_new(bullet_tex_pixels, bullet_tex_size, bullet_tex_size);
	free(bullet_tex_pixels);

	dtex_filter(bullet_tex, GL_NEAREST, GL_NEAREST);




	//make bullet shader
	bullet_shd = dshd_new( bullet_vshdsrc, bullet_fshdsrc);


	//make bullet ibo
	short* ibo = malloc(sizeof(short) * count * 6);

	int in = 0;
	for(uint i = 0; i<count * 6 -1; i++){
		if( i ){
			ibo[i] = ++in; i++;
		}/*degenerate triangle 2*/
		ibo[i] = in;
		in++; i++;
		ibo[i] = in;
		in++; i++;
		ibo[i] = in;
		in++; i++;
		ibo[i] = in;
		i++;
		ibo[i] = in;/*degenerate triangle 1*/
	}

	bullet_ibo = dibo_new(ibo, count*6, GL_TRIANGLE_STRIP);



//fbo
	bullet_shadow_shd = dshd_new(bullet_shadow_shd_vsrc, bullet_shadow_shd_fsrc);
	// bullet_fbo = dfbo_new(512,512);

	free(ibo);
}



void bullets_quit(void){
	free(bullet_ppos);
	free(bullet_pctr);
	free(bullet_pcol0);
	free(bullet_pcol1);
	dtex_free(bullet_tex);
	dshd_free(bullet_shd);
	dibo_free(bullet_ibo);
}



bullets* bullets_new(uint count){
	bullets* bs = malloc(sizeof(bullets));
	bs->b = malloc(sizeof(bullet) * count);
	bs->len = count;

	for (uint i = 0; i < count; i++){
		bs->b[i] = bullet_0;
	}

	void* verts[] = {bullet_ppos, bullet_pctr, bullet_pcol0, bullet_pcol1};
	bs->vao = dvao_newr("f2p, f2tc; f4ctr; uc4col0; uc4col1", verts, count*4, GL_TRIANGLE_STRIP);

#if defined ANDROID
	bs->shadow_map = dfbo_new(512,512);
#else
	bs->shadow_map = dfbo_new(1024,1024);
#endif

	bs->cursor = 0;
	return bs;
}



void bullets_free(bullets* bs){ if(!bs) return;
	dfbo_free(bs->shadow_map);
	dvao_free(bs->vao);
	free(bs->b);
	free(bs);

}




void bullets_update(bullets* bs, float dt){ if(!bs) return;

	for (uint i = 0; i < bs->len; i++){
		if( (bs->b[i].lifetime < (float)bs->b[i].frame) && (bs->b[i].lifetime != 0)){
			bullet_clear(bs->b + i);

		} else {
			if(bs->b[i].run != NULL){
				assert(bs == bs->b[i].buf);
				bs->b[i].frame++;
				bs->b[i].run( bs->b + i );
			}

			bs->b[i].acc += bs->b[i].force * dt;
			bs->b[i].acc[0] = clamp(bs->b[i].acc[0], -bs->b[i].tacc, bs->b[i].tacc);
			bs->b[i].acc[1] = clamp(bs->b[i].acc[1], -bs->b[i].tacc, bs->b[i].tacc);
			bs->b[i].acc[2] = clamp(bs->b[i].acc[2], -bs->b[i].tacc, bs->b[i].tacc);

			bs->b[i].vel += bs->b[i].acc * dt;
			bs->b[i].vel[0] = clamp(bs->b[i].vel[0], -bs->b[i].tvel, bs->b[i].tvel);
			bs->b[i].vel[1] = clamp(bs->b[i].vel[1], -bs->b[i].tvel, bs->b[i].tvel);
			bs->b[i].vel[2] = clamp(bs->b[i].vel[2], -bs->b[i].tvel, bs->b[i].tvel);

			bs->b[i].pos += bs->b[i].vel * dt;

	// quick bounce collision response
	// if(bs->b[i].pos[1] < -2 ) {bs->b[i].vel[1]*=-1;bs->b[i].pos[1]=-2;  /*i->acc*=0;*/ }
	// v4f pos = bs->b[i].pos;
	// if(bs->b[i].pos[1] < hfunc(pos[0], pos[2]) ) {bs->b[i].rad*=0;}

		}

	}

// prepare vertex buffers
	uint j = 0;
	for (uint i = 0; i < bs->len * 4; i++){
		bullet_pctr[i] = bs->b[j].pos; bullet_pctr[i][3] = bs->b[j].rad; bullet_pcol0[i] = bs->b[j].col0; bullet_pcol1[i] = bs->b[j].col1; i++;
		bullet_pctr[i] = bs->b[j].pos; bullet_pctr[i][3] = bs->b[j].rad; bullet_pcol0[i] = bs->b[j].col0; bullet_pcol1[i] = bs->b[j].col1; i++;
		bullet_pctr[i] = bs->b[j].pos; bullet_pctr[i][3] = bs->b[j].rad; bullet_pcol0[i] = bs->b[j].col0; bullet_pcol1[i] = bs->b[j].col1; i++;
		bullet_pctr[i] = bs->b[j].pos; bullet_pctr[i][3] = bs->b[j].rad; bullet_pcol0[i] = bs->b[j].col0; bullet_pcol1[i] = bs->b[j].col1;
		j++;
	}

	// bullet_upload
	dvao_setr(bs->vao, 1, 0, bullet_pctr,  bs->len * 4);
	dvao_setr(bs->vao, 2, 0, bullet_pcol0, bs->len * 4);
	dvao_setr(bs->vao, 3, 0, bullet_pcol1, bs->len * 4);
	// dvao_set(bs->vao, 2, 0, bullet_pctr,  bs->len * 4);
	// dvao_set(bs->vao, 3, 0, bullet_pcol0, bs->len * 4);
	// dvao_set(bs->vao, 4, 0, bullet_pcol1, bs->len * 4);
}



#include "mgame.h"
void bullets_draw(bullets* bs){ if(!bs) return;

	dcullf(0);
	ddepth(1,0);
	// ddepth(0,0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	dblend(1);
	// dblendfunc(1);

	dcam_update();
	dshd_unif(bullet_shd, "view", &De.cam.view);
	// dshd_unif(bullet_shd, "mvp", &game.mvp);
	dshd_unif(bullet_shd, "mvp", &De.cam.vp);

	// dshd_unif(bullet_shd, "eye", &De.cam.pos);
	// dshd_unif(bullet_shd, "ppos", &game.player.pos);

	dshd_unif(bullet_shd, "tex", &zero);
	dtex_bind(bullet_tex, 0);

	dshd_update(bullet_shd);
	dvao_update(bs->vao);

	dshd_bind(bullet_shd);
	dvao_bind(bs->vao);
	dibo_draw(bullet_ibo, 0, (bs->vao->len/4)*6, GL_TRIANGLE_STRIP);


// update shadow map
	dfbo_bind(bs->shadow_map);
	dclear(0);

	dcam_update();
	dshd_update(bullet_shd);

// v4f hfocus = game.hmap->focus;
// hfocus[1] = 64;
// m4f m = m4f_model(hfocus, (v4f){PI/2, 0, 0}, (v4f){1, 0, 1,1});
// m = m4f_id();
// m = m4f_view(hfocus, (v4f){PI/2, 0, 0});
// m = m4f_ortho_project( (v2f){32,32}, 1, 5000 );
m4f m = m4f_view(v4f_0, (v4f){PI/2, 0, 0});
// m = m4f_view((v4f){0,64,0,0}, (v4f){-PI/2, 0, 0});

// m4f m = m4f_model(-game.hmap->focus, (v4f){PI/2, 0, 0}, (v4f){1, 1, 1,0});
	// if(game.hmap->mode & HMAP_FIXED){
		dshd_unif(bullet_shadow_shd, "hmap_focus", &game.hmap->ifocus);
	// } else {
		// dshd_unif(bullet_shadow_shd, "hmap_focus", &game.hmap->focus);
	// }

	dshd_unif(bullet_shadow_shd, "m", &m);

	dshd_unif(bullet_shadow_shd, "tex", &zero);
	// dtex_bind(bullet_tex, 0);

	dshd_bind(bullet_shadow_shd);
	dshd_update(bullet_shadow_shd);
	// dvao_bind(bs->vao);
	dibo_draw(bullet_ibo, 0, (bs->vao->len/4)*6, GL_TRIANGLE_STRIP);

	dfbo_bind(0);


dblend(0);
}





bullet* bullets_add(bullets* bs, bullet* b){ if(!bs) return &bullet_0;
	if(!b) {DE_LOG("bullet is nil"); b = &bullet_0;}

	bs->b[ bs->cursor ] = *b;
	bs->b[ bs->cursor ].buf = bs;
	// bs->b[ bs->cursor ].frame = -1;

	// bullet* ret = bs->b + bs->cursor;
	bullet* ret = &(bs->b[ bs->cursor ]);

	bs->cursor++;
	bs->cursor %= bs->len;
	return ret;
}


void bullet_clear( bullet* b ){if (!b) return;
	// if(!b->buf) return;
	*b = bullet_0;

}


























// shader sources

char* bullet_vshdsrc = DE_SHD_HEADERV QUOTE(
	attribute vec2 pos;
	attribute vec2 tc;
	attribute vec4 pctr;
	attribute vec4 col0;
	attribute vec4 col1;

	uniform mat4 mvp;
	uniform mat4 view;

	// uniform vec4 eye;
	// uniform vec4 ppos;

	varying vec2 vtc;
	varying vec2 vpos;
	varying vec4 vcol0;
	varying vec4 vcol1;

	void main(void){
		vec3 cam_right = vec3(view[0][0],view[1][0],view[2][0]);
		vec3 cam_up =    vec3(view[0][1],view[1][1],view[2][1]);
		vec4 dout;
		vtc = tc;
		vpos = pos;
		vcol0 = col0/255.0;
		vcol1 = col1/255.0;
		float scale = pctr.a;

		// closer to camera effect
		// if(length(eye.xz-pctr.xz) < length(eye.xz-ppos.xz) ){
		// 	vcol0*=0.0;
			// vcol1*=0.8;
			// vcol1.a=1.0;
			// scale/=length(pctr.xyz-ppos.xyz)+1.0;
		// }
		vec4 ipos = vec4(pctr.xyz, 1.0);
		vec4 fpos = ipos;

		// fpos = vec4(fpos.xyz +
		// 	(cam_right * (pos.x/2.0))*scale +
		// 	(cam_up * (pos.y/2.0))*scale,
		// 	1.0);

		fpos = vec4(fpos.xyz +
			(cam_right * (pos.x))*scale +
			(cam_up * (pos.y))*scale,
			1.0);

		dout = mvp * fpos;
		gl_Position = dout;
	}
);


char* bullet_fshdsrc = DE_SHD_HEADERF QUOTE(
	varying vec2 vtc;
	varying vec2 vpos;
	varying vec4 vcol0;
	varying vec4 vcol1;
	uniform sampler2D tex;
	void main(void){
		float cs = 0.5;
		vec4 tcol0 = texture2D(tex, vtc);
		// vec2 htc = vtc*2.0-.5;
		vec2 htc = vtc;
		htc.x = clamp(htc.x, 0.0, 1.0);
		htc.y = clamp(htc.y, 0.0, 1.0);

		vec4 tcol1 = texture2D(tex, htc );

		if(tcol1.a == 0.0) discard;
		gl_FragColor = tcol1*vcol0 + vcol1*(1.0-dot(vpos,vpos));
		// gl_FragColor = pow(tcol0 * vcol0 + tcol1*(1.0-mod(dot(vpos,vpos)*5.0, 1.0)) * vcol1, vec4(.75,.75,.75,1.0));
	}
);











// shadow


char* bullet_shadow_shd_vsrc = DE_SHD_HEADERV QUOTE(
	attribute vec2 pos;
	attribute vec2 tc;
	attribute vec4 pctr;
	attribute vec4 col0;
	attribute vec4 col1;

	uniform mat4 m;
	uniform vec4 hmap_focus;

	varying vec2 vtc;
	varying vec2 vpos;
	varying vec4 vcol0;
	varying vec4 vcol1;

	void main(void){
		vtc = tc;
		vpos = pos;
	float hres = hmap_focus.w/2.0;
		vcol0 = 1.0-col0/255.0 + .25; //vcol0 -= max(pctr.y - hres/4.0, 0.0);// vcol0 /= max(pow(pctr.y-(hres/4.0),3.0), 1.0);
		vcol1 = 1.0-col1/255.0 + .25; //vcol1 -= max(pctr.y - hres/4.0, 0.0);// vcol1 /= max(pow(pctr.y-(hres/4.0),3.0), 1.0);

		float dist = abs(pctr.y - hmap_focus.y);
		float fogfactor = (hres - dist)/(hres - hres/4.0); // transposed focus
		fogfactor = clamp( fogfactor, 0.0, 1.0);
		vcol0 = mix(vec4(.0, .0, .0, 0.0), vcol0, fogfactor); // fog
		vcol1 = mix(vec4(.0, .0, .0, 0.0), vcol1, fogfactor); // fog

		// vec3 cam_right = vec3(1.0, 0.0, 0.0);
		// vec3 cam_up =    vec3(0.0, 1.0, 0.0);

		vec3 cam_right = vec3(m[0][0],m[1][0],m[2][0]);
		vec3 cam_up =    vec3(m[0][1],m[1][1],m[2][1]);
		// vec3 ipos = (vec3(pctr.x, 0.0, pctr.z)-hmap_focus.xyz) /32.0;
		vec3 ipos = (vec3(pctr.x, pctr.y, pctr.z)-hmap_focus.xyz) /hres;
		// vec4 fpos = vec4(pctr.xyz/64.0 +
		vec4 fpos = vec4(ipos +
			(cam_right * (pos.x))*pctr.w/hres +
			(cam_up *    (pos.y))*pctr.w/hres
		,1.0);

		gl_Position = m*fpos;
		// gl_Position = fpos;
	}
);










char* bullet_shadow_shd_fsrc = DE_SHD_HEADERF QUOTE(
	varying vec2 vtc;
	varying vec2 vpos;
	varying vec4 vcol0;
	varying vec4 vcol1;
	uniform sampler2D tex;
	void main(void){
		vec4 tcol0 = texture2D(tex, vtc);
		// vec2 htc = vtc*2.0-.5;
		vec2 htc = vtc;
		htc.x = clamp(htc.x, 0.0, 1.0);
		htc.y = clamp(htc.y, 0.0, 1.0);

		vec4 tcol1 = texture2D(tex, htc );

		if(tcol1.a == 0.0) discard;
		gl_FragColor = tcol1*vcol0 + vcol1*(1.0-dot(vpos,vpos));

		// gl_FragColor = pow(tcol0 * vcol0 + tcol1*(1.0-mod(dot(vpos,vpos)*5.0, 1.0)) * vcol1, vec4(.75,.75,.75,1.0));
		// gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
	}
);



