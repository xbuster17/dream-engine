#include "hmap.h"
#include "mgame.h" //game.player.pos

// void hmap_mode(hmap* hm, bool dynamic, bool follow, bool fixed){ if (!hm) return;
	// hm->dynamic = dynamic; // generate new vertices
	// hm->follow = follow; // center vertices at focus
	// hm->fixed = fixed; // generate vertices based on grid and not focus
// }

bool hmap_inited = false;
dshd* hmap_shd = NULL;
dtex* hmap_tex = NULL;
dtex* hmap_inside_tex = NULL;
v2f Screen_size;
m4f _hmap_MVP;
v4f* hmap_vbuf;
v4f* hmap_nbuf;
v4c* hmap_texpx;
char* hmap_vshdsrc;
char* hmap_fshdsrc;

dshd* hmap_shadow_map_shd = NULL;
char* shadow_map_vsrc;
char* shadow_map_fsrc;
m4f depth_mvp;
// int tres = 16;
int tres = 64;
int zero = 0;
int tunit_one = 1;
int tunit_two = 2;
int hmap_max_res = 512;
float _spike = .3;



hmap* hmap_new(int xres, int yres, float(*func)(float x, float y), enum hmap_mode mode){
	if(!hmap_inited){
		hmap_inited = true;
		// hmap_vbuf = malloc(sizeof(v4f) * hmap_max_res*hmap_max_res * 6);
		hmap_vbuf = malloc(sizeof(v4f) * hmap_max_res*hmap_max_res * 12);
		// hmap_nbuf = malloc(sizeof(v4f) * hmap_max_res*hmap_max_res * 6);
		hmap_nbuf = malloc(sizeof(v4f) * hmap_max_res*hmap_max_res * 12);
		hmap_texpx = malloc(sizeof(v4c) * tres*tres);
v4f fcol;
//texture
		for (int i = 0; i < tres; ++i){
			for (int j = 0; j < tres; ++j){
				// v4c c = {255,255,255,255};
				// hmap_texpx[i*tres + j] = c;
				hmap_texpx[i*tres + j] = v4c_0;
				// hmap_texpx[i*tres + j] = (v4c){.02*255, .08*255, .202*255, 255};
				// srand(i/(tres/di) + (j/(tres/di)) *100 );
				// hmap_texpx[i*tres + j] = v4c_rand()>>4;
				// hmap_texpx[i*tres + j] = colors[rand()%num_colors];
				// if(i < 1 || j < 1 || i > tres-1 || j > tres-1 || abs(i - (tres-j))<1 ) // line
				// if((abs(i)==abs(j)) || (i==-j+tres) || i < 1 || j < 1 || i > tres-1 || j > tres-1 ) //cross
				// if(i < 1 || j < 1 /*|| i >= tres-1 || j >= tres-1 */) 
					// hmap_texpx[i*tres + j] = (v4c){255,255,255,255};
					// hmap_texpx[i*tres + j] = v4c_0;
				// else{
					// hmap_texpx[i*tres + j] = (v4c){255,255,255,255};
					// hmap_texpx[i*tres + j] = v4c_0;
					// hmap_texpx[i*tres + j] = v4c_rand()/4;
					// hmap_texpx[i*tres + j][0] += 150;
					// hmap_texpx[i*tres + j][1] += 50;
					// hmap_texpx[i*tres + j][2] += 200;
// #define v4_cast(v, target_type) ((target_type){(v)[0], (v)[1], (v)[2], (v)[3]})
// fcol=(v4f){150,200,50}*(fmodf(hash2(i/4,j/4)/123123.f, .75f)+.25f);
// fcol=(v4f){150,200,50}*(fmodf(hash2(i/4,j/4)/123123.f, .75f)+.25f);
fcol=(v4f){200,150,50}*(fmodf(hash2(i/8,j/8)/123123.f, .75f)+.25f);
v4c ccol={fcol[0],fcol[1],fcol[2]};
					hmap_texpx[i*tres + j] = ccol;
					hmap_texpx[i*tres + j][3] = 255;
					// hmap_texpx[i*tres + j] /= 3;
				// }
					// hmap_texpx[i*tres + j] = (v4c){255,255,255,255};
				// if(!(i%(tres/(di))) || !(j%(tres/(di))))
				// if(!(i%(tres/di)) || !(j%(tres/di)) || abs(i%(tres/di) - (tres/di-j%(tres/di)))<1)
				// if(i < 3 || j < 3 || i > tres-3 || j > tres-3 || abs(i - (tres-j))<3 )
				// if(i < 1 || j < 1 || i > tres-1 || j > tres-1 || abs(i - (tres-j))<1 )
				// if(i < 1 || j < 1 || i > tres-1 || j > tres-1)

					// hmap_texpx[i*tres + j] = (v4c){255,255,255,255};
					// hmap_texpx[i*tres + j] = v4c_0;
					// hmap_texpx[i*tres + j] = v4c_rand();
			}
		}

		hmap_tex = dtex_new(hmap_texpx, tres,tres);
		
		dtex_bind(hmap_tex, zero);
		// glGenerateMipmap(GL_TEXTURE_2D);
		// dtex_wrap(hmap_tex, GL_REPEAT, GL_REPEAT);
		// dtex_filter(hmap_tex, GL_NEAREST,GL_NEAREST);
		// dtex_filter(hmap_tex, GL_LINEAR,GL_NEAREST);
		dtex_filter(hmap_tex, GL_LINEAR,GL_LINEAR);
		// dtex_filter(hmap_tex, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR);
		// dtex_filter(hmap_tex, GL_NEAREST_MIPMAP_LINEAR, GL_NEAREST);
		// dtex_filter(hmap_tex, GL_LINEAR_MIPMAP_LINEAR, GL_NEAREST);
		// dtex_filter(hmap_tex, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR);
		// dtex_filter(hmap_tex, GL_LINEAR, GL_LINEAR_MIPMAP_LINEAR);


		for (int i = 0; i < tres; ++i){
			for (int j = 0; j < tres; ++j){
				// hmap_texpx[i*tres + j] = (v4c){200,150,64,128};
				hmap_texpx[i*tres + j][3] = 128+64;
				// if(!((j+i)%4))
				if( (j+i)%2 || j%2 ) 
					hmap_texpx[i*tres + j][3] *= 0;
				// hmap_texpx[i*tres + j][0] = ((j+i)%2) * 255;
				// hmap_texpx[i*tres + j][3] = ((j+i)%2) * 255;
			}
		}
		hmap_inside_tex = dtex_new(hmap_texpx, tres,tres);
		dtex_wrap(hmap_tex, GL_REPEAT, GL_REPEAT);
		dtex_filter(hmap_inside_tex, GL_LINEAR,GL_LINEAR);

	} // if not inited
	hmap* ret = malloc(sizeof(hmap));

	ret->cache = malloc(sizeof(float) * xres*yres);
	memset(ret->cache, 0, sizeof(float) * xres*yres);
	ret->cache_mutex = SDL_CreateMutex();
	ret->f = func;
	ret->x = xres;
	ret->y = yres;
	ret->res = xres;
	// ret->pos = v4f_0;
	// ret->rot = v4f_0;
	// ret->sca = (v4f){1,1,1,0};

	ret->scale = (v4f){2,1,1,1};
	ret->focus = v4f_0;
	ret->ifocus = v4f_0;
	ret->generated = false;
	ret->mode = mode;

	ret->use_shadow = true;
	// ret->shadow_len = 6.0;
	ret->shadow_len = 8.0;
	// ret->shadow_len = G.gconf[G.gquality].hm_shadow_len;
	// ret->sca = (v4f){10,.1,10,1};

	ret->shadow = NULL;


// //vertices call update
	// void* verts[] = {hmap_vbuf, hmap_nbuf};
// ret->vao = dvao_newr("f4p; f4n", verts, xres*yres*6, GL_TRIANGLES);

	ret->vao = dvao_newr("f4p; f4n", NULL, xres*yres*12, GL_TRIANGLES);

	if(mode & HMAP_DYNAMIC){
		ret->vao->vbos[0]->usage = GL_STREAM_DRAW;
		ret->vao->vbos[1]->usage = GL_STREAM_DRAW;
	}
	// ret->vao = dvao_newr("f4p", NULL, xres*yres*6, GL_TRIANGLES);

//shader
	if(!hmap_shd) hmap_shd = dshd_new(hmap_vshdsrc, hmap_fshdsrc);
	ret->shd = hmap_shd;

	// dshd_unif(hmap_shd, "view", &game.view);
	dshd_unif(hmap_shd, "mvp", &De.cam.vp);
	// dshd_unif(hmap_shd, "mvp", &_hmap_MVP);

	dshd_unif(hmap_shd, "eye", &De.cam.pos);
	dshd_unif(hmap_shd, "tex", &zero);
	dshd_unif(hmap_shd, "shadow_map", &tunit_one);
	dshd_unif(hmap_shd, "depth_map", &tunit_two);
	dshd_unif(hmap_shd, "depth_mvp", &depth_mvp);
	// dshd_unif(hmap_shd, "tm", &tunit_two);
	// dshd_unif(hmap_shd, "tmd", &tunit_three); // this one selects the tile
	// dshd_unif(hmap_shd, "tmr", &tilemap_res);

	// dshd_unif(hmap_shd, "fogcolor", &G.clear_color);

	// dshd_unif(hmap_shd, "lpos", &G.enm[0].pos);
	dshd_unif(hmap_shd, "lpos", &G.sunpos);
	dshd_unif(hmap_shd, "sunc", &G.suncol);
	// dshd_utex(hmap_shd, "t", 0);
	if(mode & HMAP_FIXED || !mode)
		dshd_unif(hmap_shd, "focus", &ret->ifocus);
	else
		dshd_unif(hmap_shd, "focus", &ret->focus);

	dshd_unif(hmap_shd, "ppos", &G.player.pos);

	dshd_unif(hmap_shd, "shadow_len", &ret->shadow_len);


	dshd_unif(hmap_shd, "bg", &G.bg_index);
	dshd_unif(hmap_shd, "screen_res", &Screen_size);
	// dshd_unif(hmap_shd, "screen_res", &De.fsize);

	ret->shadow_map = dfbo_new(1024,1024);
	if(!hmap_shadow_map_shd) hmap_shadow_map_shd = dshd_new(shadow_map_vsrc,shadow_map_fsrc);
	dshd_unif(hmap_shadow_map_shd, "mvp", &depth_mvp);



	return ret;
}




