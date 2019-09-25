#include "mesh.h"

dmesh* dmesh_open(char* mesh_path, char* tex_path){ if(!mesh_path) return NULL;
	dvao* v = dmesh_open_obj(mesh_path);
	if(!v){ DE_LOG("couldnt open .obj file: %s", mesh_path); return NULL;}
	dtex* t=NULL;
	if(tex_path) {
		t = dtex_open(tex_path);
		if(!t) DE_LOG("couldnt open texture file: \"%s\" for obj file: %s ", tex_path, mesh_path);
	}
	return dmesh_new(v, t);
}
dmesh dmesh_default;
dmesh* dmesh_new(dvao* v, dtex* t){ if(!v) return NULL;
	dmesh* m = malloc(sizeof(struct dmesh));
	dmesh_default = (dmesh){
		.pos = (v4f){0,0,0,1},
		.rot = (v4f){0,0,0,1},
		.sca = (v4f){1,1,1,1},
		.m = M4F_ID,
		.n = M4F_ID,
		.mvp = M4F_ID,
		.lp1 = (v4f){0,0,0,1},
		.lc1 = (v4f){1,1,1,0},
		.li1 = 1,
		.lp2 = (v4f){0,0,0,1},
		.lc2 = (v4f){1,1,1,0},
		.li2 = 1,
	};
	*m=dmesh_default;
	m->vao = v;
	m->tex = t;

// 	m->pos = (v4f){0,0,0,1};
// 	m->rot = (v4f){0,0,0,1};
// 	m->sca = (v4f){1,1,1,1};
// 	m->m = m4f_id();
// 	m->n = m4f_id();
// 	m->mvp = m4f_id();
// //tmp light
// 	m->lp1 = (v4f){0,0,0,1};
// 	m->lc1 = (v4f){1,1,1,0};
// 	m->li1 = 1;

// 	m->lp2 = (v4f){0,0,0,1};
// 	m->lc2 = (v4f){1,1,1,0};
// 	m->li2 = 1;

	return m;
}

void dmesh_free(dmesh* m){ if(!m) return;
	dvao_free(m->vao);
	dtex_free(m->tex);
	free(m);
}

void dmesh_tex(dmesh* m, dtex* t){ if(!m || !t) return;
	dtex_free(m->tex);
	// m->tex = dtex_ref(t);
	m->tex = t;
}

void dmesh_update(dmesh* m){ if(!m) return;
	m->m = m4f_model(m->pos, m->rot, m->sca);
	// m->m = m4f_model(v4f_0, m->rot, m->sca);
	m->n = m4f_timodel(m->m);
	// m->n = m4f_transpose(m4f_inverse(m->m));
	m->mvp = m4f_mul(m->m, De.cam.vp);
}



















// mesh shader

dshd* Dmesh_shd = NULL;
char* Dmesh_shd_vsrc, *Dmesh_shd_fsrc;
static struct Dmesh_shd_unif{
	m4f n, m, mvp;
	int tex_unit;
	v4f mdif, mspec, eye;
	float mshin;
	// lights
	v4f lp1, lc1;
	v4f lp2, lc2;
	float li1, li2;
} Dmesh_shd_unif;


void dmesh_shd_init(void){ if(Dmesh_shd) return;
	Dmesh_shd = dshd_new(Dmesh_shd_vsrc, Dmesh_shd_fsrc);
	if(!Dmesh_shd){ DE_LOG("default shader not loaded"); return;}
	dshd_unif(Dmesh_shd, "M", &Dmesh_shd_unif.m);
	dshd_unif(Dmesh_shd, "N", &Dmesh_shd_unif.n);
	dshd_unif(Dmesh_shd, "MVP", &Dmesh_shd_unif.mvp);
	dshd_unif(Dmesh_shd, "tex", &Dmesh_shd_unif.tex_unit);
	dshd_unif(Dmesh_shd, "mdif", &Dmesh_shd_unif.mdif);
	dshd_unif(Dmesh_shd, "mspec", &Dmesh_shd_unif.mspec);
	dshd_unif(Dmesh_shd, "mshin", &Dmesh_shd_unif.mshin);
	dshd_unif(Dmesh_shd, "eye", &Dmesh_shd_unif.eye);
	dshd_unif(Dmesh_shd, "lp1", &Dmesh_shd_unif.lp1);
	dshd_unif(Dmesh_shd, "lc1", &Dmesh_shd_unif.lc1);
	dshd_unif(Dmesh_shd, "li1", &Dmesh_shd_unif.li1);
	dshd_unif(Dmesh_shd, "lp2", &Dmesh_shd_unif.lp2);
	dshd_unif(Dmesh_shd, "lc2", &Dmesh_shd_unif.lc2);
	dshd_unif(Dmesh_shd, "li2", &Dmesh_shd_unif.li2);
}



