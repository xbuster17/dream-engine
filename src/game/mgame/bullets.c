#include "bullets.h"

void bullet_set_pos(bullet, v4f pos);
void bullet_set_col(bullet, v4f col);

// void bullets_init(int count); //you should not call bullets new with an int higher than this

char* bullet_vshdsrc;
char* bullet_fshdsrc;

dshd* bullet_shd;
dibo* bullet_ibo;

v4f* _bullet_ppos;
v4f* _bullet_pctr;
v4c* _bullet_pcol0;
v4c* _bullet_pcol1;
SDL_mutex* _bullet_buffer_mutex;

v4c* bullet_tex_pixels;
int bullet_tex_size = 32;
int bullet_tex_size2 = 32*32;
#ifdef ANDROID
int shadow_map_size = 1024;
#else
int shadow_map_size = 1024*2;
#endif
dtex* bullet_tex;
static int zero = 0;

m4f shadow_cam_view;

dshd* bullet_shadow_shd = NULL;
char* bullet_shadow_shd_vsrc;
char* bullet_shadow_shd_fsrc;
v4f Hmap_shadow_focus;



#include "mgame.h" // G. struct
void bullets_init(uint count){
	//these are initialization buffers to avoid mallocs on _new()
	_bullet_ppos = malloc(sizeof(v4f)*count*4);
	_bullet_pctr = malloc(sizeof(v4f)*count*4);
	_bullet_pcol0 = malloc(sizeof(v4c)*count*4);
	_bullet_pcol1 = malloc(sizeof(v4c)*count*4);

//init bullet vertices
	// v4c col0 = {255,255,255,255};
	// v4c col1 = col0;
	for (uint i = 0; i < count*4; i++){
		// m4f rot = m4f_rotation((v4f){0,0,PI/14.0*i,0});
		// m4f rot = m4f_rotation((v4f){0,0,PI/4,0});
		// m4f rot = m4f_rotation((v4f){0,0,0,0});
		_bullet_ppos[i] = (v4f){-1,-1, 0, 0};i++;
		_bullet_ppos[i] = (v4f){ 1,-1, 1, 0};i++;
		_bullet_ppos[i] = (v4f){-1, 1, 0, 1};i++;
		_bullet_ppos[i] = (v4f){ 1, 1, 1, 1};
		// _bullet_ppos[i] = (v4f){-1,-1, 0, 0}; i++;
		// _bullet_ppos[i] = (v4f){ 1,-1, 1, 0}; i++;
		// _bullet_ppos[i] = (v4f){-1, 1, 0, 1}; i++;
		// _bullet_ppos[i] = (v4f){ 1, 1, 1, 1};
	}


//make bullet texture
	bullet_tex_pixels = malloc(sizeof(v4c) * bullet_tex_size * bullet_tex_size);
	for (int i = 0; i < bullet_tex_size; i++){
		for (int j = 0; j < bullet_tex_size; j++){

			bullet_tex_pixels[i * bullet_tex_size + j] = v4c_0;
			float dist = v2f_len2((v2f){i,j} - (v2f){bullet_tex_size/2, bullet_tex_size/2});
			if(dist < bullet_tex_size2 / 4){
			// if(dist < bullet_tex_size2 / 5){
				// if(dist > bullet_tex_size2 / 8) // empty circles
				bullet_tex_pixels[i * bullet_tex_size + j] = (v4c){255,255,255,255};
			}
		}
	}

	bullet_tex = dtex_new(bullet_tex_pixels, bullet_tex_size, bullet_tex_size);
	free(bullet_tex_pixels);

	dtex_filter(bullet_tex, GL_NEAREST, GL_NEAREST);
	// dtex_filter(bullet_tex, GL_NEAREST, GL_LINEAR);



	//make bullet shader
	bullet_shd = dshd_new( bullet_vshdsrc, bullet_fshdsrc);
	dshd_unif(bullet_shd, "view", &De.cam.view);
	dshd_unif(bullet_shd, "mvp", &De.cam.vp);
	dshd_unif(bullet_shd, "tex", &zero);
	// dtex_bind(bullet_tex, 0);
	dshd_unif(bullet_shd, "eye", &De.cam.pos);
	dshd_unif(bullet_shd, "lpos", &G.player.pos);


	//make bullet ibo
//strip
//	short* ibo = malloc(sizeof(short) * count * 6);
//	int in = 0;
//	for(uint i = 0; i<count * 6 -1; i++){
//		if( i ){
//			ibo[i] = ++in; i++;
//		}/*degenerate triangle 2*/
//		ibo[i] = in;
//		in++; i++;
//		ibo[i] = in;
//		in++; i++;
//		ibo[i] = in;
//		in++; i++;
//		ibo[i] = in;
//		i++;
//		ibo[i] = in;/*degenerate triangle 1*/
//	}
//	bullet_ibo = dibo_new(ibo, count*6, GL_TRIANGLE_STRIP);

//tris
	short* ibo = malloc(sizeof(short) * count * 6);
	int in = 0;
	for(uint i = 0; i<count * 6 -1; i++){
		ibo[i] = in; // 1
		in++; i++;
		ibo[i] = in; // 2
		in++; i++;
		ibo[i] = in; // 3
		i++;
		ibo[i] = in; // 3
		in--; i++;
		ibo[i] = in; // 2
		in+=2; i++;
		ibo[i] = in; // 4
		in++;
	}

	bullet_ibo = dibo_new(ibo, count*6, GL_TRIANGLES);
	free(ibo);


//shadow
	shadow_cam_view = m4f_view(v4f_0, (v4f){PI/2, 0, 0});
	bullet_shadow_shd = dshd_new(bullet_shadow_shd_vsrc, bullet_shadow_shd_fsrc);

	dshd_unif(bullet_shadow_shd, "m", &shadow_cam_view);
	dshd_unif(bullet_shadow_shd, "tex", &zero);
	dshd_unif(bullet_shadow_shd, "hmap_focus", &Hmap_shadow_focus);

	_bullet_buffer_mutex = SDL_CreateMutex();
}





