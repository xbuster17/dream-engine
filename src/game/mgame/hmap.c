#include "hmap.h"
#if 0
typedef struct hmap{
	int x,y;
	float(*f)(float x, float y);
	v4f focus;
	v4f ifocus; // floor of focus
	v4f sca;
	dvao* vao;
	dibo* ibo;
	dshd* shd;
	dtex* tex;
} hmap;
#endif

// static m4f hmap_mvp;

void hmap_mode(hmap* hm, bool dynamic, bool follow, bool fixed){ if (!hm) return;
	hm->dynamic = dynamic; // generate new vertices
	hm->follow = follow; // center vertices at focus
	hm->fixed = fixed; // generate vertices based on grid and not focus
}


// hmap
bool hmap_inited = false;
dshd* hmap_shd = NULL;
dtex* hmap_tex = NULL;
v4f* hmap_vbuf;
v4f* hmap_nbuf;
v4c* hmap_texpx;
char* hmap_vshdsrc;
char* hmap_fshdsrc;
int tres = 64;
int zero = 0;
int tunit_one = 1;
// pre{ 0 < xres,yres < 256 }
// v4f hmap_sca = {1,1,1,1};
// int hmap_res = 64;
// #define hmap_res 64
// int hmap_res = 96;
// int hmap_res = 64;
// #define hmap_res 64
int hmap_max_res = 512;

















#include "mgame.h" //game.player.pos






hmap* hmap_new(int xres, int yres, float(*func)(float x, float y), enum hmap_mode mode){
	if(!hmap_inited){
		hmap_inited = true;
		hmap_vbuf = malloc(sizeof(v4f) * hmap_max_res*hmap_max_res * 6);
		hmap_nbuf = malloc(sizeof(v4f) * hmap_max_res*hmap_max_res * 6);
		hmap_texpx = malloc(sizeof(v4c) * tres*tres);

// int num_colors = 8;
// v4c colors[num_colors];
// colors[0] = (v4c){255,255,255,255};
// colors[1] = (v4c){255,0,255,255};
// colors[2] = (v4c){0,0,255,255};
// colors[3] = (v4c){255,0,0,255};
// colors[4] = (v4c){255,255,0,255};
// colors[5] = (v4c){0,255,255,255};
// colors[6] = (v4c){0,155,155,255};
// colors[7] = (v4c){0,255,0,255};

//texture
		for (int i = 0; i < tres; ++i){
			for (int j = 0; j < tres; ++j){
				// v4c c = {255,255,255,255};
				// hmap_texpx[i*tres + j] = c;
				hmap_texpx[i*tres + j] = v4c_0;
				// hmap_texpx[i*tres + j] = (v4c){.02*255, .08*255, .202*255, 255};
				// srand(i/(tres/di) + (j/(tres/di)) *100 );
				// hmap_texpx[i*tres + j] = v4c_rand();
				// hmap_texpx[i*tres + j] = colors[rand()%num_colors];
				// if(i < 1 || j < 1 || i > tres-1 || j > tres-1 || abs(i - (tres-j))<1 )
					// hmap_texpx[i*tres + j] = (v4c){255,255,255,255};
				// if(!(i%(tres/(di))) || !(j%(tres/(di))))
				// if(!(i%(tres/di)) || !(j%(tres/di)) || abs(i%(tres/di) - (tres/di-j%(tres/di)))<1)
				if(i < 3 || j < 3 || i > tres-3 || j > tres-3 || abs(i - (tres-j))<3 )
				// if(i < 1 || j < 1 || i > tres-1 || j > tres-1)

					hmap_texpx[i*tres + j] = (v4c){255,255,255,255};
					// hmap_texpx[i*tres + j] = v4c_0;
					// hmap_texpx[i*tres + j] = v4c_rand();
			}
		}

		hmap_tex = dtex_new(hmap_texpx, tres,tres);
		dtex_bind(hmap_tex, zero);
		glGenerateMipmap(GL_TEXTURE_2D);
		// dtex_filter(hmap_tex, GL_NEAREST,GL_NEAREST);
		// dtex_filter(hmap_tex, GL_LINEAR,GL_LINEAR);
		// dtex_filter(hmap_tex, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR);
		// dtex_filter(hmap_tex, GL_NEAREST_MIPMAP_LINEAR, GL_NEAREST);
		dtex_filter(hmap_tex, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR);

	}

	hmap* ret = malloc(sizeof(hmap));
	ret->f = func;
	ret->x = xres;
	ret->y = yres;
	ret->focus = v4f_0;
	ret->ifocus = v4f_0;
	// ret->sca = hmap_sca;
	ret->generated = false;
	ret->mode = mode;
	// ret->sca = (v4f){10,.1,10,1};

// //vertices call update
	// void* verts[] = {hmap_vbuf, hmap_nbuf};
// ret->vao = dvao_newr("f4p; f4n", verts, xres*yres*6, GL_TRIANGLES);

	ret->vao = dvao_newr("f4p; f4n", NULL, xres*yres*6, GL_TRIANGLES);

	// ret->vao = dvao_newr("f4p", NULL, xres*yres*6, GL_TRIANGLES);

//shader
	if(!hmap_shd) hmap_shd = dshd_new(hmap_vshdsrc, hmap_fshdsrc);
	ret->shd = hmap_shd;

	// dshd_unif(hmap_shd, "view", &game.view);
	// dshd_unif(hmap_shd, "mvp", &hmap_mvp);
	dshd_unif(hmap_shd, "mvp", &De.cam.vp);
	dshd_unif(hmap_shd, "eye", &De.cam.pos);
	dshd_unif(hmap_shd, "tex", &zero);
	dshd_unif(hmap_shd, "shadow_map", &tunit_one);
	// dshd_utex(hmap_shd, "t", 0);
	if(mode & HMAP_FIXED || !mode)
		dshd_unif(hmap_shd, "focus", &ret->ifocus);
	else
		dshd_unif(hmap_shd, "focus", &ret->focus);

	dshd_unif(hmap_shd, "ppos", &game.player.pos);




// int ultex = glGetUniformLocation(hmap_shd->program, "tex");
// DE_LOGE("ultex %i, zero %i", ultex, zero);

// int ulsmap = glGetUniformLocation(hmap_shd->program, "shadow_map");
// DE_LOGE("ulsmap %i, tunit %i", ulsmap, tunit_one);

// glUniform1i(ultex, zero);
// glUniform1i(ulsmap, tunit_one);



return ret;
}




