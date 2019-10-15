#include "../../de/de.h"
#include <stdbool.h>
#include "points.h"
#include "mgame.h"
#include "hmap.h" // hmap_eval
// #define num_mpts 1024*2
static bool _initialized=false;
static v4f* vbuf=NULL;
static v4f* nbuf=NULL;
static dshd* pts_shd=NULL;
static char* pts_vsrc;
static char* pts_fsrc;

void pts_init(void){ if(_initialized) return; _initialized=true;
	vbuf=malloc(num_mpts * sizeof(v4f) * 3);
	nbuf=malloc(num_mpts * sizeof(v4f) * 3);
	pts_shd=dshd_new(pts_vsrc, pts_fsrc);
	dshd_unif(pts_shd, "mvp", &De.cam.vp);
	dshd_unif(pts_shd, "eye", &De.cam.pos);
	dshd_unif(pts_shd, "sunp", &G.sunpos);
	dshd_unif(pts_shd, "sunc", &G.suncol);
}
void pts_quit(void){ if(!_initialized) return; _initialized=false;
	free(vbuf); vbuf=NULL;
	free(nbuf); nbuf=NULL;
	dshd_free(pts_shd);
}

mpts* pts_new(void){ assert(_initialized);
	mpts* pts=malloc(sizeof(mpts));
	pts->p=malloc(sizeof(struct mpt) * num_mpts);
	pts->pivot=0;

	for(int i = 0; i < num_mpts; i++){
		pts->p[i].pos = v4f_0;
		pts->p[i].vel = v4f_0;
		pts->p[i].acc = v4f_0;
		pts->p[i].rot = v4f_0;
		// vbuf[i]=v4f_0;
		// nbuf[i]=v4f_0;
		pts->p[i].scale = 0;

	}
	// void * vecs[]={vbuf, nbuf};
	pts->vao=dvao_newr("f4p; f4n", NULL, num_mpts*3, GL_TRIANGLES);
	pts->vao->vbos[0]->usage = GL_STREAM_DRAW;
	pts->vao->vbos[1]->usage = GL_STREAM_DRAW;
	return pts;
}

void pts_free(mpts* pts){ if(!pts) return;
	free(pts->p);pts->p=NULL;
	free(pts);
}


void pts_update(mpts* pts){ if(!pts || !pts->p) return;
	srand(G.frame);
	float nx,ny,nz;
	float h;
	// float scale = 19;
	for(int i=0; i<num_mpts; i++){
		if(pts->p[i].scale == 0) continue;
		pts->p[i].vel /= 1.03f;
		
		//attract
		if( v3f_len2 (pts->p[i].pos - G.player.pos) < 5 /*&& v4f_len2(pts->p[i].vel)<1*/) { // attract
			pts->p[i].vel= (pts->p[i].vel * 9 + v3f_normalize( G.player.pos - pts->p[i].pos ) *4)/10;
		}
		pts->p[i].vel += pts->p[i].acc * G.dt;
		pts->p[i].pos += pts->p[i].vel * G.dt;

		
		pts->p[i].rot[1] += 0.1*v3f_len2(pts->p[i].vel /** (v4f){1,0,1,0}*/);
		//pickup
		if( v3f_len2 (pts->p[i].pos - G.player.pos) < .02f) {
			// pts->p[i].pos= v4f_0;
			// pts->p[i].pos= (v4f_rand()-(v4f){.5,0,.5,0})*100;
			// pts->p[i].pos= (v3f_rand()-(v4f){.5,-50,.5,0})*(v4f){4000, 40, 4000,1};
			// pts->p[i].pos[1] += 50;
			pts->p[i].scale = 0;
			pts->p[i].acc[1] = 0;
		}

		//collide
		h = hmap_evald(G.hmap0, pts->p[i].pos[0], pts->p[i].pos[2], &nx,&ny,&nz);

	// if(h + in->rad + G.player.height/2 > in->pos[1]){
		if(h + pts->p[i].scale > pts->p[i].pos[1]){
			pts->p[i].pos[1] = h + pts->p[i].scale;
			pts->p[i].rot[0] += nx/10.0;
			pts->p[i].rot[1] += ny/10.0;
			pts->p[i].rot[2] += nz/10.0;

			// pts->p[i]->vel[1] *= -1;

			if(nx > .75) pts->p[i].vel[0] += nx /** G.dt*/; //else in->acc[0]=0;
			if(nx <-.75) pts->p[i].vel[0] += nx /** G.dt*/; //else in->acc[0]=0;
			if(nz > .75) pts->p[i].vel[2] += nz /** G.dt*/; //else in->acc[2]=0;
			if(nz <-.75) pts->p[i].vel[2] += nz /** G.dt*/; //else in->acc[2]=0;

			pts->p[i].acc[1] = 0;
		} else 
			pts->p[i].acc[1] = -2.2;
		
		m4f rot = m4f_rotation(pts->p[i].rot);

		vbuf[i*3+0] = pts->p[i].pos + v4f_mtmul( (v4f){0,1,0,0}  *pts->p[i].scale, rot); vbuf[i*3+0][3]=1;
		vbuf[i*3+1] = pts->p[i].pos + v4f_mtmul( (v4f){-1,-1,0,0} *pts->p[i].scale, rot); vbuf[i*3+1][3]=1;
		vbuf[i*3+2] = pts->p[i].pos + v4f_mtmul( (v4f){1,-1,0,0}*pts->p[i].scale, rot); vbuf[i*3+2][3]=1;
		v4f nor = v3f_normalize( v4f_mtmul( (v4f){0,0,-1,0}, rot ));
		
		nbuf[i*3+0] = nor;
		nbuf[i*3+1] = nor;
		nbuf[i*3+2] = nor;

		// nbuf[i*3+0] = (v4f){.99,.95, (rand()%1000)/1000.f,1}; // gold
		// nbuf[i*3+1] = (v4f){.99,.95, (rand()%1000)/1000.f,1}; // gold
		// nbuf[i*3+2] = (v4f){.99,.95, (rand()%1000)/1000.f,1}; // gold
		// nbuf[i*3+0] = (v4f){.1, (rand()%1000)/1000.f, .99 , v4f_len2(G.player.pos - pts->p[i].pos)*2}; // blue
		// nbuf[i*3+1] = (v4f){.1, (rand()%1000)/1000.f, .99 , v4f_len2(G.player.pos - pts->p[i].pos)*2}; // blue
		// nbuf[i*3+2] = (v4f){.1, (rand()%1000)/1000.f, .99 , v4f_len2(G.player.pos - pts->p[i].pos)*2}; // blue
		// nbuf[i*3+0] = (v4f){ .99, .1, (rand()%1000)/1000.f , v4f_len2(G.player.pos - pts->p[i].pos)*2}; // pink
		// nbuf[i*3+1] = (v4f){ .99, .1, (rand()%1000)/1000.f , v4f_len2(G.player.pos - pts->p[i].pos)*2}; // pink
		// nbuf[i*3+2] = (v4f){ .99, .1, (rand()%1000)/1000.f , v4f_len2(G.player.pos - pts->p[i].pos)*2}; // pink
		// nbuf[i*3+0] = (v4f){ .99, (rand()%1000)/1000.f ,.1, v4f_len2(G.player.pos - pts->p[i].pos)*5}; // orange
		// nbuf[i*3+1] = (v4f){ .99, (rand()%1000)/1000.f ,.1, v4f_len2(G.player.pos - pts->p[i].pos)*5}; // orange
		// nbuf[i*3+2] = (v4f){ .99, (rand()%1000)/1000.f ,.1, v4f_len2(G.player.pos - pts->p[i].pos)*5}; // orange
	}
	dvao_setr(pts->vao, 0, vbuf, 0, num_mpts*3);
	dvao_setr(pts->vao, 1, nbuf, 0, num_mpts*3);
	dvao_update(pts->vao);
}