void bullets_quit(void){
	free(_bullet_ppos);
	free(_bullet_pctr);
	free(_bullet_pcol0);
	free(_bullet_pcol1);
	dtex_free(bullet_tex);
	dshd_free(bullet_shd);
	dibo_free(bullet_ibo);
	// SDL_UnlockMutex(_bullet_buffer_mutex);
	SDL_DestroyMutex(_bullet_buffer_mutex);
}





bullets* bullets_new(uint count, bool shadow, int thread){
	bullets* bs = malloc(sizeof(bullets));
	bs->b = malloc(sizeof(bullet) * count);
	bs->len = count;

	for (uint i = 0; i < count; i++){
		bs->b[i] = bullet_0;
	}

	void* verts[] = {_bullet_ppos, _bullet_pctr, _bullet_pcol0, _bullet_pcol1};
	// bs->vao = dvao_newr("f2p, f2tc; f4ctr; uc4col0; uc4col1", verts, count*4, GL_TRIANGLE_STRIP);
	bs->vao = dvao_newr("f2p, f2tc; f4ctr; uc4col0; uc4col1", verts, count*4, GL_TRIANGLES);

	// bs->vao->vbos[0]->usage = GL_DYNAMIC_DRAW;
	bs->vao->vbos[1]->usage = GL_STREAM_DRAW;
	bs->vao->vbos[2]->usage = GL_STREAM_DRAW;
	bs->vao->vbos[3]->usage = GL_STREAM_DRAW;


	bs->shadow = shadow;
	if(shadow)
		bs->shadow_map = dfbo_new(shadow_map_size, shadow_map_size);
	else bs->shadow_map = NULL;

	bs->cursor = 0;


	if(thread){
		bs->tshould_finish = false;
		bs->tlogic_done = true;
		bs->bmutex = SDL_CreateMutex();
		if(!bs->bmutex) DE_LOG("couldnt create mutex: %s", SDL_GetError());

		bs->thread = SDL_CreateThread(bullets_thread_fn, NULL, bs);
		if(!bs->thread) DE_LOG("%s", SDL_GetError());
		else {
			bs->threadID = SDL_GetThreadID(bs->thread);
			DE_LOG("tID %lu", bs->threadID);
		}
	} else bs->thread = NULL;

	return bs;
}