void hmap_free(hmap* hmap){// todo
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
	typeof(hmap->f) func = hmap->f;
	// v4f sca = hmap->sca;
	//if dynamic
	v4f focus = hmap->mode & HMAP_FIXED? ifocus : infocus;
	// v4f focus_fract = {modf(infocus[0], &dd), 0, modf(infocus[2], &dd), 0};

	if((!hmap->generated) || (hmap->mode & HMAP_DYNAMIC)){
		for (int i = 0; i < xres; ++i){
			for (int j = 0; j < yres; ++j){
				// for (int k = 0; k < 6; ++k){ // for each point in 2 triangles
				int k = 0;
				v4f p0 = (v4f){i  -xres/2, func( i  -xres/2+focus[0], j  -xres/2+focus[2] ), j  -yres/2, 1};
				v4f p1 = (v4f){i+1-xres/2, func( i+1-xres/2+focus[0], j  -xres/2+focus[2] ), j  -yres/2, 1};
				v4f p2 = (v4f){i  -xres/2, func( i  -xres/2+focus[0], j+1-xres/2+focus[2] ), j+1-yres/2, 1};
				v4f p3 = (v4f){i+1-xres/2, func( i+1-xres/2+focus[0], j+1-xres/2+focus[2] ), j+1-yres/2, 1};
				focus[1] = 0;
				// ifocus[1] = 0;
				focus[3] = 0;

				p0 += focus;//focus - focus_fract;
				p1 += focus;//focus - focus_fract;
				p2 += focus;//focus - focus_fract;
				p3 += focus;//focus - focus_fract;

				// flat shading
				v4f n0 = /*v3f_normalize*/(v3f_cross(p2-p0, p1-p0));
				// triangle shading
				v4f n1 = /*v3f_normalize*/(v3f_cross(p1-p3, p2-p3));
				// square shading
				// v4f n2 = (n0+n1) /2;
				// n0=n2;
				// n1=n2;


				hmap_vbuf[i*yres*6 + j*6 + k] = p0; hmap_nbuf[i*yres*6 + j*6 + k] = n0; k++;
				hmap_vbuf[i*yres*6 + j*6 + k] = p1; hmap_nbuf[i*yres*6 + j*6 + k] = n0; k++;
				hmap_vbuf[i*yres*6 + j*6 + k] = p2; hmap_nbuf[i*yres*6 + j*6 + k] = n0; k++;
				hmap_vbuf[i*yres*6 + j*6 + k] = p2; hmap_nbuf[i*yres*6 + j*6 + k] = n1; k++;
				hmap_vbuf[i*yres*6 + j*6 + k] = p1; hmap_nbuf[i*yres*6 + j*6 + k] = n1; k++;
				hmap_vbuf[i*yres*6 + j*6 + k] = p3; hmap_nbuf[i*yres*6 + j*6 + k] = n1; k++;

			// }
			}
		}
		dvao_setr(hmap->vao, 0, 0, hmap_vbuf, xres*yres*6);
		dvao_setr(hmap->vao, 1, 0, hmap_nbuf, xres*yres*6);
	}
		// void* hmap_verts[] = {hmap_vbuf, hmap_nbuf};
	// hmap_once = 1;
	hmap->generated = true;

}

