void hmap_free(hmap* hm){ if(!hm) return;// todo
	dvao_free(hm->vao);
	SDL_DestroyMutex(hm->cache_mutex);
	free(hm->cache);
	free(hm);
}



float hmap_eval(hmap* hm, float x, float y){ return hmap_evald(hm,x,y,NULL,NULL,NULL);}
float hmap_evald(hmap* hm, float x, float y, float*dx, float*dy, float*dz){ assert(hm);
// return hm->f(x,y);
//i: 0 ->xres
//j: 0 ->yres
// hmap->cache[ i*yres + j ] = func(i - xres/2 + focus[0], j - xres/2 + focus[2]);
	// x= x<0? x+1: x;
	// y= y<0? y+1: y;
	// x/=hm->scale[0];
	// y/=hm->scale[2];

SDL_LockMutex(hm->cache_mutex);
	int i = CLAMP( floor(x) - hm->ifocus[0] + hm->res/2, 0, hm->res-1 );
	int j = CLAMP( floor(y) - hm->ifocus[2] + hm->res/2, 0, hm->res-1 );
	int i1 = i == hm->res-1 ? i : i + 1;
	int j1 = j == hm->res-1 ? j : j + 1;
	float h0 = hm->cache[i * hm->res + j];
	float h1 = hm->cache[i1 * hm->res + j];
	float h2 = hm->cache[i * hm->res + j1];
	float h3 = hm->cache[i1 * hm->res + j1];
SDL_UnlockMutex(hm->cache_mutex);

	float xf = x -((int)x);
	float zf = y -((int)y);
	v4f p = v4f_0; // plane
	if(xf<0) xf+=1;
	if(zf<0) zf+=1;
#if 0 // 2 triangles
//select triangle
	// f(x): -x+1
	if(zf< -xf+1){ // lower	 left triange
		//plane h0,h1,h2
		p = v3f_cross((v4f){1,h1,0,0}-(v4f){0,h0,0,0}, (v4f){0,h2,1,0}-(v4f){0,h0,0,0});
		// p[3] = - (p[0]*1 + p[1]*h1); // eval point to get d
		// float y = -(p[0]*xf + p[2]*zf + p[3])/p[1];
		// return y;
	} else { // top right triange
		//plane h1,h2,h3
		p = v3f_cross((v4f){1,h1,0,0}-(v4f){1,h3,1,0}, (v4f){0,h2,1,0}-(v4f){1,h3,1,0});
		// p[3] = - (p[0]*1 + p[1]*h1); // eval point to get d
		// float y = -(p[0]*xf + p[2]*zf + p[3])/p[1];
		// return y;
	}
	p[3] = - (p[0]*1 + p[1]*h1); // eval point to get d
	return -(p[0]*xf + p[2]*zf + p[3])/p[1];
#else // 4 triangles
float ha = /*_spike+*/ (h0+h1+h2+h3) / 4.f;
v4f ca = {.5,ha,.5,0};
char select=0;
	// f(x): -x+1
	if(zf< -xf+1){ // lower left triange
		//plane h0,h1,h2
		if(zf > xf){ // left
		select=0;
			p = v3f_normalize(v3f_cross((v4f){0,h2,1,0}-ca, (v4f){0,h0,0,0}-ca));
		} else { // bottom
			select=1;
			p = v3f_normalize(v3f_cross((v4f){0,h0,0,0}-ca, (v4f){1,h1,0,0}-ca));
		}
	} else { // top right triange
		if(zf > xf){ // top
			select=2;
		//plane h1,h2,h3
			p = v3f_normalize(v3f_cross((v4f){1,h3,1,0}-ca, (v4f){0,h2,1,0}-ca));
		} else { //right
			select=3;
			p = v3f_normalize(v3f_cross((v4f){1,h1,0,0}-ca, (v4f){1,h3,1,0}-ca));

		}
		// p[3] = - (p[0]*1 + p[1]*h1); // eval point to get d
		// float y = -(p[0]*xf + p[2]*zf + p[3])/p[1];
		// return y;
	}
	p=-p; // normals are backwards for some reason
// assert(p[1]>0);
	//fill derivs
	// if(dx){
	// 	switch (select){
	// 		case 0: *dx = 2*(MAX(ha,h1)-h0); break;/*left*/ 
	// 		case 1: *dx = h1-h0; break;/*bott*/ 
	// 		case 2: *dx = h3-h2; break;/*top*/  
	// 		case 3: *dx = 2*(h1-ha); break;/*right*/
	// 	}
	// }
	// if(dz){
	// 	switch (select){
	// 		case 0: *dz = MAX(h2,ha)-h0; break;/*left*/ 
	// 		case 1: *dz = 2*(ha-h0); break;/*bott*/ 
	// 		case 2: *dz = -2*(MAX(h2,h3)-ha); break;/*top*/  
	// 		case 3: *dz = MAX(h3,ha)-h1; break;/*right*/
	// 	}
	// }
if(dx) *dx=p[0];
if(dy) *dy=p[1];
if(dz) *dz=p[2];


	p[3] = - (p[0]*.5 + p[1]*ha + p[2]*.5); // eval point to get d
	return -(p[0]*xf + p[2]*zf + p[3])/p[1];
#endif
}


























