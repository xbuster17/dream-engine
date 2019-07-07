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

m4f _hmap_MVP;
v4f* hmap_vbuf;
v4f* hmap_nbuf;
v4c* hmap_texpx;
char* hmap_vshdsrc;
char* hmap_fshdsrc;
int tres = 32*2;
int zero = 0;
int tunit_one = 1;
int hmap_max_res = 512;

hmap* hmap_new(int xres, int yres, float(*func)(float x, float y), enum hmap_mode mode){
	if(!hmap_inited){
		hmap_inited = true;
		hmap_vbuf = malloc(sizeof(v4f) * hmap_max_res*hmap_max_res * 6);
		hmap_nbuf = malloc(sizeof(v4f) * hmap_max_res*hmap_max_res * 6);
		hmap_texpx = malloc(sizeof(v4c) * tres*tres);
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
				if(i < 1 || j < 1 || i > tres-1 || j > tres-1 || abs(i - (tres-j))<1 )
				hmap_texpx[i*tres + j] = v4c_rand();
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
		// dtex_filter(hmap_tex, GL_NEAREST,GL_NEAREST);
		dtex_filter(hmap_tex, GL_LINEAR,GL_LINEAR);
		// dtex_filter(hmap_tex, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR);
		// dtex_filter(hmap_tex, GL_NEAREST_MIPMAP_LINEAR, GL_NEAREST);
		// dtex_filter(hmap_tex, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR);

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

	ret->focus = v4f_0;
	ret->ifocus = v4f_0;
	// ret->sca = hmap_sca;
	ret->generated = false;
	ret->mode = mode;

	ret->use_shadow = true;
	// ret->sca = (v4f){10,.1,10,1};

	ret->shadow = NULL;


// //vertices call update
	// void* verts[] = {hmap_vbuf, hmap_nbuf};
// ret->vao = dvao_newr("f4p; f4n", verts, xres*yres*6, GL_TRIANGLES);

	ret->vao = dvao_newr("f4p; f4n", NULL, xres*yres*6, GL_TRIANGLES);

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

	// dshd_unif(hmap_shd, "eye", &De.cam.pos);
	dshd_unif(hmap_shd, "tex", &zero);
	dshd_unif(hmap_shd, "shadow_map", &tunit_one);

	dshd_unif(hmap_shd, "fogcolor", &G.clear_color);

	dshd_unif(hmap_shd, "lpos", &G.enm[0].pos);
	// dshd_utex(hmap_shd, "t", 0);
	if(mode & HMAP_FIXED || !mode)
		dshd_unif(hmap_shd, "focus", &ret->ifocus);
	else
		dshd_unif(hmap_shd, "focus", &ret->focus);

	dshd_unif(hmap_shd, "ppos", &G.player.pos);



	return ret;
}




void hmap_free(hmap* hm){ if(!hm) return;// todo
	dvao_free(hm->vao);
	SDL_DestroyMutex(hm->cache_mutex);
	free(hm->cache);
	free(hm);
}



float hmap_eval(hmap* hm, float x, float y){ if(!hm) return 0;
// return hm->f(x,y);
//i: 0 ->xres
//j: 0 ->yres
// hmap->cache[ i*yres + j ] = func(i - xres/2 + focus[0], j - xres/2 + focus[2]);
	// x= x<0? x+1: x;
	// y= y<0? y+1: y;
	int i = clamp( floor(x) - hm->ifocus[0] + hm->res/2, 0, hm->res-1 );
	int j = clamp( floor(y) - hm->ifocus[2] + hm->res/2, 0, hm->res-1 );
	int i1 = i == hm->res-1 ? i : i + 1;
	int j1 = j == hm->res-1 ? j : j + 1;
SDL_LockMutex(hm->cache_mutex);
	float h0 = hm->cache[i * hm->res + j];
	float h2 = hm->cache[i * hm->res + j1];
	float h1 = hm->cache[i1 * hm->res + j];
	float h3 = hm->cache[i1 * hm->res + j1];
SDL_UnlockMutex(hm->cache_mutex);

	float xf = x -((int)x);
	float zf = y -((int)y);
	v4f p = v4f_0; // plane
	if(xf<0) xf+=1;
	if(zf<0) zf+=1;

//select triangle
	// f(x): -x+1
	if(zf< -xf+1){ // lower left triange
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

}


























void hmap_update(hmap* hmap, v4f infocus){
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
		typeof(hmap->f) func = hmap->f;
		// v4f sca = hmap->sca;
		//if dynamic
		v4f focus = hmap->mode & HMAP_FIXED? ifocus : infocus;

		// v4f focus_fract = {modf(infocus[0], &dd), 0, modf(infocus[2], &dd), 0};

		hmap->vao->vbos[0]->usage = GL_STREAM_DRAW;
		hmap->vao->vbos[1]->usage = GL_STREAM_DRAW;



//cache
		for (int i = 0; i < xres; ++i){
			for (int j = 0; j < yres; ++j){
				// for (int k = 0; k < 6; ++k){ // for each point in 2 triangles
				// int k = 0;
				hmap->cache[ i*yres + j ] = func(i - xres/2 + ifocus[0], j - xres/2 + ifocus[2]);
			}
		}
SDL_UnlockMutex(hmap->cache_mutex);

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
	// void* hmap_verts[] = {hmap_vbuf, hmap_nbuf};
	// hmap_once = 1;
	hmap->generated = true;

}

























void hmap_draw(hmap* hmap){
	// dclear_color(G.clear_color);
	dcullf('b');
	ddepth(1,0);
	// dcullf(0);
	// ddepth(0,0);
	// dblend(1);

	dcam_update();


	dtex_bind(hmap_tex, zero);

	if ( G.enemy_bullets->shadow_map && hmap->use_shadow )
		dtex_bind(G.enemy_bullets->shadow_map->color, tunit_one);
	else
		dtex_bind(NULL, tunit_one);

	// dshd_unif(hmap_shd, "tex", &zero);
	// dshd_unif(hmap_shd, "shadow_map", &tunit_one);

	dshd_bind(hmap_shd);
	dshd_update(hmap_shd);

	dvao_bind(hmap->vao);
	// dvao_update(hmap->vao);
	dvao_draw(hmap->vao, 0, hmap->vao->len ,GL_TRIANGLES);

	// dcullf('b');
	// dblend(0);
}


































char* hmap_vshdsrc = DE_SHD_HEADERV QUOTE(
	attribute vec4 ap;
	attribute vec4 an;

	uniform mat4 mvp;
	// uniform vec4 focus;
	// uniform mat4 view;
	uniform vec4 lpos;

	// uniform vec4 eye;
	uniform vec4 ppos;

	// varying vec4 col;
	varying vec4 vp;
	varying vec4 vn;
	varying vec4 vcol;

	mat4 M = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	mat4 N = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	// vec4 lpos = vec4(0.0, 5.0, 0.0, 1.0);
	// vec4 lpos = eye;
	vec4 lcol = vec4(0.98, 0.98, 1.0, 1.0);
	// vec4 lcol = vec4(mod(ap.x,1.0), mod(ap.y,1.0), mod(ap.z,1.0) , 1.0);
	// vec4 lcol = vec4(1.0, 1.0, 1.0 , 1.0);
	float lpow = 0.987;
	// vec4 spec = vec4(0.95, 0.95, 0.95, 1.0);
	// vec4 spec = vec4(0.0, 0.0, 1.0, 1.0);
	vec4 diff = vec4(0.95, 0.94, 0.95, 1.0);
	// vec4 diff = vec4(1.0, 1.0, 1.0, 1.0);
	// float shin = 16.0;
	\nvec4 light(
		vec4 vpos, vec4 vnor, mat4 N,
		vec4 lpos, vec4 lcol, float lpow,
		vec4 diff
	){ \n\t
	// vec4 light(
	// 	vec4 eye, vec4 vpos, vec4 vnor, mat4 M, mat4 N,
	// 	vec4 lpos, vec4 lcol, float lpow,
	// 	vec4 spec, vec4 diff, float shininess
	// ){
	    vec4 inor = normalize(N * vnor);                                 \n\t
	// \t    vec4 lspec = vec4(0.0, 0.0, 0.0, 1.0);                           \n
	// \t    vec4 v = normalize( eye - M * vpos);                             \n
	    vec4 ldirv = lpos - vpos;                                        \n\t
	    vec4 ldir = normalize(ldirv);                                    \n\t
	// \t    vec4 r = reflect(-ldir, inor);                                   \n
	    float cosTheta = max(dot(ldir, inor), 0.0);                      \n\t
	    float dist2 = pow(dot(ldirv, ldirv), .5);                         \n\t
	    vec4 ldiff = diff * lcol * lpow * cosTheta/dist2;              \n\t
	// \t    if(cosTheta > 0.0)                                               \n
	// \t\t        lspec = ( spec  *  lcol  *  lpow  *                        \n
	// \t\t        lspec = clamp(cosTheta*1000.0, 0.0, 1.0) * ( spec  *  lcol  *  lpow  *                        \n
	// \t\t\t            pow(max(dot(r,v), 0.0),  shininess ))/dist2;  \n
	// \t    return ldiff + lspec ;                                            \n
	    return ldiff;                                            \n
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

		vcol = vec4(1.0,1.0,1.0,1.0);
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


		vcol *= light(vp, vn, N, ppos+vec4(0.0,0.75,0.0,0.0), vec4(0.2,0.2,0.98,0.0), lpow*1.0, diff)
		 +      light(vp, vn, N, lpos+vec4(0.0,1.5,0.0,0.0), lcol, lpow*1.0, diff);
//ambient
		vcol += vec4(.39,.28,.21, 0.0) * 1.5;
		if (ap.y<.0)
			vcol *= vec4(.59,.58,.71, 1.0);
		vcol = pow(vcol, vec4(2.5,2.5,2.5,1.0));



		// vp.y += length(vp.xz);
		// float d = length(eye.xz - vp.xz)/10.0;
		// vp.y += d*d*d;

		gl_Position = mvp * vp;
	}
);




char* hmap_fshdsrc = DE_SHD_HEADERF QUOTE(
	// uniform mat4 mvp;
	uniform sampler2D tex;
	uniform sampler2D shadow_map;
	// uniform vec4 sca;
	uniform vec4 lpos;

	// uniform vec4 eye;
	uniform vec4 ppos;

	uniform vec4 focus;
	uniform vec4 fogcolor;
	// uniform vec4 sca;

	varying vec4 vp;
	varying vec4 vn;
	varying vec4 vcol;



	mat4 M = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	mat4 N = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	// vec4 lpos = vec4(0.0, 5.0, 0.0, 1.0);
	// vec4 lpos = eye;
	vec4 lcol = vec4(1.0, 1.0, 1.0, 1.0);
	// vec4 lcol = vec4(mod(ap.x,1.0), mod(ap.y,1.0), mod(ap.z,1.0) , 1.0);
	// vec4 lcol = vec4(1.0, 1.0, 1.0 , 1.0);
	float lpow = 0.87;
	// vec4 spec = vec4(0.95, 0.95, 0.95, 1.0);
	// vec4 spec = vec4(0.0, 0.0, 1.0, 1.0);
	vec4 diff = vec4(0.95, 0.94, 0.95, 1.0);
	// vec4 diff = vec4(1.0, 1.0, 1.0, 1.0);
	float shin = 16.0;

	vec4 light(
		vec4 vpos, vec4 vnor, mat4 N,
		vec4 lpos, vec4 lcol, float lpow,
		vec4 diff
	){
	// 	vec4 eye, vec4 vpos, vec4 vnor, mat4 M, mat4 N,
	// 	vec4 lpos, vec4 lcol, float lpow,
	// 	vec4 spec, vec4 diff, float shininess
	// ){
	\t    vec4 inor = normalize(N * vnor);                                 \n
	\t    vec4 lspec = vec4(0.0, 0.0, 0.0, 1.0);                           \n
	// \t    vec4 v = normalize( eye - M * vpos);                             \n
	\t    vec4 ldirv = lpos - vpos;                                        \n
	\t    vec4 ldir = normalize(ldirv);                                    \n
	// \t    vec4 r = reflect(-ldir, inor);                                   \n
	\t    float cosTheta = max(dot(ldir, inor), 0.0);                      \n
	\t    float dist2 = dot(ldirv, ldirv);                         \n
	\t    vec4 ldiff = diff * lcol * lpow * cosTheta/dist2;              \n
	// \t    if(cosTheta > 0.0)                                               \n
	// \t\t        lspec = /*clamp(cosTheta*1000.0, 0.0, 1.0) **/ ( spec  *  lcol  *  lpow  *                        \n
	// \t\t\t            pow(max(dot(r,v), 0.0),  shininess ))/dist2;  \n
	// \t    return ldiff + lspec ;                                            \n
	\t    return ldiff;                                            \n
	}



	void main(void){

		// vec4 col = pow(vcol, vec4(2.5) );
		vec4 col = vcol;
//fragment light
		// col *= light(eye, vp, vn, M,N, ppos+vec4(0.0,0.5,0.0,0.0), lcol, lpow, spec, diff, shin)
		//  + light(eye, vp, vn, M,N, lpos+vec4(0.0,0.5,0.0,0.0), lcol, lpow, spec, diff, shin);;
		//  +     light(vp, vn, N, lpos+vec4(0.0,0.5,0.0,0.0), lcol, lpow*4.0, diff);;

		// col *= light(vp, vn, N, ppos+vec4(0.0,0.5,0.0,0.0), vec4(0.2,0.2,0.98,0.0), lpow*4.0, diff)
		//  +     light(vp, vn, N, lpos+vec4(0.0,1.5,0.0,0.0), lcol, lpow*4.0, diff);
		// col += vec4(.39,.28,.21, 0.0);
		// col = pow(col, vec4(2.5,2.5,2.5,1.0));

		// if(vp.y < .1) col = vec4(0.3,0.67,0.89,0.0) * vcol; // water
		// // if(vp.y < pow(length(vp.xz)/2.0,3.0)) col = vec4(0.3,0.67,0.89,0.0) * vcol; // water
		// else if(vp.y < .5) col = vec4(0.987,0.877,0.959,0.0) *vcol; //sand
		// else if(vp.y > 2.5) col = vec4(0.9,0.9,0.9,0.0); //snow

//shadow texture sampling
		#if 1
		// vec2 stc = ((vp.xz-focus.xz)/focus.w) + vec2(0.5);
		// vec2 stc = ((vp.xz-focus.xz)/(focus.w/2.0)) + vec2(0.5);
		vec2 stc = ((vp.xz-focus.xz)/(focus.w/4.0)) + vec2(0.5); // quarter size . todo mix with bullets.c

		// stc.x = clamp(stc.x, 0.0,1.0);
		// stc.y = clamp(stc.y, 0.0,1.0);
		// vec4 tcol = (texture2D(t, (vp.xz/*-focus.xz*/)/64.0)) * col;
		vec4 stcol = texture2D(shadow_map, stc);


		float sdist = length(vp.xz-ppos.xz);
		// float sfogfactor = (focus.w/5.0 - sdist)/(focus.w/5.0-focus.w/8.0);
		float sfogfactor = (focus.w/10.0 - sdist)/(focus.w/10.0-focus.w/80.0);
		sfogfactor = clamp( sfogfactor, 0.0, 1.0);
		// stcol *= 4.0;
		stcol = mix(vec4(0.0,0.0,0.0,1.0), stcol, sfogfactor);
//cheap player light over shadow
		// stcol *= vec4(.17, .17, .9, 1.0) * sdist;

		// vec4 tcol = texture2D(tex, (vp.xz/*-focus.xz*/));
		vec4 tcol = texture2D(tex, (vp.xz-focus.xz));
		// tcol *=  (col);
		tcol += col-stcol;
		// tcol = tcol - stcol;
		#endif //shadow
		// vec4 tcol = texture2D(tex, (vp.xz/*-focus.xz*/));

		// vec4 tcol = col;
		// float dist = length(vp.xz-eye.xz);
		// float fogfactor = (31.0 - dist)/(31.0-20.0); // player center 64 block fog dist

		// float dist = length(vp.xz-focus.xz);
		// float fogfactor = (31.0 - dist)/(31.0-20.0); // player center 64 block fog dist
		// float fogfactor = (40.0 - dist)/(40.0-30.0); // transposed focus
		float dist = length(vp.xz-ppos.xz);
		// float fogfactor = (focus.w/2.5 - dist)/(focus.w/2.5-focus.w/3.0); // focus fog
		float fogfactor = (focus.w/2.1 - dist)/(focus.w/2.1-focus.w/3.0); // focus fog
		fogfactor = clamp( fogfactor, 0.0, 1.0);

		// if (fogfactor<0.1) discard;

		tcol = mix(fogcolor, tcol, fogfactor); // fog
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
		gl_FragColor = tcol ;

		// gl_FragColor = pow(tcol, vec4(.75,.75,.75,1.0));
		// gl_FragColor.r = col.r;
	}
);
