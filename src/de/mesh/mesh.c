#include "mesh.h"

dmesh* dmesh_open(char* mesh_path, char* tex_path){ if(!mesh_path) return NULL;
	dvao* v = dmesh_open_obj(mesh_path);
	if(!v){ DE_LOG("couldnt open .obj file: %s", mesh_path); return NULL;}
	dtex* t = dtex_open(tex_path);
	if(!t){ DE_LOG("couldnt open texture file: \"%s\" for obj file: %s ", tex_path, mesh_path);}
	return dmesh_new(v, t);
}

dmesh* dmesh_new(dvao* v, dtex* t){ if(!v) return NULL;
	dmesh* m = malloc(sizeof(struct dmesh));
	m->vao = v;
	m->tex = t;
	m->pos = (v4f){0,0,0,1};
	m->rot = (v4f){0,0,0,1};
	m->sca = (v4f){1,1,1,1};
	m->pos = (v4f){0,0,0,1};
	m->m = m4f_id();
	m->n = m4f_id();
	m->mvp = m4f_id();
	return m;
}

void dmesh_free(dmesh* m){ if(!m) return;
	dvao_free(m->vao);
	dtex_free(m->tex);
	free(m);
}

















void dmesh_update(dmesh* m){ if(!m) return;
	m->m = m4f_model(m->pos, m->rot, m->sca);
	// m4f M = m4f_id();
	// M = m4f_translate(M, m->pos);
	// if(m->rot[2]) M = m4f_rotate_z(M, m->rot[2]);
	// if(m->rot[1]) M = m4f_rotate_y(M, m->rot[1]);
	// if(m->rot[0]) M = m4f_rotate_x(M, m->rot[0]);
	// M = m4f_scale(M, m->sca);
	// m->m = M;

	// M[15]=1; M[3]=M[7]=M[11]=M[14]=M[13]=M[12]=0;
	// m->n = m4f_transpose(m4f_inverse(M));

	m->n = m4f_timodel(m->m);

	m->mvp = m4f_mul(m->m, De.cam.vp);
}



















// mesh shader

dshd* Dmesh_shd = NULL;
char* Dmesh_shd_vsrc, *Dmesh_shd_fsrc;
static struct Dmesh_shd_unif
{
	m4f n, m, mvp;
	int tex_unit;
	v4f mdif, mspec, eye;
	float mshin;
	// lights
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
}



void dmesh_shd_unif_update(dmesh* m){ if(!m) return;
	Dmesh_shd_unif.n = m->n;
	Dmesh_shd_unif.m = m->m;
	Dmesh_shd_unif.mvp = m->mvp;
	Dmesh_shd_unif.tex_unit = 0;
	Dmesh_shd_unif.mdif = (v4f){1,1,1,1};
	Dmesh_shd_unif.mspec = (v4f){1,1,1,1};
	Dmesh_shd_unif.mshin = 8.0;
	Dmesh_shd_unif.eye = De.cam.pos;
}



void dmesh_draw(dmesh* m){ if(!m) return;
	dmesh_update(m);
	if(!Dmesh_shd) dmesh_shd_init();
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
	// uniform vec4 lp1;
	// uniform vec4 lc1;
	// uniform float li1;
	// uniform vec4 lp2;
	// uniform vec4 lc2;
	// uniform float li2;

//hard light
	vec4 lpos = eye;
	vec4 lcol = vec4(1.0, 1.0, 1.0, 1.0);
	float lpow = 0.978;
	// vec4 spec = vec4(0.95, 0.95, 0.95, 1.0);
	// vec4 dif = vec4(0.95, 0.94, 0.95, 1.0);
	// float shin = 16.0;


	vec4 light( // directional light
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
	\t    vec4 ldiff = diff * lcol * lpow * cosTheta;                      \n
	\t    if(cosTheta > 0.0)                                               \n
	\t\t        lspec = ( spec  *  lcol  *  lpow  *                        \n
	\t\t\t            pow(max(dot(r,v), 0.0),  shininess ));               \n
	\t    return ldiff + lspec;                                            \n
	}
	// TODO multiply lspec by clamped costheta to avoid if guard

	varying vec4 vcol;
	varying vec2 vtc;

	void main(void){
		vtc = atc;

		vcol = light(eye, ap, an, M,N, lpos, lcol, lpow, mspec, mdif, mshin);
		gl_Position = MVP * ap;
	}
);








char* Dmesh_shd_fsrc = DE_SHD_HEADERF QUOTE(
	uniform sampler2D tex;
	varying vec4 vcol;
	varying vec2 vtc;

	void main(void){
		vec4 tcol = texture2D(tex, vtc);
		if(tcol.a == 0.0) discard;
		gl_FragColor = tcol * vcol;
	}
);