void bullets_free(bullets* bs){ if(!bs) return;
	if(bs->thread) { int ret;
		bs->tshould_finish = true;
		SDL_WaitThread(bs->thread, &ret);
		// SDL_UnlockMutex(bs->bmutex);
		SDL_DestroyMutex(bs->bmutex);
		// SDL_DetachThread(bs->thread);
		// bs->thread = NULL;
	}
	if(bs->shadow) dfbo_free(bs->shadow_map);
	// if(bs->shadow_map) dfbo_free(bs->shadow_map);
	dvao_free(bs->vao);

	free(bs->b);
	free(bs);

}





void bullets_update_logic(bullets* bs, float dt){
	for (uint i = 0; i < bs->len; i++){
		if( (bs->b[i].lifetime < (float)bs->b[i].frame) && (bs->b[i].lifetime != 0)){
			bullet_clear(bs->b + i);
		} else {

			bs->b[i].acc += bs->b[i].force * dt;
			bs->b[i].acc[0] = clamp(bs->b[i].acc[0], -bs->b[i].tacc, bs->b[i].tacc);
			bs->b[i].acc[1] = clamp(bs->b[i].acc[1], -bs->b[i].tacc, bs->b[i].tacc);
			bs->b[i].acc[2] = clamp(bs->b[i].acc[2], -bs->b[i].tacc, bs->b[i].tacc);

			bs->b[i].vel += bs->b[i].acc * dt;
			bs->b[i].vel[0] = clamp(bs->b[i].vel[0], -bs->b[i].tvel, bs->b[i].tvel);
			bs->b[i].vel[1] = clamp(bs->b[i].vel[1], -bs->b[i].tvel, bs->b[i].tvel);
			bs->b[i].vel[2] = clamp(bs->b[i].vel[2], -bs->b[i].tvel, bs->b[i].tvel);

			bs->b[i].pos += bs->b[i].vel * dt;

			if(bs->b[i].run != NULL){
				// assert(bs == bs->b[i].buf);
				bs->b[i].frame++;
				bs->b[i].run( bs->b + i );
			}

		}
	}

	// SDL_TryLockMutex(_bullet_buffer_mutex);
	SDL_LockMutex(_bullet_buffer_mutex);
// prepare buffers on main ram
	uint j = 0;
	for (uint i = 0; i < bs->len * 4; i++){
		_bullet_pctr[i] = bs->b[j].pos; _bullet_pctr[i][3] = bs->b[j].rad; _bullet_pcol0[i] = bs->b[j].col0; _bullet_pcol1[i] = bs->b[j].col1; i++;
		_bullet_pctr[i] = bs->b[j].pos; _bullet_pctr[i][3] = bs->b[j].rad; _bullet_pcol0[i] = bs->b[j].col0; _bullet_pcol1[i] = bs->b[j].col1; i++;
		_bullet_pctr[i] = bs->b[j].pos; _bullet_pctr[i][3] = bs->b[j].rad; _bullet_pcol0[i] = bs->b[j].col0; _bullet_pcol1[i] = bs->b[j].col1; i++;
		_bullet_pctr[i] = bs->b[j].pos; _bullet_pctr[i][3] = bs->b[j].rad; _bullet_pcol0[i] = bs->b[j].col0; _bullet_pcol1[i] = bs->b[j].col1;
		j++;
	}

	// bullet_upload
	dvao_setr(bs->vao, 1, _bullet_pctr, 0, bs->len * 4);
	dvao_setr(bs->vao, 2, _bullet_pcol0, 0, bs->len * 4);
	dvao_setr(bs->vao, 3, _bullet_pcol1, 0, bs->len * 4);
	// dvao_set(bs->vao, 2, _bullet_pctr, 0, bs->len * 4);
	// dvao_set(bs->vao, 3, _bullet_pcol0, 0, bs->len * 4);
	// dvao_set(bs->vao, 4, _bullet_pcol1, 0, bs->len * 4);

	SDL_UnlockMutex( _bullet_buffer_mutex );
}


void bullets_update(bullets* bs, float dt){ if(!bs) return;
	bs->dt = dt;
	if(!bs->thread) {
		bullets_update_logic(bs, dt);
		bullets_upload(bs);
	} else {
		// skip
		// if(bs->tlogic_done) bullets_upload(bs);
		// bs->tlogic_done = false;
		//wait
		SDL_LockMutex(bs->bmutex);
		// while (!bs->tlogic_done) ddelay(1);
		bullets_upload(bs);
		bs->tlogic_done = false;
		SDL_UnlockMutex(bs->bmutex);
	}
}