void hmap_update(hmap* hmap, v4f infocus){
		hmap->focus = infocus;
		v4f ifocus = {floor(infocus[0]), floor(infocus[1]), floor(infocus[2]), 0};
		hmap->ifocus = ifocus;

		// make hmap square to fit shadow?
		hmap->focus[3] =  hmap->x;
		hmap->ifocus[3] = hmap->x;

		int xres = hmap->x;
		int yres = hmap->y;
		// typeof(hmap->f) func = hmap->f;
		// v4f sca = hmap->sca;
		//if dynamic
		v4f focus = hmap->mode & HMAP_FIXED? ifocus : infocus;

				focus[1] = 0;
				// ifocus[1] = 0;
				focus[3] = 0;



	// infocus/=hmap->scale;
	// _spike = pow(sin(G.frame/500.f)/2, 3) >0 ? 0.3 : 0;
	// _spike=0;

	// hmap->m = m4f_model(v4f_0, v4f_0, hmap->sca);
	// hmap->n = m4f_timodel(hmap->m);
	// _hmap_MVP = m4f_mul(hmap->m, De.cam.vp);

	if((!hmap->generated) || (hmap->mode & HMAP_DYNAMIC)){

		SDL_LockMutex(hmap->cache_mutex);
		hmap->focus = infocus;
		v4f ifocus = {floor(infocus[0]), floor(infocus[1]), floor(infocus[2]), 0};
		hmap->ifocus = ifocus;

		// make hmap square to fit shadow?
		hmap->focus[3] =  hmap->x;
		hmap->ifocus[3] = hmap->x;

		int xres = hmap->x;
		int yres = hmap->y;
		// typeof(hmap->f) func = hmap->f;
		// v4f sca = hmap->sca;
		//if dynamic
		v4f focus = hmap->mode & HMAP_FIXED? ifocus : infocus;

				focus[1] = 0;
				// ifocus[1] = 0;
				focus[3] = 0;

		// v4f focus_fract = {modf(infocus[0], &dd), 0, modf(infocus[2], &dd), 0};

		hmap->vao->vbos[0]->usage = GL_STREAM_DRAW;
		hmap->vao->vbos[1]->usage = GL_STREAM_DRAW;

//cache
		for (int i = 0; i < xres; ++i){
			for (int j = 0; j < yres; ++j){
				// for (int k = 0; k < 6; ++k){ // for each point in 2 triangles
				// int k = 0;
				hmap->cache[ i*yres + j ] = hmap->f( (i - xres/2 + ifocus[0]),  (j - xres/2 + ifocus[2]) );
				// hmap->cache[ i*yres + j ] = hmap_eval(hmap, i - xres/2 + ifocus[0], j - xres/2 + ifocus[2]);
			}
		}
		SDL_UnlockMutex(hmap->cache_mutex);

#if 0 // 2 tris per tile
// generate vertices
		for (int i = 0; i < xres; ++i){
			for (int j = 0; j < yres; ++j){
				// for (int k = 0; k < 6; ++k){ // for each point in 2 triangles
				int k = 0;
				v4f p0 = (v4f){i  -xres/2, hmap_eval(hmap, i  -xres/2+focus[0], j  -xres/2+focus[2] ), j  -yres/2, 1};
				v4f p1 = (v4f){i+1-xres/2, hmap_eval(hmap, i+1-xres/2+focus[0], j  -xres/2+focus[2] ), j  -yres/2, 1};
				v4f p2 = (v4f){i  -xres/2, hmap_eval(hmap, i  -xres/2+focus[0], j+1-xres/2+focus[2] ), j+1-yres/2, 1};
				v4f p3 = (v4f){i+1-xres/2, hmap_eval(hmap, i+1-xres/2+focus[0], j+1-xres/2+focus[2] ), j+1-yres/2, 1};
				focus[1] = 0;
				// ifocus[1] = 0;
				focus[3] = 0;

				// flat shading
				v4f n0 = v3f_normalize(v3f_cross(p2-p0, p1-p0));
				v4f n1 = v3f_normalize(v3f_cross(p1-p3, p2-p3));
				// square shading
				// v4f n2 = (n0+n1) /2;
				// n0=n2;
				// n1=n2;
				p0 += focus;//focus - focus_fract;
				p1 += focus;//focus - focus_fract;
				p2 += focus;//focus - focus_fract;
				p3 += focus;//focus - focus_fract;

				hmap_vbuf[i*yres*6 + j*6 + k] = p0; hmap_nbuf[i*yres*6 + j*6 + k] = n0; k++;
				hmap_vbuf[i*yres*6 + j*6 + k] = p1; hmap_nbuf[i*yres*6 + j*6 + k] = n0; k++;
				hmap_vbuf[i*yres*6 + j*6 + k] = p2; hmap_nbuf[i*yres*6 + j*6 + k] = n0; k++;
				hmap_vbuf[i*yres*6 + j*6 + k] = p2; hmap_nbuf[i*yres*6 + j*6 + k] = n1; k++;
				hmap_vbuf[i*yres*6 + j*6 + k] = p1; hmap_nbuf[i*yres*6 + j*6 + k] = n1; k++;
				hmap_vbuf[i*yres*6 + j*6 + k] = p3; hmap_nbuf[i*yres*6 + j*6 + k] = n1; k++;
			}
		}
		dvao_setr(hmap->vao, 0, hmap_vbuf, 0, xres*yres*6);
		dvao_setr(hmap->vao, 1, hmap_nbuf, 0, xres*yres*6);
		dvao_update(hmap->vao);
	}
	hmap->generated = true;
#else // 4 tris per tile
		float dx0;
		float dy0;
		float dz0;
		float dx1;
		float dy1;
		float dz1;
		float dx2;
		float dy2;
		float dz2;
		float dx3;
		float dy3;
		float dz3;
		float d=.01;
		float e=.02;
		int cache_nor=0;
		v4f nor[xres*yres]; // cache of all normals before interpolation
		// smooth shading
		if(!cache_nor){
			cache_nor=1;
			for (int i = 0; i < xres; ++i){
				for (int j = 0; j < yres; ++j){
					// v4f p0 = (v4f){i  -xres/2, hmap_evald(hmap, i  -xres/2+focus[0], -1*(j  -xres/2)+focus[2] , &dx0,NULL,&dz0), -1*(j  -yres/2), 1};
					// v4f p1 = (v4f){i+1-xres/2, hmap_evald(hmap, i+.132-xres/2+focus[0], -1*(j  -xres/2)+focus[2] , &dx1,NULL,&dz1), -1*(j  -yres/2), 1};
					// v4f p2 = (v4f){i  -xres/2, hmap_evald(hmap, i  -xres/2+focus[0], -1*(j+.132-xres/2)+focus[2] , &dx2,NULL,&dz2), -1*(j+1-yres/2), 1};
					// v4f p3 = (v4f){i+1-xres/2, hmap_evald(hmap, i+.132-xres/2+focus[0], -1*(j+.132-xres/2)+focus[2] , &dx3,NULL,&dz3), -1*(j+1-yres/2), 1};
					
					// v4f p4 = (v4f){i-1-xres/2, hmap_evald(hmap, i-.132-xres/2+focus[0], -1*(j  -xres/2)+focus[2] , &dx3,NULL,&dz3), -1*(j  -yres/2), 1};
					// v4f p5 = (v4f){i  -xres/2, hmap_evald(hmap, i  -xres/2+focus[0], -1*(j-.132-xres/2)+focus[2] , &dx3,NULL,&dz3), -1*(j-1-yres/2), 1};
					

					// v4f pa = (p0+p1+p2+p3) /4.f;

					// v4f n = .25*(v3f_cross(p4-p0, p5-p0) + v3f_cross(p2-p0, p4-p0) + v3f_cross(p1-p0, p2-p0) + v3f_cross(p2-p0, p5-p0)) ;
					
					v4f n=v4f_0;// = .25f*((v4f){dx0,dy0,dz0,0}+(v4f){dx1,dy1,dz1,0}+(v4f){dx2,dy2,dz2,0}+(v4f){dx3,dy3,dz3,0});					
					hmap_evald(hmap, i+e-xres/2+focus[0], -1*(j+d-xres/2)+focus[2] , &dx0,&dy0,&dz0);
					n += (v4f){dx0,dy0,dz0,0};
					hmap_evald(hmap, i+d-xres/2+focus[0], -1*(j+e-xres/2)+focus[2] , &dx0,&dy0,&dz0);
					n += (v4f){dx0,dy0,dz0,0};

					hmap_evald(hmap, i-e-xres/2+focus[0], -1*(j+d-xres/2)+focus[2] , &dx0,&dy0,&dz0);
					n += (v4f){dx0,dy0,dz0,0};
					hmap_evald(hmap, i-d-xres/2+focus[0], -1*(j+e-xres/2)+focus[2] , &dx0,&dy0,&dz0);
					n += (v4f){dx0,dy0,dz0,0};

					hmap_evald(hmap, i-e-xres/2+focus[0], -1*(j-d-xres/2)+focus[2] , &dx0,&dy0,&dz0);
					n += (v4f){dx0,dy0,dz0,0};
					hmap_evald(hmap, i-d-xres/2+focus[0], -1*(j-e-xres/2)+focus[2] , &dx0,&dy0,&dz0);
					n += (v4f){dx0,dy0,dz0,0};

					hmap_evald(hmap, i+e-xres/2+focus[0], -1*(j-d-xres/2)+focus[2] , &dx0,&dy0,&dz0);
					n += (v4f){dx0,dy0,dz0,0};
					hmap_evald(hmap, i+d-xres/2+focus[0], -1*(j-e-xres/2)+focus[2] , &dx0,&dy0,&dz0);
					n += (v4f){dx0,dy0,dz0,0};

					n/=8;
					// n += .25f*((v4f){dx0,dy0,dz0,0}+(v4f){dx1,dy1,dz1,0}+(v4f){dx2,dy2,dz2,0}+(v4f){dx3,dy3,dz3,0});
					// v4f n = v3f_normalize(-v3f_cross(p0-pa, p2-pa)); // normals are inverted ??



					nor[i*yres + j] = n;
				}
			}
		}

		for (int i = 0; i < xres; ++i){
			for (int j = 0; j < yres; ++j){
				// for (int k = 0; k < 6; ++k){ // for each point in 2 triangles
				v4f p0 = (v4f){i  -xres/2, hmap_evald(hmap, i  -xres/2+focus[0], -1*(j  -xres/2)+focus[2] , &dx0,NULL,&dz0), -1*(j  -yres/2), 1};
				v4f p1 = (v4f){i+1-xres/2, hmap_evald(hmap, i+1-xres/2+focus[0], -1*(j  -xres/2)+focus[2] , &dx1,NULL,&dz1), -1*(j  -yres/2), 1};
				v4f p2 = (v4f){i  -xres/2, hmap_evald(hmap, i  -xres/2+focus[0], -1*(j+1-xres/2)+focus[2] , &dx2,NULL,&dz2), -1*(j+1-yres/2), 1};
				v4f p3 = (v4f){i+1-xres/2, hmap_evald(hmap, i+1-xres/2+focus[0], -1*(j+1-xres/2)+focus[2] , &dx3,NULL,&dz3), -1*(j+1-yres/2), 1};

				v4f pa = (p0+p1+p2+p3) /4.f;
				
				// v2d noi = (v2d){pa[0]+focus[0],pa[2]+focus[2]}/10.0+G.frame/8000.0;
				// _spike=noise2(noi);
				// pa[1]+=_spike;
	
				// square shading
				// v4f n2 = (n0+n1) /2;
				// n0=n2;
				// n1=n2;
				

				p0 += focus;//focus - focus_fract;
				p1 += focus;//focus - focus_fract;
				p2 += focus;//focus - focus_fract;
				p3 += focus;//focus - focus_fract;
				pa += focus;
				int k = 0;
#if 0 /*flat on/off*/
				// flat shading
				v4f nl = v3f_normalize(v3f_cross(p0-pa, p2-pa));
				v4f nr = v3f_normalize(v3f_cross(p3-pa, p1-pa));// warning order
				v4f nt = v3f_normalize(v3f_cross(p2-pa, p3-pa));
				v4f nb = v3f_normalize(v3f_cross(p1-pa, p0-pa));// warning order
				pa[1] -= _spike;
				// semi smooth
				v4f na = (nl+nr+nt+nb)/4.0;

				hmap_vbuf[i*yres*12 + j*12 + k] = pa; hmap_nbuf[i*yres*12 + j*12 + k] = na; k++;
				hmap_vbuf[i*yres*12 + j*12 + k] = p0; hmap_nbuf[i*yres*12 + j*12 + k] = nb; k++;
				hmap_vbuf[i*yres*12 + j*12 + k] = p1; hmap_nbuf[i*yres*12 + j*12 + k] = nb; k++;

				hmap_vbuf[i*yres*12 + j*12 + k] = pa; hmap_nbuf[i*yres*12 + j*12 + k] = na; k++;
				hmap_vbuf[i*yres*12 + j*12 + k] = p2; hmap_nbuf[i*yres*12 + j*12 + k] = nl; k++;
				hmap_vbuf[i*yres*12 + j*12 + k] = p0; hmap_nbuf[i*yres*12 + j*12 + k] = nl; k++;

				hmap_vbuf[i*yres*12 + j*12 + k] = pa; hmap_nbuf[i*yres*12 + j*12 + k] = na; k++;
				hmap_vbuf[i*yres*12 + j*12 + k] = p3; hmap_nbuf[i*yres*12 + j*12 + k] = nt; k++;
				hmap_vbuf[i*yres*12 + j*12 + k] = p2; hmap_nbuf[i*yres*12 + j*12 + k] = nt; k++;

				hmap_vbuf[i*yres*12 + j*12 + k] = pa; hmap_nbuf[i*yres*12 + j*12 + k] = na; k++;
				hmap_vbuf[i*yres*12 + j*12 + k] = p1; hmap_nbuf[i*yres*12 + j*12 + k] = nr; k++;
				hmap_vbuf[i*yres*12 + j*12 + k] = p3; hmap_nbuf[i*yres*12 + j*12 + k] = nr; k++;
#else
				// v4f pd = (v4f){i  -xres/2, hmap_evald(hmap, i  -xres/2+focus[0], j  -xres/2+focus[2] , dx0, dz0), j  -yres/2, 1};
				// v4f pl = (v4f){i+1-xres/2, hmap_evald(hmap, i+1-xres/2+focus[0], j  -xres/2+focus[2] , dx1, dz1), j  -yres/2, 1};
				
				// v4f n0 = v3f_normalize(v3f_cross(p0-pa, p2-pa));
				// v4f n1 = v3f_normalize(v3f_cross(p3-pa, p1-pa));
				// v4f n2 = v3f_normalize(v3f_cross(p2-pa, p3-pa));
				// v4f n3 = v3f_normalize(v3f_cross(p1-pa, p0-pa));
				
				// int k = 0;
				int ti=i, tj=j;

				v4f n0=.2f*( nor[ti*yres+tj] + nor[MAX(ti-1,0)*yres+tj] + nor[ti*yres+MAX(tj-1,0)] + nor[MIN(ti+1,xres)*yres+tj] + nor[ti*yres+MIN(tj+1, yres)] );
				ti++;
				v4f n1=.2f*( nor[ti*yres+tj] + nor[MAX(ti-1,0)*yres+tj] + nor[ti*yres+MAX(tj-1,0)] + nor[MIN(ti+1,xres)*yres+tj] + nor[ti*yres+MIN(tj+1, yres)] );
				ti--;tj++;
				v4f n2=.2f*( nor[ti*yres+tj] + nor[MAX(ti-1,0)*yres+tj] + nor[ti*yres+MAX(tj-1,0)] + nor[MIN(ti+1,xres)*yres+tj] + nor[ti*yres+MIN(tj+1, yres)] );
				ti++;
				v4f n3=.2f*( nor[ti*yres+tj] + nor[MAX(ti-1,0)*yres+tj] + nor[ti*yres+MAX(tj-1,0)] + nor[MIN(ti+1,xres)*yres+tj] + nor[ti*yres+MIN(tj+1, yres)] );
				v4f na=.25*(n0+n1+n2+n3);
//bot
				hmap_vbuf[i*yres*12 + j*12 + k] = pa; hmap_nbuf[i*yres*12 + j*12 + k] = na; k++;
				hmap_vbuf[i*yres*12 + j*12 + k] = p0; hmap_nbuf[i*yres*12 + j*12 + k] = n0; k++;
				hmap_vbuf[i*yres*12 + j*12 + k] = p1; hmap_nbuf[i*yres*12 + j*12 + k] = n1; k++;
//left
				hmap_vbuf[i*yres*12 + j*12 + k] = pa; hmap_nbuf[i*yres*12 + j*12 + k] = na; k++;
				hmap_vbuf[i*yres*12 + j*12 + k] = p2; hmap_nbuf[i*yres*12 + j*12 + k] = n2; k++;
				hmap_vbuf[i*yres*12 + j*12 + k] = p0; hmap_nbuf[i*yres*12 + j*12 + k] = n0; k++;
//top
				hmap_vbuf[i*yres*12 + j*12 + k] = pa; hmap_nbuf[i*yres*12 + j*12 + k] = na; k++;
				hmap_vbuf[i*yres*12 + j*12 + k] = p3; hmap_nbuf[i*yres*12 + j*12 + k] = n3; k++;
				hmap_vbuf[i*yres*12 + j*12 + k] = p2; hmap_nbuf[i*yres*12 + j*12 + k] = n2; k++;
//right
				hmap_vbuf[i*yres*12 + j*12 + k] = pa; hmap_nbuf[i*yres*12 + j*12 + k] = na; k++;
				hmap_vbuf[i*yres*12 + j*12 + k] = p1; hmap_nbuf[i*yres*12 + j*12 + k] = n1; k++;
				hmap_vbuf[i*yres*12 + j*12 + k] = p3; hmap_nbuf[i*yres*12 + j*12 + k] = n3; k++;
#endif


			}
		}
		_spike=0;
		dvao_setr(hmap->vao, 0, hmap_vbuf, 0, xres*yres*12);
		dvao_setr(hmap->vao, 1, hmap_nbuf, 0, xres*yres*12);
		dvao_update(hmap->vao);
	}
	hmap->generated = true;

