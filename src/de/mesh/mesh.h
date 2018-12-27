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
	dshd* shd;
	//lights
} dmesh;

dmesh* dmesh_open(char* mesh_path, char* tex_path); //tex is optional NULL
dmesh* dmesh_new(dvao*, dtex*); //tex is optional NULL

void dmesh_free(dmesh*);

void dmesh_draw(dmesh*);



#endif