void dmesh_shd_unif_update(dmesh* m){ if(!m) return;
	Dmesh_shd_unif.n = m->n;
	Dmesh_shd_unif.m = m->m;
	Dmesh_shd_unif.mvp = m->mvp;
	Dmesh_shd_unif.tex_unit = 0;
	Dmesh_shd_unif.mdif = (v4f){1,1,1,1};
	Dmesh_shd_unif.mspec = (v4f){1,1,1,1};
	Dmesh_shd_unif.mshin = 16.0;
	Dmesh_shd_unif.eye = De.cam.pos;
//tmp light
	Dmesh_shd_unif.lp1 = m->lp1;
	Dmesh_shd_unif.lc1 = m->lc1;
	Dmesh_shd_unif.li1 = m->li1;
	Dmesh_shd_unif.lp2 = m->lp2;
	Dmesh_shd_unif.lc2 = m->lc2;
	Dmesh_shd_unif.li2 = m->li2;
}



void dmesh_draw(dmesh* m){ if(!m) return;
	if(!Dmesh_shd) dmesh_shd_init();
	dmesh_update(m);
	dmesh_shd_unif_update(m);
	dshd_bind(Dmesh_shd);
	dshd_update(Dmesh_shd);
	dtex_bind(m->tex, Dmesh_shd_unif.tex_unit);
	dvao_draw(m->vao, 0,0,0);

}































//mesh shader


char* Dmesh_shd_vsrc = DE_SHD_HEADERV QUOTE(
	attribute vec4 ap;
	attribute vec4 an;
	attribute vec2 atc;
	//view
	uniform mat4 MVP;
	uniform mat4 M;
	uniform mat4 N;
	uniform vec4 eye;
	//material
	uniform vec4 mdif;
	uniform vec4 mspec;
	uniform float mshin;
	// TODO light uniforms
	// uniform vec4 lamb;
	uniform vec4 lp1;
	uniform vec4 lc1;
	uniform float li1;
	uniform vec4 lp2;
	uniform vec4 lc2;
	uniform float li2;


	vec4 light( // directional light
		vec4 eye, vec4 vpos, vec4 vnor, mat4 M, mat4 N,
		vec4 lpos, vec4 lcol, float lpow,
		vec4 spec, vec4 diff, float shininess
	){
	\t    vec4 inor = normalize(N * vnor);                                 \n
	\t    vec4 lspec = vec4(0.0, 0.0, 0.0, 1.0);                           \n
	\t    vec4 v = normalize( eye - M * vpos);                             \n
	\t    vec4 ldirv = lpos - M*vpos;                                        \n
	\t    vec4 ldir = normalize(ldirv);                                    \n
	\t    vec4 r = reflect(-ldir, inor);                                   \n
	\t    float cosTheta = max(dot(ldir, inor), 0.0);                      \n
	\t    float dist2 = dot(ldirv, ldirv);                         \n\t
	\t    if( lpos.w == 0.0 ) dist2=1.0;
	\t    vec4 ldiff = diff * lcol * lpow * cosTheta / dist2;                      \n
	\t    if(cosTheta > 0.0)                                               \n
	\t\t        lspec = ( spec  *  lcol  *  lpow  *                        \n
	// \t\t        lspec = clamp(cosTheta*1000.0, 0.0, 1.0)*( spec  *  lcol  *  lpow  *                        \n
	\t\t\t            pow(max(dot(r,v), 0.0),  shininess ))/dist2;               \n
	\t    return ldiff + lspec;                                            \n
	}
	// TODO multiply lspec by clamped costheta to avoid if guard

	varying vec4 vcol;
	varying vec2 vtc;

	void main(void){
//hard light
		// vec4 lpos = vec4(0.,0.,0.,1.);
		// vec4 lcol = vec4(1.0, 1.0, 1.0, 1.0);
		// float lpow = 1.978;
		// vec4 spec = vec4(0.95, 0.95, 0.95, 1.0);
		// vec4 dif = vec4(0.95, 0.94, 0.95, 1.0);
		// float shin = 16.0;



		vtc = atc;

		// vcol = light(eye, ap, an, M,N, lpos, lcol, lpow, mspec, mdif, mshin);
		vcol = light(eye, ap, an, M,N, lp1, lc1, li1, mspec, mdif, mshin);
		vcol += light(eye, ap, an, M,N, lp2, lc2, li2, mspec, mdif, mshin);
		vcol.a=1.0;
		gl_Position = MVP * ap;
	}
);








char* Dmesh_shd_fsrc = DE_SHD_HEADERF QUOTE(
	uniform sampler2D tex;
	varying vec4 vcol;
	varying vec2 vtc;

	void main(void){
		vec4 tcol = texture2D(tex, vtc);
		// if(tcol.a == 0.0) discard;
		gl_FragColor = tcol * vcol;
	}
);