void bullets_upload(bullets* bs){if (!bs) return;
	dvao_update(bs->vao);
}




void bullets_draw(bullets* bs){ if(!bs) return;
	dtex_bind(bullet_tex, 0);

	if(bs->shadow){
	// update shadow map
		dfbo_bind(bs->shadow_map);
		dclear_color((v4f){0,0,0,0});
		dclear(0);
		// dcam_update();
	// dblend(0);

	// v4f hfocus = game.hmap->focus;
	// hfocus[1] = 64;
	// m4f m = m4f_model(hfocus, (v4f){PI/2, 0, 0}, (v4f){1, 0, 1,1});
	// m = m4f_id();
	// m = m4f_view(hfocus, (v4f){PI/2, 0, 0});
	// m = m4f_ortho_project( (v2f){32,32}, 1, 5000 );
	// m4f m = m4f_view(v4f_0, (v4f){PI/2, 0, 0});
	// m = m4f_view((v4f){0,64,0,0}, (v4f){-PI/2, 0, 0});
		dshd_bind(bullet_shadow_shd);

	// m4f m = m4f_model(-game.hmap->focus, (v4f){PI/2, 0, 0}, (v4f){1, 1, 1,0});
		// if(game.hmap->mode & HMAP_FIXED){
			// dshd_unif(bullet_shadow_shd, "hmap_focus", &G.hmap->ifocus);
		// dshd_unif(bullet_shadow_shd, "hmap_focus", &G.hmap->focus);
		// } else {
			// dshd_unif(bullet_shadow_shd, "hmap_focus", &game.hmap->focus);
		// }
Hmap_shadow_focus = G.hmap->focus;
		// dshd_unif(bullet_shadow_shd, "m", &m);
		// dshd_unif(bullet_shadow_shd, "tex", &zero);
		// dtex_bind(bullet_tex, 0);

		dshd_update(bullet_shadow_shd);
		dvao_bind(bs->vao);
		// dibo_draw(bullet_ibo, 0, (bs->vao->len/4)*6, GL_TRIANGLE_STRIP);
		dibo_draw(bullet_ibo, 0, (bs->vao->len/4)*6, GL_TRIANGLES);

		dfbo_bind(0);

	}

	dcullf(0);
	ddepth(1,0);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	dblend(1);

	dcam_update();
	// dshd_unif(bullet_shd, "view", &De.cam.view);
	// dshd_unif(bullet_shd, "mvp", &De.cam.vp);
	// dshd_unif(bullet_shd, "tex", &zero);
	// // dtex_bind(bullet_tex, 0);
	// dshd_unif(bullet_shd, "eye", &De.cam.pos);
	// dshd_unif(bullet_shd, "lpos", &G.player.pos);

	dshd_update(bullet_shd);
	// dvao_update(bs->vao);

	dshd_bind(bullet_shd);
	dvao_bind(bs->vao);
	// dibo_draw(bullet_ibo, 0, (bs->vao->len/4)*6, GL_TRIANGLE_STRIP);
	dibo_draw(bullet_ibo, 0, (bs->vao->len/4)*6, GL_TRIANGLES);


	dblend(0);
}





bullet* bullets_add(bullets* bs, bullet* b){
	if(!bs){ DE_LOG("bullet container is null"); return &bullet_0; }
	if(!b){ b = &bullet_0; }
	// bool need_mutex = bs->thread && (SDL_ThreadID() != bs->threadID);
	// bool need_mutex = !!bs->thread;

	if( bs->thread ){
		int status = SDL_LockMutex(bs->bmutex);
		if(status != 0){
			if (status == SDL_MUTEX_TIMEDOUT) DE_LOG("from thread %lu: mutex timedout %p",SDL_ThreadID(), bs->bmutex);
			else DE_LOG("from thread %lu: mutex lock error: %s",SDL_ThreadID(), SDL_GetError());
			return bs->b + bs->cursor;
		}
	}

	bs->b[ bs->cursor ] = *b;
	bs->b[ bs->cursor ].buf = bs;
	// bs->b[ bs->cursor ].frame = 0;

	bullet* ret = bs->b + bs->cursor;

	bs->cursor = (bs->cursor+1) % bs->len;
	// bs->cursor++;
	// bs->cursor %= bs->len;

	if( bs->thread ) SDL_UnlockMutex(bs->bmutex);
	return ret;
}