#endif
}

























void hmap_draw(hmap* hmap){
//draw shadow map
	// dfbo* prevfbo=dfbo_bound();
	// dcullf(0);
	// ddepth(1,0);
	// dfbo_bind(hmap->shadow_map);
	// dtex_filter(hmap->shadow_map->depth, GL_LINEAR,GL_LINEAR);

	// dclear_color((v4f){0,0,0,0});
	// dclear(0);
	// dcam_update();
	// m4f depth_proj = 
	// m4f_ortho( -hmap->res/2, hmap->res/2, hmap->res/2, -hmap->res/2, hmap->res/2, -hmap->res/2);
	// m4f_proj(M_PI*25.0, De.cam.res, 25, 200+64);
	// m4f depth_view = m4f_look_at(G.sunpos, hmap->focus*(v4f){1,.1,1,0}, (v4f){0,1,0,0});
	// depth_mvp = m4f_mul(depth_view, depth_proj);
	// dshd_bind(hmap_shadow_map_shd);
	// dshd_update(hmap_shadow_map_shd);
	// dvao_draw(hmap->vao, 0, hmap->vao->len ,GL_TRIANGLES);
	// dfbo_bind(prevfbo);
	// dcam_update();

	
// m4f bias= {
// 	0.5, 0.0, 0.0, 0.0,
// 	0.0, 0.5, 0.0, 0.0,
// 	0.0, 0.0, 0.5, 0.0,
// 	0.5, 0.5, 0.5, 1.0
// };
// 	depth_mvp = m4f_mul( bias, depth_mvp);

	dcullf('b');
	ddepth(1,0);
	// dcullf(0);
	// ddepth(0,0);
	dblend(1);

	Screen_size[0] = dfbo_bound() ? dfbo_bound()->x : De.size[0];
	Screen_size[1] = dfbo_bound() ? dfbo_bound()->y : De.size[1];

	dtex_bind(hmap_tex, zero);

	if ( G.enemy_bullets->shadow_map && hmap->use_shadow )
		dtex_bind(G.enemy_bullets->shadow_map->color, tunit_one);
	else
		dtex_bind(NULL, tunit_one);
	
	dtex_bind(G.bg->color, G.bg_index);
	dtex_bind(hmap->shadow_map->depth, tunit_two);

	// dshd_unif(hmap_shd, "tex", &zero);
	// dshd_unif(hmap_shd, "shadow_map", &tunit_one);

	dshd_bind(hmap_shd);
	dshd_update(hmap_shd);

	// dvao_bind(hmap->vao);
	// dvao_update(hmap->vao);
	dvao_draw(hmap->vao, 0, hmap->vao->len ,GL_TRIANGLES);

	// dcullf('b');
	// dblend(0);


	// draw insides
	if(G.cam_in_hmap){
		dcullf('f');
		dblend(1);
		dtex_bind(hmap_inside_tex, zero);
		dvao_draw(hmap->vao, 0, hmap->vao->len ,GL_TRIANGLES);
	}


}


































