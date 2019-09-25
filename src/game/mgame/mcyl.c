#include "mcyl.h"
#include "../../de/de.h"
#if 0
typedef struct mcyl{
	dmesh* mesh;
	int x;//slices
	int y;//disks
	float* v;
} mcyl;

// void mcyl_init(void);
// void mcyl_quit(void);

mcyl* mcyl_new(float* mxn, int x, int y);
void mcyl_free(mcyl*);
void mcyl_update(mcyl*);
void mcyl_draw(mcyl*);
void mcyl_rand(mcyl*);
void mcyl_set(mcyl*, int x, int y, float val);
float mcyl_get(mcyl*, int x, int y);
#endif
// dshd* Mcyl_shd=NULL;
// char* mcyl_vsrc;
// char* mcyl_fsrc;
// m4f Mcyl_shd_unif_mvp;
v4c* ttex=NULL;
mcyl* mcyl_new(float* mxn, int x, int y){ assert((x>0) && (y>0));
	if(ttex == NULL) ttex=malloc(sizeof(v4c)*32*32);
	// if(!Mcyl_shd){
	// 	Mcyl_shd=dshd_new(mcyl_vsrc,mcyl_fsrc);
	// 	dshd_unif(Mcyl_shd, "mvp", &Mcyl_shd_unif_mvp);
	// }
	mcyl* ret=malloc(sizeof(mcyl));
	ret->x=x;
	ret->y=y;
	ret->v = malloc(x*y*sizeof(float));
	if (mxn) memcpy(ret->v, mxn, x*y*sizeof(float));
	else 
		for(int i=0; i<x*y; i++) 
			ret->v[i] = ((float)(rand()%1000))/2000.f + .5;
	// else for(int i=0; i<x*y; i++) ret->v[i] = sin(((float)i*5.0)/(x*y));
	// else for(int i=0; i<x*y; i++) ret->v[i] = 1.f;

	for(int i=0; i<32*32; i++)
		// ttex[i]=v3c_rand()+(v4c){0,0,0,64};
		ttex[i]=v3c_rand(), ttex[i][3]=255;
	ret->mesh=dmesh_new( mcyl_vao(ret->v,x,y), dtex_new(ttex ,32,32) );
	// dtex_filter(ret->mesh->tex, GL_NEAREST, GL_NEAREST);
	// ret->vao=mcyl_vao(ret->v, x,y);
	// ret->mesh->shd=Mcyl_shd;
	// dvao_new("f4p,f4n,f2tc", NULL, x*y*6, GL_TRIANGLES);
	// dvao_setr(ret->mesh->vao, 0, verts, 0, x*y*6);
	// free(verts);
	// ret->mesh->pos[1]=4.1;
	// ret->mesh->pos[2]=0;
	// ret->mesh->sca[0]=6;
	// ret->mesh->sca*=-6;
	dmesh_update(ret->mesh);
	// dvao_update(ret->mesh->vao);



	return ret;
}

void mcyl_free(mcyl*m){ assert(m && m->v && m->mesh);
	dmesh_free(m->mesh);
	free(m->v);
	free(m);
}
void mcyl_update(mcyl* m){ assert(m);
	dmesh_update(m->mesh);
	// dvao_update(m->vao);
}

void mcyl_draw(mcyl* m){ assert(m);
	// Mcyl_shd_unif_mvp = m4f_model((v4f){1,4,0,1},(v4f){0,0,0,0},(v4f){1,1,1,0});
	// Mcyl_shd_unif_mvp = m4f_mul(Mcyl_shd_unif_mvp, De.cam.vp);
	dmesh_update(m->mesh);

	// dcullf(0);
	// dcullf('b');
	// dcullf('f');
	ddepth(1,0);
	// dshd_bind(m->shd);
	// dvao_update(m->vao);
	// dshd_update(m->shd);
	// dvao_draw(m->vao, 0,0,0);
	dmesh_draw(m->mesh);
}

void mcyl_rand(mcyl*);
void mcyl_set(mcyl*, int x, int y, float val);
float mcyl_get(mcyl*, int x, int y);


dvao* mcyl_vao(float* mxn, int x, int y){ assert(mxn && (x>0) && (y>0));
	void* b=mcyl_vaob(mxn,x,y,true);
	void* vvs[1]={ b };
	// dvao* ret=dvao_newr("f4p,f4n,f2tc", vvs, x*(y-1)*3*4, GL_TRIANGLES);
	dvao* ret=dvao_newr("f4p,f4n,f2tc", vvs, x*y*3*4, GL_TRIANGLES);
	free(b);
	return ret;
}