int bullets_thread_fn(void* data){
	if (!data) return 0;
	bullets* bs = (bullets*)data;

	while(!bs->tshould_finish){
		if(!bs->tlogic_done){
			int error;
			error = SDL_LockMutex(bs->bmutex);
			// if(error) goto _delay;
			if(error) DE_LOG("error locking mutex");
			bullets_update_logic(bs, bs->dt);
			bs->tlogic_done = true;

			SDL_UnlockMutex(bs->bmutex);
		}
		// _delay:
		ddelay(1);
	}

	return 0;
}





// todo ?
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
	// uniform vec4 lpos;

	varying vec2 vtc;
	varying vec2 vpos;
	varying vec4 vcol0;
	varying vec4 vcol1;
	varying vec4 vpctr;
	varying vec4 vbpos;
	varying vec4 vfpos;
	varying vec4 vnor_view;
	// varying float vlight_reached;


	void main(void){
		vtc = tc;
		vpos = pos;
		vpctr = pctr;
		vcol0 = col0/255.0;
		vcol1 = col1/255.0;

		vec3 cam_right = vec3(view[0][0],view[1][0],view[2][0]);
		vec3 cam_up =    vec3(view[0][1],view[1][1],view[2][1]);
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

		fpos = vec4(fpos.xyz +
			cam_right * pos.x * scale +
			cam_up * pos.y * scale,
			1.0);

		vbpos = vec4(
			cam_right * pos.x+
			cam_up * pos.y,
		1.0);

		// vnor_view = (mvp*fpos+1.0)/2.0;
		vfpos = fpos;

		// if(length(pctr.xyz-lpos.xyz) < 2.0)
		// 	vlight_reached = 2.0;
		// else
		// 	vlight_reached = 0.0;

		gl_Position = mvp * fpos;
	}
);