char* hmap_vshdsrc = DSHD_QUOTE(
	attribute vec4 ap;
	attribute vec4 an;

	uniform mat4 mvp;
	// uniform vec4 focus;
	// uniform mat4 view;
	uniform vec4 lpos;
	uniform vec4 sunc;

	uniform vec4 eye;
	uniform vec4 ppos;

	// varying vec4 col;
	varying vec4 vp;
	varying vec4 vn;
	varying vec4 vcol;
	varying vec4 vtc;

	mat4 M = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	mat4 N = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	// vec4 lpos = vec4(0.0, 5.0, 0.0, 1.0);
	// vec4 lpos = eye;
	vec4 lcol = vec4(0.98, 0.98, 1.0, 1.0);
	// vec4 lcol = vec4(mod(ap.x,1.0), mod(ap.y,1.0), mod(ap.z,1.0) , 1.0);
	// vec4 lcol = vec4(1.0, 1.0, 1.0 , 1.0);
	float lpow = 0.987;
	vec4 spec = vec4(0.0095, 0.95, 0.95, 1.0);
	// vec4 spec = vec4(0.0, 0.0, 1.0, 1.0);
	// vec4 diff = vec4(0.95, 0.94, 0.95, 1.0);
	vec4 diff = vec4(1.0, 1.0, 1.0, 1.0);
	float shin = 16.0;

	// \nvec4 light(
	// 	vec4 vpos, vec4 vnor, mat4 N,
	// 	vec4 lpos, vec4 lcol, float lpow,
	// 	vec4 diff
	// ){ \n\t
	vec4 light(
		vec4 eye, vec4 vpos, vec4 vnor, mat4 M, mat4 N,
		vec4 lpos, vec4 lcol, float lpow,
		vec4 diff, vec4 spec, float shininess
	){
	    vec4 inor = normalize(N * vnor);                                 \n\t
	\t    vec4 lspec = vec4(0.0, 0.0, 0.0, 1.0);                           \n
	\t    vec4 v = normalize( eye - M * vpos);                             \n
	    vec4 ldirv = lpos - vpos;                                        \n\t
	    ldirv.a = 0.0;                                        \n\t
	    vec4 ldir = normalize(ldirv);                                    \n\t
	\t    vec4 r = reflect(-ldir, inor);                                   \n
	    float cosTheta = max(dot(ldir, inor), 0.0);                      \n\t
	    float dist2 = lpos.w > .0 ? pow(dot(ldirv, ldirv),.2) : 1.0;                         \n\t
	    vec4 ldiff = diff * lcol * lpow * cosTheta/dist2;              \n\t
	\t    if(cosTheta > 0.0)                                               \n
	// \t\t        lspec = ( spec  *  lcol  *  lpow  *                        \n
	\t\t        lspec = cosTheta * ( spec  *  lcol  *  lpow  *                        \n
	\t\t\t            pow(max(dot(r,v), 0.0),  shininess ))/dist2;  \n
	\t    return ldiff + lspec ;                                            \n
	    // return ldiff;                                            \n
	}\n

	void main(void){
		// vec4 lpos = ppos;
		// lpos.y += 10.0;
		// vec4 lpos = vec4(0.0,3.0,0.0,0.0);
		// fpos = vec4(fpos.xyz + (cam_right * (pos.x/2.0))*pctr.a + (cam_up * (pos.y/2.0))*pctr.a, 1.0);
		// col = normalize(n) + vec4(10.0,10.0,10.0,10.0);
		// // col = vec4(10.0,10.0,10.0,1.0);
		// //coloring
vec4 pos = ap;
// vtc=atc;
		vcol = vec4(0.0,0.0,0.0,0.0);
		// if(ap.y>5.0) vcol *= 0.0;

		// if(ap.y < 0.0) vcol = normalize(
		// 		mix(vcol, vec4(0.3,0.67,0.89,0.11), -mod( vec4(ap.y, ap.y, ap.y, 0.0)*256.0, 5.0) )
		// );


//fragment light
		vp = ap;
		vn = an;

//vertex light
		// vcol *= light(eye, ap, an, M,N, ppos+vec4(0.0,0.5,0.0,0.0), lcol, lpow, spec, diff, shin)
		      // + light(eye, ap, an, M,N, lpos+vec4(0.0,0.5,0.0,0.0), lcol, lpow, spec, diff, shin);
		// vcol *= light(ap, an, N, ppos+vec4(0.0,0.5,0.0,0.0), /*lcol**/vec4(1.0,1.0,0.0,1.0), lpow*-5.0, diff)
		//       + light(ap, an, N, lpos+vec4(0.0,0.5,0.0,0.0), lcol, lpow*10.0, diff);
		// vcol += vec4(.3,.2,.23, 0.0);

//sun
		// vcol +=
		//        light(eye, vp, vn, M,N, vec4(1.0,1.0,1.0,0.0)*lpos+vec4(0.0,10.5,100.0,0.0), lcol*vec4(0.82,0.87,0.68,0.0), lpow*1.0, diff, spec*0.0,shin);

			// light(eye, vp, vn, M,N, lpos+vec4(0.0,0.2175,0.0,1.0),   vec4(0.82,0.82,0.98,1.0), lpow*0.997, diff, vec4(1.0,0.0,0.0,0.0),shin)
			//  +


		vcol += 
					 // light(eye, vp, vn, M,N, lpos+vec4(0.0,0.75,0.0,1.0),   vec4(0.82,0.82,0.98,1.0), lpow*0.997, diff, vec4(1.0,0.0,0.0,0.0),shin);
					 light(eye, vp, vn, M,N, lpos,   sunc, lpow, diff, sunc,shin);
		 // +      light(eye, vp, vn, M,N, vec4(1.0,1.0,1.0,0.0)*lpos+vec4(0.0,100.5,0.0,0.0), lcol*vec4(0.82,0.87,0.68,0.0), lpow*1.0, diff, spec*2.0,shin)

// //ambient
		// vcol += vec4(.39,.28,.21, 0.0) ; // amb
		vcol.a=1.0;
		// vcol=clamp(vcol, 0.0, 1.1);
		// if (ap.y<.0)
		// 	vcol *= vec4(.59,.58,.71, 1.0);
		// vcol = pow(vcol, vec4(1.5,1.5,1.5,1.0));

		// vcol=vec4(1.0,1.0,1.0,1.0);
		// vcol += vec4(.9,.8,.821, 0.0)*5.5; // amb


		// vp.y += length(vp.xz);
		// float d = length(eye.xz - vp.xz)/10.0;
		// vp.y += d*d*d;

		gl_Position = mvp * vp;
	}
);