void* mcyl_vaob(float* mxn, int x, int y, bool sph){ assert(mxn && (x>0) && (y>0));
	// int strafe = 4*sizeof(float) + 4*sizeof(float) + 2*sizeof(float);
	size_t strafe = sizeof(v4f) + sizeof(v4f) + sizeof(v2f);
	int bytes = x*y* (strafe) * 3*4;
	
	void* verts = malloc( bytes );
	// assign points
	float m; 
	v4f p; 
	m4f rot;
	v4f* ps = malloc(sizeof(v4f)* x*y);
	for (int i = 0; i < y; i++){
		for (int j = 0; j < x; j++){
			m = mxn[i*x + j];
			p=v4f_0;
			p[2] = -m;
			p[3] = 1;
if(sph){
			// p[1] = ((float)i-(y-1)/2.0)/(y-1) ;
			// p[1] = ((float)i)/(y-1) ;
			rot = m4f_rotation_y((((float)j)/(x)) *2.0* PI);
			// rot = m4f_rotate_x(rot, ((float)i-y/2.0) /(y/2.0) *0.5 *PI  );
			rot = m4f_rotate_x(rot, ((((float)i)/(y-1))-0.5) *PI  );
}else{
			p[1] = ((float)i)/(y-1);
			rot = m4f_id();
			rot = m4f_rotation_y((((float)j)/(x)) *2.0* PI);
}
			p=v4f_mmul(p, rot);
			ps[i*x + j] = p;
		}
	}

	int bi=0;
	for (int i = 0; i < y; i++){
	// for (int i = 0; i < y-1; i++){
		for (int j = 0; j < x; j++){
			v4f p0 = ps[i*x          + j];
			v4f p1 = ps[i*x          + ((j+1)%x)];
			// v4f p2 = ps[MIN(i+1,y)*x + j];????
			// v4f p3 = ps[MIN(i+1,y)*x + ((j+1)%x)];????
			v4f p2 = ps[MIN(i+1,y-1)*x + j]; 
			v4f p3 = ps[MIN(i+1,y-1)*x + ((j+1)%x)]; 
			// v4f p0 = ps[i*x         + j];
			// v4f p1 = ps[i*x         + ((j+1)%x)];
			// v4f p2 = ps[((i+1)%y)*x + j];
			// v4f p3 = ps[((i+1)%y)*x + ((j+1)%x)];
			v4f pa = (p0+p1+p2+p3) / 4.0; pa[3]=1;

			// norms
			v4f nb = -v3f_normalize( v3f_cross( p0-pa , p1-pa ) ); nb[3]=0;
			v4f nl = -v3f_normalize( v3f_cross( p2-pa , p0-pa ) ); nl[3]=0;
			v4f nt = -v3f_normalize( v3f_cross( p3-pa , p2-pa ) ); nt[3]=0;
			v4f nr = -v3f_normalize( v3f_cross( p1-pa , p3-pa ) ); nr[3]=0;
			// tex coord
			// if(sph)
			// v2f t0 = { ((float)j) / (x),   ((float)i) / (y-1)};
			// v2f t1 = { ((float)j+1) / (x), ((float)i) / (y-1)};
			// v2f t2 = { ((float)j) / (x),   ((float)i+1) / (y-1)};
			// v2f t3 = { ((float)j+1) / (x), ((float)i+1) / (y-1)};
			v2f t0 = { ((float)j) / (x),   ((float)i) / (y)};
			v2f t1 = { ((float)j+1) / (x), ((float)i) / (y)};
			v2f t2 = { ((float)j) / (x),   ((float)i+1) / (y)};
			v2f t3 = { ((float)j+1) / (x), ((float)i+1) / (y)};
			v2f ta = (t0+t1+t2+t3)/4.0;

//bottom
			memcpy(verts + bi, &pa, sizeof(v4f)); bi+=sizeof(v4f);
			memcpy(verts + bi, &nb, sizeof(v4f)); bi+=sizeof(v4f);
			memcpy(verts + bi, &ta, sizeof(v2f)); bi+=sizeof(v2f);

			memcpy(verts + bi, &p0, sizeof(v4f)); bi+=sizeof(v4f);
			memcpy(verts + bi, &nb, sizeof(v4f)); bi+=sizeof(v4f);
			memcpy(verts + bi, &t0, sizeof(v2f)); bi+=sizeof(v2f);

			memcpy(verts + bi, &p1, sizeof(v4f)); bi+=sizeof(v4f);
			memcpy(verts + bi, &nb, sizeof(v4f)); bi+=sizeof(v4f);
			memcpy(verts + bi, &t1, sizeof(v2f)); bi+=sizeof(v2f);

//left
			memcpy(verts + bi, &pa, sizeof(v4f)); bi+=sizeof(v4f);
			memcpy(verts + bi, &nl, sizeof(v4f)); bi+=sizeof(v4f);
			memcpy(verts + bi, &ta, sizeof(v2f)); bi+=sizeof(v2f);

			memcpy(verts + bi, &p2, sizeof(v4f)); bi+=sizeof(v4f);
			memcpy(verts + bi, &nl, sizeof(v4f)); bi+=sizeof(v4f);
			memcpy(verts + bi, &t2, sizeof(v2f)); bi+=sizeof(v2f);

			memcpy(verts + bi, &p0, sizeof(v4f)); bi+=sizeof(v4f);
			memcpy(verts + bi, &nl, sizeof(v4f)); bi+=sizeof(v4f);
			memcpy(verts + bi, &t0, sizeof(v2f)); bi+=sizeof(v2f);

//top
			memcpy(verts + bi, &pa, sizeof(v4f)); bi+=sizeof(v4f);
			memcpy(verts + bi, &nt, sizeof(v4f)); bi+=sizeof(v4f);
			memcpy(verts + bi, &ta, sizeof(v2f)); bi+=sizeof(v2f);

			memcpy(verts + bi, &p3, sizeof(v4f)); bi+=sizeof(v4f);
			memcpy(verts + bi, &nt, sizeof(v4f)); bi+=sizeof(v4f);
			memcpy(verts + bi, &t3, sizeof(v2f)); bi+=sizeof(v2f);

			memcpy(verts + bi, &p2, sizeof(v4f)); bi+=sizeof(v4f);
			memcpy(verts + bi, &nt, sizeof(v4f)); bi+=sizeof(v4f);
			memcpy(verts + bi, &t2, sizeof(v2f)); bi+=sizeof(v2f);

//right
			memcpy(verts + bi, &pa, sizeof(v4f)); bi+=sizeof(v4f);
			memcpy(verts + bi, &nr, sizeof(v4f)); bi+=sizeof(v4f);
			memcpy(verts + bi, &ta, sizeof(v2f)); bi+=sizeof(v2f);

			memcpy(verts + bi, &p1, sizeof(v4f)); bi+=sizeof(v4f);
			memcpy(verts + bi, &nr, sizeof(v4f)); bi+=sizeof(v4f);
			memcpy(verts + bi, &t1, sizeof(v2f)); bi+=sizeof(v2f);

			memcpy(verts + bi, &p3, sizeof(v4f)); bi+=sizeof(v4f);
			memcpy(verts + bi, &nr, sizeof(v4f)); bi+=sizeof(v4f);
			memcpy(verts + bi, &t3, sizeof(v2f)); bi+=sizeof(v2f);
		}
	}
	free(ps);

	// void* vvs[1]={ verts };
	// dvao* ret=dvao_newr("f4p,f4n,f2tc", vvs, x*(y-1)*3*4, GL_TRIANGLES);
	// dvao* ret=dvao_newr("f4p", vvs, x*(y-1)*3*4, GL_TRIANGLES);
	// dvao_update(ret);
	
	// free(verts);

	return verts;
}