char* bullet_fshdsrc = DE_SHD_HEADERF QUOTE(
	uniform sampler2D tex;
	varying vec2 vtc;
	varying vec2 vpos;
	varying vec4 vcol0;
	varying vec4 vcol1;
	varying vec4 vpctr;
	varying vec4 vbpos;
	varying vec4 vfpos;
	varying vec4 vnor_view;
	// varying float vlight_reached;

	uniform vec4 eye;
	uniform vec4 lpos;

	mat4 M = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	mat4 N = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	vec4 lcol = vec4(1.0, 1.0, 1.25, 1.0);
	// vec4 lcol = vec4(1.0, 1.0, 0.0, 1.0);
	float lpow = 2.0;

	vec4 light(
		vec4 eye, vec4 vpos, vec4 vnor, mat4 M, mat4 N,
		vec4 lpos, vec4 lcol, float lpow){
	\t    vec4 inor = normalize(N * vnor);              \n
	\t    vec4 ldirv = lpos - vpos;                     \n
	\t    vec4 ldir = normalize(ldirv);                 \n
	\t    float dist2 = dot(ldirv,ldirv);               \n
	\t    float distp = pow(dist2, 3.0);                \n
	\t    float cosTheta = max(dot(ldir, inor), 0.0);   \n
	\t    return lcol * lpow * cosTheta / distp; \n
	}


	void main(void){
	vec4 ap = vfpos;
	// vec4 ap = vec4(vpos.xy,0.0,1.0);
	// mat4 M = mat4(1.0, 0.0, 0.0, vpctr.x, 0.0, 1.0, 0.0, vpctr.y, 0.0, 0.0, 1.0, vpctr.z, 0.0, 0.0, 0.0, 1.0);
	// mat4 M = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, vpctr.x,vpctr.y,vpctr.z, 1.0);
	// vec4 an = normalize(eye)-(vbpos-vpctr);
	// vec4 an = normalize(normalize(eye)+(vbpos));
	// vec4 an = normalize(vec4(vpos.xy,-1.0,0.0) / vpctr.a);
	// vec4 an = (vbpos + normalize(eye-vpctr)) ;
	vec4 an = normalize(vbpos) + vec4(normalize(eye-vpctr).xyz, 0.0);
	// an = pow(an, vec4(4.0,4.0,4.0,1.0));
	// vec4 an = reflect(vec4(-normalize(eye-vpctr).xyz, 0.0), vbpos + vec4(normalize(eye-vpctr).xyz, 0.0));

	// vec4 an = normalize(vbpos + vec4(0.0,0.0,-1.0,0.0) );
	an.w = 0.0;
	// vec4 flpos = lpos + (eye-lpos)/100.0;
	vec4 flpos = lpos;
	// flpos.y += 0.02;
	// vec4 an = vec4( vpctr.xy, sqrt(1.0-dot(vpctr.xy,vpctr.xy)), 0.0 );
	// vec4 an = vnor_view;

		float cs = 0.5;
		vec4 tcol0 = texture2D(tex, vtc);
		// vec2 htc = vtc*2.0-.5;
		vec2 htc = vtc;
		htc.x = clamp(htc.x, 0.0, 1.0);
		htc.y = clamp(htc.y, 0.0, 1.0);

		vec4 tcol1 = texture2D(tex, htc );

		if(tcol1.a == 0.0) discard;
		// vec4 light_component = vec4(0.0, 0.0, 0.0, 1.0);
		// if(vlight_reached > 1.0)
		// vec4 light_component = light(eye, ap, an, M,N, flpos, lcol, lpow)*2.0+0.5;
		vec4 light_component = light(eye, ap, an, M,N, flpos, lcol, lpow) + 0.4;
		// light_component = pow(light_component, vec4(4.5, 4.5, 4.5, 1.0));
		// light_component = pow(light_component, vec4(3.5, 3.5, 3.5, 1.0));

		light_component.a = 1.0;
		// light_component = pow(light_component, vec4(.67,.67,.67,1.0));

		gl_FragColor = tcol1*vcol0 + 0.5 + vcol1*(1.0-dot(vpos,vpos));
		gl_FragColor *= light_component;//*mod(length(flpos.xyz-vpos.xyz), 2.0);
		// gl_FragColor = pow(gl_FragColor, vec4(.67, .67, .67, 1.0));
		gl_FragColor = pow(gl_FragColor, vec4(2.5, 2.5, 2.5, 1.0));
		// gl_FragColor += tcol1*vcol0 + vcol1*(1.0-dot(vpos,vpos));
		// gl_FragColor = tcol1*vcol0 + vcol1*(1.0-dot(vpos,vpos)) + light_component;

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

	// float darkness = .25;
	// float fogmax =
	void main(void){
		vtc = tc;
		vpos = pos;
		vcol0 = 1.0-col0/255.0 + .25;
		vcol1 = 1.0-col1/255.0 + .25;

		// float hres = hmap_focus.w/2.0;
		// float hres = hmap_focus.w/4.0;// half hmap shadow for higher res
		float hres = hmap_focus.w/8.0;// quarter hmap shadow for higher res

		// shadow blend based on height
		float dist = abs(pctr.y - hmap_focus.y);
		float fogfactor = (hres - dist)/(hres - hres/32.0);
		fogfactor = clamp( fogfactor, 0.0, 1.0);
		vcol0 = mix(vec4(.0, .0, .0, 0.0), vcol0, fogfactor);
		vcol1 = mix(vec4(.0, .0, .0, 0.0), vcol1, fogfactor);

		float scale = pctr.w/hres;
		scale = mix(0.0, scale, fogfactor);

		//billboard
		vec3 cam_right = vec3(m[0][0],m[1][0],m[2][0]);
		vec3 cam_up =    vec3(m[0][1],m[1][1],m[2][1]);
		// vec3 ipos = (vec3(pctr.x, pctr.y, pctr.z)-hmap_focus.xyz) /hres;
		vec3 ipos = (vec3(pctr.x, floor(hmap_focus.y), pctr.z)-floor(hmap_focus.xyz)) /hres;
		vec4 fpos = vec4(ipos +
			(cam_right * (pos.x))*scale/*/hres*/ +
			(cam_up *    (pos.y))*scale/*/hres*/
		,1.0);

		gl_Position = m*fpos;
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
		gl_FragColor.a = 0.0;
		// gl_FragColor = pow(tcol0 * vcol0 + tcol1*(1.0-mod(dot(vpos,vpos)*5.0, 1.0)) * vcol1, vec4(.75,.75,.75,1.0));
		// gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
	}
);