char* hmap_fshdsrc = DSHD_QUOTE(
	// uniform mat4 mvp;
	uniform sampler2D tex;
	uniform sampler2D shadow_map;
	uniform sampler2D depth_map;
	uniform mat4 depth_mvp;
	uniform sampler2D bg;
	// uniform vec4 sca;
	uniform vec2 screen_res;
	uniform vec4 lpos;
	uniform vec4 sunc;

	uniform vec4 eye;
	uniform vec4 ppos;
	uniform float shadow_len;

	uniform vec4 focus;
	uniform vec4 fogcolor;
	// uniform vec4 sca;

	varying vec4 vp;
	varying vec4 vn;
	varying vec4 vcol;
	varying vec4 vtc;


	mat4 M = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	mat4 N = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	// vec4 lpos = vec4(0.0, 5.0, 0.0, 1.0);
	// vec4 lpos = eye;
	// vec4 lcol = vec4(mod(ap.x,1.0), mod(ap.y,1.0), mod(ap.z,1.0) , 1.0);
	// vec4 lcol = vec4(1.0, 1.0, 1.0 , 1.0);
	float lpow = 0.987;
	vec4 spec = vec4(0.0095, 0.95, 0.95, 1.0);
	// vec4 spec = vec4(0.0, 0.0, 1.0, 1.0);
	// vec4 diff = vec4(0.95, 0.94, 0.95, 1.0);
	vec4 diff = vec4(1.0, 1.0, 1.0, 1.0);
	float shin = 16.0;

	vec4 light(
		vec4 eye, vec4 vpos, vec4 vnor, mat4 M, mat4 N,
		vec4 lpos, vec4 lcol, float lpow,
		vec4 diff, vec4 spec, float shininess
	){
	    vec4 inor = normalize(N * vnor);                                 \n\t
	\t    vec4 lspec = vec4(0.0, 0.0, 0.0, 1.0);                           \n
	\t    vec4 v = normalize( eye - M * vpos);                             \n
	    vec4 ldirv = lpos - vpos;                                        \n\t
	    vec4 ldir = normalize(ldirv);                                    \n\t
	    vec4 r = reflect(-ldir, inor);                                   \n\t
	    float cosTheta = max(dot(ldir, inor), 0.0);                      \n\t
	    float dist2 = lpos.w > 0.0 ? pow(dot(ldirv, ldirv), .2) : 1.0;                         \n\t
	    vec4 ldiff = diff * lcol * lpow * cosTheta/dist2;              \n\t
	\t    if(cosTheta > 0.0)                                               \n
	// \t\t        lspec = ( spec  *  lcol  *  lpow  *                        \n
	\t\t        lspec = /*cosTheta **/ ( spec  *  lcol  *  lpow  *                        \n
	\t\t\t            pow(max(dot(r,v), 0.0),  shininess ))/dist2;  \n
	\t    return ldiff + lspec ;                                            \n
	    // return ldiff;                                            \n
	}\n



	void main(void){

		// vec4 col = pow(vcol, vec4(2.5) );
		vec4 col = vcol;
//fragment light
		// if(length(vp.xyz-ppos.xyz) < 1.2)
		col += 
		light(eye, vp, vn, M,N, ppos+vec4(0.0,0.75,0.0,1.0),   vec4(0.42,0.2,0.98,1.0), lpow*0.997, diff, vec4(0.42,0.2,0.98,0.0),shin)
		 // +      light(eye, vp, vn, M,N, vec4(1.0,1.0,1.0,0.0)*lpos+vec4(0.0,100.5,0.0,0.0), lcol*vec4(0.82,0.87,0.68,0.0), lpow*1.0, diff, spec*2.0,shin)
		 // +      light(eye, vp, vn, M,N, vec4(1.0,1.0,1.0,0.0)*lpos+vec4(0.0,0.5,1.0,1.0), lcol*vec4(0.82,0.87,0.68,0.0), lpow*1.0, vec4(.1,1.0,0.1,0.0), vec4(.0,1.0,0.0,0.0)*2.0,shin)
		       // light(eye, vp, vn, M,N, vec4(1.0,1.0,1.0,0.0)*ppos+vec4(0.0,0.015,0.0,1.0), lcol*vec4(0.12,0.17,0.98,0.0), lpow*1.0, vec4(.1,0.1,1.0,0.0), vec4(.1,0.1,1.0,0.0),8.0)
		;
//ambient
		// col += vec4(.39,.28,.21, 0.0) ; // amb
		col.a=1.0;
		// col *= light(eye, vp, vn, M,N, ppos+vec4(0.0,0.5,0.0,0.0), lcol*.7, lpow, diff, vec4(0.0,0.0,1.0,0.0), shin)
		//  + light(eye, vp, vn, M,N, vec4(1.0,1.0,1.0,0.0)*lpos+vec4(0.0,100.5,100.0,0.0), lcol, lpow, diff, lcol*vec4(0.82,0.87,0.68,0.0), shin);;
		//  +     light(vp, vn, N, lpos+vec4(0.0,0.5,0.0,0.0), lcol, lpow*4.0, diff);;

		// col *= light(vp, vn, N, ppos+vec4(0.0,0.5,0.0,0.0), vec4(0.2,0.2,0.98,0.0), lpow*4.0, diff)
		//  +     light(vp, vn, N, lpos+vec4(0.0,1.5,0.0,0.0), lcol, lpow*4.0, diff);
		// col += vec4(.39,.28,.21, 0.0);
		// col = pow(col, vec4(2.5,2.5,2.5,1.0));
//coloring?
		// if(vp.y < .1) col = vec4(0.3,0.67,0.89,0.0) * vcol; // water
		// // if(vp.y < pow(length(vp.xz)/2.0,3.0)) col = vec4(0.3,0.67,0.89,0.0) * vcol; // water
		// else if(vp.y < .5) col = vec4(0.987,0.877,0.959,0.0) *vcol; //sand
		// else if(vp.y > 2.5) col = vec4(0.9,0.9,0.9,0.0); //snow

//texturing
		// vec4 tcol = texture2D(tex, mod(vp.xz-focus.xz, 1.0));
		vec4 tcol = texture2D(tex, mod(vp.xz, 1.0));
		// float halfpixel=(1.0/64.0)*.5;
		// vec4 tcol = texture2D(tex, mod(vp.xz, 1.0 - halfpixel*2.0)+halfpixel);
		// if(tcol.a == 0.0) discard;

//shadow texture sampling
		#if 1
		// vec2 stc = ((vp.xz-focus.xz)/focus.w) + vec2(0.5);
		// vec2 stc = ((vp.xz-focus.xz)/(focus.w/2.0)) + vec2(0.5);
		// vec2 stc = ((vp.xz-focus.xz)/(focus.w/4.0)) + vec2(0.5); // quarter size . todo mix with bullets.c
		vec2 stc = ((vp.xz-focus.xz)/(shadow_len)) + vec2(0.5); // quarter size . todo mix with bullets.c

		// stc.x = clamp(stc.x, 0.0,1.0);
		// stc.y = clamp(stc.y, 0.0,1.0);
		// vec4 tcol = (texture2D(t, (vp.xz/*-focus.xz*/)/64.0)) * col;
		vec4 stcol = texture2D(shadow_map, stc);


		float sdist = length(vp.xz-ppos.xz);
		// float sfogfactor = (focus.w/5.0 - sdist)/(focus.w/5.0-focus.w/8.0);
		// float sfogfactor = (focus.w/10.0 - sdist)/(focus.w/10.0-focus.w/80.0);
		float sfogfactor = (shadow_len/3.0 - sdist)/(shadow_len/3.0-shadow_len/10.0);
		// float sfogfactor = 1.0;
		// float sfogfactor = (focus.w/10.0 - sdist)/(focus.w/10.0-focus.w/80.0);
		sfogfactor = clamp( sfogfactor, 0.0, 1.0);
		// stcol *= 4.0;
		stcol = mix(vec4(0.0,0.0,0.0,0.0), stcol, sfogfactor);


		// vec4 tcol = texture2D(tex, (vp.xz/*-focus.xz*/));

		// tcol *=  (col);
		// tcol += col -= stcol;
		tcol *= col;
		// tcol -= pow(stcol,vec4(0.5,0.5,0.5,1.0));
		stcol.a=0.0;
		tcol -= stcol;
		#endif //shadow
		// vec4 tcol = texture2D(tex, (vp.xz/*-focus.xz*/));

	// main fog
		float dist = length(vp.xyz-ppos.xyz);
		// float fogfactor = (focus.w/2.5 - dist)/(focus.w/2.5-focus.w/3.0); // focus fog
		// float fogfactor = (focus.w/2.1 - dist)/(focus.w/2.1-focus.w/4.5); // focus fog
		float fogfactor = (focus.w/2.25 - dist)/(focus.w/2.25-focus.w/4.5); // focus fog
		fogfactor = clamp( fogfactor, 0.0, 1.0);

		// if (fogfactor<0.1) discard;

// depth shadow sampling
		// vec4 shadow_coord = depth_mvp * vp;
		// shadow_coord.xyz /= shadow_coord.w;
		// // shadow_coord.xyz /= 450.0;
		// shadow_coord = shadow_coord*.5+0.5;
		// float bias = 0.005;
		// float sdepth = texture2D( depth_map, shadow_coord.xy ).r;
		// if ( sdepth  <  shadow_coord.z-bias){
		// 	tcol*=.5;
		// }
		// tcol.a=1.0;


		// tcol = mix(fogcolor, tcol, fogfactor); // fog
		vec4 bgcol = texture2D(bg, gl_FragCoord.xy / screen_res );
		bgcol.a=0.0; // on low resolution avoids blitting distortion
		tcol = mix( bgcol, tcol, pow(fogfactor, 0.5)); // fog

		// fogfactor = (focus.w/2.25 - dist)/(focus.w/2.25-focus.w/32.5); // focus fog
		// fogfactor = clamp( fogfactor, 0.0, 1.0);
		// tcol = mix( bgcol, tcol, pow(fogfactor,.5)); // fog2


		// tcol = mix(texture2D(bg, gl_FragCoord.xy / vec2(1024.0, 768.0) ), tcol, fogfactor); // fog
		// tcol = mix(vec4(.02, .08, .202, 0.0), tcol, fogfactor); // fog

		// tcol = mix(vec4(.02, .08, .202, 0.0), tco.0l, fogfactor);
		// float shadow = clamp(dist, 0.0, 1.0);
		// tcol = mix(vec4(0.0), tcol, fogfactor);

		//old player shadow
		// float shadow = clamp(pow(length(vp.xz-ppos.xz)*8.0,2.0), 0.25, 1.0);
		// tcol *= shadow;
// new bullet shadow
		// tcol *= texture2D( shadow_map, (vp.xz-focus.xz)/64.0 + 0.5 );
		// tcol += texture2D( shadow_texture, (vp.xz)/128.0 + .5 );
		// tcol = col;
		// if(tcol.w < 0.5) discard;

		// if ( texture2D( depth_map, (shadow_coord.xy/shadow_coord.w) ).r  <  (shadow_coord.z-bias)/shadow_coord.w ){
		// if ( texture2D( depth_map, shadow_coord.xy ).r  <  shadow_coord.z-bias){


		gl_FragColor = tcol ;
		// gl_FragColor = vec4(texture2DProj( depth_map, shadow_coord.xyz ).z) ;
		// gl_FragColor = pow(tcol, vec4(.4545,.4545,.4545,1.0));

		// gl_FragColor.r = col.r;
	}
);

































//depth shadow map shader
char* shadow_map_vsrc = DSHD_QUOTE(
	attribute vec4 ap;
	attribute vec4 an;

	uniform mat4 mvp;
	void main(){
		gl_Position = mvp * (ap + an*0.01);
	}
);
char* shadow_map_fsrc = DSHD_QUOTE(
	void main(){
// gl_FragDepth = gl_FragCoord.z;
	}
);