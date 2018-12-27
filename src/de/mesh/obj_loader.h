#ifndef __DE_LOADER_OBJ_H__
#define __DE_LOADER_OBJ_H__

// load format must be
// Z forward, Y up, single object, no edges, normal, uv, ,no mtl, triangulated

// #include "../libs.h"
#include "../gl/vao.h"

// dvao* dload_obj(char* file);
dvao* dmesh_open_obj(char* file);

#endif