void hmap_draw(hmap* hmap){
	dcullf('b');
	ddepth(1,0);
	// dblend(1);

	dcam_update();


	dtex_bind(hmap_tex, zero);
	dtex_bind(game.enemy_bullets->shadow_map->color, tunit_one);

	// dshd_unif(hmap_shd, "tex", &zero);
	// dshd_unif(hmap_shd, "shadow_map", &tunit_one);

	dshd_bind(hmap_shd);
	dshd_update(hmap_shd);


//wtf this works on droid
// int ultex = glGetUniformLocation(hmap_shd->program, "tex");
// int ulsmap = glGetUniformLocation(hmap_shd->program, "shadow_map");
// glUniform1i(ultex, zero);
// glUniform1i(ulsmap, tunit_one);



	dvao_bind(hmap->vao);
	dvao_update(hmap->vao);
	dvao_draw(hmap->vao, 0, hmap->vao->len ,GL_TRIANGLES);

	dblend(0);
}



































char* hmap_vshdsrc = DE_SHD_HEADERV QUOTE(
	attribute vec4 ap;
	attribute vec4 an;

	uniform mat4 mvp;
	// uniform vec4 sca;
	// uniform vec4 focus;
	// uniform mat4 view;

	uniform vec4 eye;
	uniform vec4 ppos;

	// varying vec4 col;
	varying vec4 vp;
	varying vec4 vcol;

	mat4 M = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	mat4 N = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	// vec4 lpos = vec4(0.0, 5.0, 0.0, 1.0);
	// vec4 lpos = eye;
	vec4 lcol = vec4(1.0, 1.0, 1.0, 1.0);
	// vec4 lcol = vec4(mod(ap.x,1.0), mod(ap.y,1.0), mod(ap.z,1.0) , 1.0);
	// vec4 lcol = vec4(1.0, 1.0, 1.0 , 1.0);
	float lpow = 0.78;
	// vec4 spec = vec4(0.95, 0.95, 0.95, 1.0);
	vec4 spec = vec4(1.0, 1.0, 1.0, 1.0);
	// vec4 diff = vec4(0.95, 0.94, 0.95, 1.0);
	vec4 diff = vec4(1.0, 1.0, 1.0, 1.0);
	float shin = 16.0;

	vec4 light(
		vec4 eye, vec4 vpos, vec4 vnor, mat4 M, mat4 N,
		vec4 lpos, vec4 lcol, float lpow,
		vec4 spec, vec4 diff, float shininess
	){
	\t    vec4 inor = normalize(N * vnor);                                 \n
	\t    vec4 lspec = vec4(0.0, 0.0, 0.0, 1.0);                           \n
	\t    vec4 v = normalize( eye - M * vpos);                             \n
	\t    vec4 ldirv = lpos - vpos;                                        \n
	\t    vec4 ldir = normalize(ldirv);                                    \n
	\t    vec4 r = reflect(-ldir, inor);                                   \n
	\t    float cosTheta = max(dot(ldir, inor), 0.0);                      \n
	\t    vec4 ldiff = diff * lcol * lpow * cosTheta;              \n
	\t    if(cosTheta > 0.0)                                               \n
	\t\t        lspec = ( spec  *  lcol  *  lpow  *                        \n
	\t\t\t            pow(max(dot(r,v), 0.0),  shininess ));  \n
	\t    return ldiff + lspec;                                            \n
	}

	void main(void){
		vec4 lpos = ppos;
		// fpos = vec4(fpos.xyz + (cam_right * (pos.x/2.0))*pctr.a + (cam_up * (pos.y/2.0))*pctr.a, 1.0);
		// col = normalize(n) + vec4(10.0,10.0,10.0,10.0);
		// // col = vec4(10.0,10.0,10.0,1.0);
		lpos.y += 100.0;
		// //coloring
vec4 pos = ap;
		// if(ap.y < .5) lcol = lerp(lcol, vec4(0.3,0.67,1.89,0.0), -vec4(ap.y, ap.y, ap.y, 0.0));
		// else if(ap.y < 1.5) {lcol = vec4(0.287,0.577,0.259,0.0) *lcol;} //sand / fauna
		// else  {lcol = vec4(0.87,0.577,0.96,0.0);} // neon magenta

		// else lcol = lerp(
		// 	vec4(1.0,0.77,0.0,0.0),
		// 	vec4(0.87,0.577,0.96,0.0),
		// 	(vec4(ap.y,ap.y,ap.y,1.5) - vec4(0.5)) / 3.0);
		// if(ap.y < 0.5) lcol = vec4(0.87,0.577,0.96,0.0); // water color
		if(ap.y < 0.5){
			// lcol = vec4(0.07,0.577,0.06,0.0);
			// lcol = vec4(0.87,0.577,0.96,0.0);
			lcol = vec4(1.0, 0.5, 1.0 ,1.0);

			// lcol = vec4(0.07,0.577,0.06,0.0);
		} else lcol = vec4(
			min(pos.y,1.0),
			min(pos.y,1.0),
			min(pos.y,1.0),
			1.0
		);


		// else if(ap.y < 3.0) {lcol = lerp(vec4(0.287,0.577,0.259,0.0), vec4(vec4(0.887,0.777,0.759,0.0)),(vec4(ap.y,ap.y,ap.y,1.0)-3.0)/3.0) *lcol; spec=lcol*0.0;} //stone
		// else  {lcol = vec4(1.0,1.0,1.0,0.0) * lcol; spec=lcol*0.0;} // not stone
		// else if(ap.y > 30.0) {lcol=lerp(vec4(0.887,0.1777,0.59,0.0), vec4(1.9,1.9,1.9,1.0), (vec4(ap.y,ap.y,ap.y,1.0)-3.0)/10.0); } //snow

		vcol = light(eye, ap, an, M,N, lpos, lcol, lpow, spec, diff, shin);

		// else if(ap.y < 1.5) lcol = vec4(1.987,1.877,1.959,0.0);
		vp = ap;
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

	uniform vec4 ppos;
	// uniform vec4 eye;
	uniform vec4 focus;

	varying vec4 vp;
	varying vec4 vcol;
	void main(void){

		vec4 col = pow(vcol, vec4(2.5) );
		// if(vp.y < .1) col = vec4(0.3,0.67,0.89,0.0) * vcol; // water
		// // if(vp.y < pow(length(vp.xz)/2.0,3.0)) col = vec4(0.3,0.67,0.89,0.0) * vcol; // water
		// else if(vp.y < .5) col = vec4(0.987,0.877,0.959,0.0) *vcol; //sand
		// else if(vp.y > 2.5) col = vec4(0.9,0.9,0.9,0.0); //snow
		vec2 stc = ((vp.xz-focus.xz)/64.0) + vec2(0.5);
		// vec4 tcol = (texture2D(t, (vp.xz/*-focus.xz*/)/64.0)) * col;
		vec4 stcol = texture2D(shadow_map, stc);
		// vec4 tcol = texture2D(tex, (vp.xz/*-focus.xz*/));
		vec4 tcol = texture2D(tex, (vp.xz-focus.xz));
		tcol *=  (col);
		tcol += col-stcol;
		// tcol = tcol - stcol;


		// vec4 tcol = col;
		// float dist = length(vp.xz-eye.xz);
		// float fogfactor = (31.0 - dist)/(31.0-20.0); // player center 64 block fog dist

		// float dist = length(vp.xz-focus.xz);
		// float fogfactor = (31.0 - dist)/(31.0-20.0); // player center 64 block fog dist
		float dist = length(vp.xz-ppos.xz);
		float fogfactor = (40.0 - dist)/(40.0-30.0); // transposed focus
		fogfactor = clamp( fogfactor, 0.0, 1.0);
		tcol = mix(vec4(.0, .0, .0, 0.0), tcol, fogfactor); // fog
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