void mcyl_bvo_usage(mcyl* m, GLint u){
	if (u)
		m->mesh->vao->vbos[0]->usage = u;
	else 
		m->mesh->vao->vbos[0]->usage = GL_STREAM_DRAW;
}

void mcyl_regen(mcyl* m, float* v){ assert(m); assert(v);
	// dvao_setr(m->mesh->vao, 0, v, m->x * m->y)
	// dvao_free( m->mesh->vao );
	// m->mesh->vao = mcyl_vao(v, m->x, m->y);
	void* b=mcyl_vaob(v,m->x,m->y,m->sph);
	// void* vvs[1]={ b };
	// dvao* ret=dvao_newr("f4p,f4n,f2tc", vvs, x*(y-1)*3*4, GL_TRIANGLES);
	// dvao_setr(m->mesh->vao, 0, b, 0, m->x*(m->y-1)*3*4);
	dvao_setr(m->mesh->vao, 0, b, 0, m->x*(m->y)*3*4);
	free(b);
	dvao_update(m->mesh->vao);
}





// char* mcyl_vsrc= DSHD_QUOTE(
// 	attribute vec4 pos;	
// 	attribute vec4 nor;	
// 	attribute vec2 tc;	
// 	varying vec4 vcol;
// 	uniform mat4 mvp;
// 	void main(void){
// 		gl_Position=mvp*pos;
// 		vcol=nor*tc.x*tc.y;
// 		// vcol=vec4(1.0,1.0,1.0,1.0);
// 		vcol.a=1.0;
// 	}
// );

// char* mcyl_fsrc= DSHD_QUOTE(
// 	varying vec4 vcol;
// 	void main(void){
// 		gl_FragColor=vcol;
// 	}
// );