void pts_draw(mpts* pts){ if(!pts || !pts->p) return;
	dcullf(0);
	dblend(1);
	// ddepth(0,0);
	dshd_bind(pts_shd);
	dshd_update(pts_shd);
	dvao_draw(pts->vao, 0,0, GL_TRIANGLES);
}


void pts_add(mpts* pts, v4f pos, int count){ assert(pts && pts->p);
	srand(G.frame);
	pos[3]=1;
	for(int i=0; i<count; i++){
		pts->p[ pts->pivot ].pos = pos;
		// pts->p[ pts->pivot ].col = v4c_rand();
		// pts->p[ pts->pivot ].col[3] = 128;
		pts->p[ pts->pivot ].rot = v3f_rand();
		pts->p[ pts->pivot ].vel = v3f_normalize(v3f_rand()-.5)*5;
		pts->p[ pts->pivot ].vel[3] = 0;
		pts->p[ pts->pivot ].acc = v4f_0;
		pts->p[ pts->pivot ].acc[1] = -2.2;
		pts->p[ pts->pivot ].scale = .029f;


		pts->pivot = (pts->pivot+1)%num_mpts;
	}
}






static char* pts_vsrc=DSHD_QUOTE(
	attribute vec4 pos;
	attribute vec4 nor;
	uniform mat4 mvp;
	uniform vec4 eye;
	uniform vec4 sunp;
	uniform vec4 sunc;
	// DSHDLIB_LIGHT;
	varying vec4 vcol;

	vec4 light(
		vec4 eye, vec4 vpos, vec4 vnor, mat4 M, mat4 N,
		vec4 lpos, vec4 lcol, float lpow,
		vec4 diff, vec4 spec, float shininess
	){
	    vec4 inor = normalize(N * vnor);                                 
	    vec4 lspec = vec4(0.0, 0.0, 0.0, 1.0);                           
	    vec4 v = normalize( eye - M * vpos);                             
	    vec4 ldirv = lpos - vpos;                                        
	    vec4 ldir = normalize(ldirv);                                    
	    vec4 r = reflect(-ldir, inor);                                   
	    // float cosTheta = max(dot(ldir, inor), 0.0);                      
	    float cosTheta = abs(dot(ldir, inor));                      
	    // float dist2 = lpos.w > 0.5 ? pow(dot(ldirv, ldirv), .2) : 1.0;                         
	    float dist2 = 1.0;                         
	    vec4 ldiff = diff * lcol * lpow * cosTheta/dist2;              
	    if(cosTheta > 0.1)                                               
	///        lspec = ( spec  *  lcol  *  lpow  *                        
	        lspec = /*cosTheta **/ ( spec  *  lcol  *  lpow  *                        
	            pow(max(dot(r,v), 0.0),  shininess ))/dist2;  
	            // pow(abs(dot(r,v)),  shininess ))/dist2;  
	    return ldiff + lspec ;                                            
}

	mat4 M = mat4(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);

	vec4 mate=vec4(1.0,0.5,0.1,1.0);
	// vec4 lcol=vec4(1.0,0.0,0.0,1.0);
	// vec4 lspc=vec4(0.0,0.0,1.0,1.0);

	void main(void){
		vcol= mate/4.0 + (mate/1.0) * light( eye, pos, nor, M,M, sunp, sunc, 1.0, vec4(1.0), vec4(1.0), 2.0 );
		vcol.a=1.0;
		gl_Position=mvp*pos;
	}
);

static char* pts_fsrc=DSHD_QUOTE(
	varying vec4 vcol;
	void main(void){
		gl_FragColor=vcol;
	}
);
