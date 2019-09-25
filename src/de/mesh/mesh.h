#ifndef __DE_MESH_H__
#define __DE_MESH_H__

#include "../de.h"
#include "obj_loader.h"

// mesh vertices consist of (in order)
// 	v4f position, v4f normal, v2f tex_coords

typedef struct dmesh{
	v4f pos;
	v4f rot;
	v4f sca;
	m4f m;
	m4f n;
	// m4f vp; // view * projection
	m4f mvp;
	dvao* vao;
	dtex* tex;
	// dshd* shd;
	//tmp lights
	v4f lp1;
	v4f lc1;
	float li1;
	v4f lp2;
	v4f lc2;
	float li2;

} dmesh;

dmesh* dmesh_open(char* mesh_path, char* tex_path); //tex is optional NULL
//expected vao format
//	attribute vec4 apos;
//	attribute vec4 anor;
//	attribute vec2 atexCoord;
dmesh* dmesh_new(dvao*, dtex*); //tex is optional NULL
void dmesh_update(dmesh*);

void dmesh_free(dmesh*);

void dmesh_draw(dmesh*);



#endif
