#include "glext.h"

// #ifndef _WIN32 || __EMSCRIPTEN__
#if defined (ANDROID)

#include "../libs.h"
#include "../common/log.h"
#include "../de_struct.h" // De.glext_has_vao
#include <dlfcn.h>

PFNGLBINDVERTEXARRAYOESPROC     glBindVertexArray;
PFNGLDELETEVERTEXARRAYSOESPROC  glDeleteVertexArrays;
PFNGLGENVERTEXARRAYSOESPROC     glGenVertexArrays;
PFNGLISVERTEXARRAYOESPROC       glIsVertexArray;

static char _inited = 0;
static void* _libhandle;

void dandroid_glext_init(void){ if (_inited) return; _inited = 1;

	// if(De.gles_v2)
	_libhandle = dlopen( "libGLESv2.so", RTLD_LAZY | RTLD_GLOBAL );
	// else
	if(!_libhandle)
		_libhandle = dlopen( "libGLESv3.so", RTLD_LAZY | RTLD_GLOBAL );

	if(!_libhandle) DE_LOGE("libGLESv2/3.so not found!");
//VAO
#ifndef ANDROID
	glBindVertexArray = (PFNGLBINDVERTEXARRAYOESPROC)dlsym( _libhandle,
		"glBindVertexArray");
	glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSOESPROC)dlsym( _libhandle,
		"glDeleteVertexArrays");
	glGenVertexArrays = (PFNGLGENVERTEXARRAYSOESPROC)dlsym( _libhandle,
		"glGenVertexArrays");
	glIsVertexArray = (PFNGLISVERTEXARRAYOESPROC)dlsym(_libhandle,
		"glIsVertexArray");
#else
	glBindVertexArray = (PFNGLBINDVERTEXARRAYOESPROC)dlsym( _libhandle,
		"glBindVertexArrayOES");
	glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSOESPROC)dlsym( _libhandle,
		"glDeleteVertexArraysOES");
	glGenVertexArrays = (PFNGLGENVERTEXARRAYSOESPROC)dlsym( _libhandle,
		"glGenVertexArraysOES");
	glIsVertexArray = (PFNGLISVERTEXARRAYOESPROC)dlsym(_libhandle,
		"glIsVertexArrayOES");
#endif

	if(!glBindVertexArray || !glDeleteVertexArrays || !glGenVertexArrays || !glIsVertexArray)
		De.glext_has_vao = 0;
	else
		De.glext_has_vao = 1;

}



void dandroid_glext_quit(void){
	if (_inited) dlclose(_libhandle);
	_inited = 0;
}



#else // windows || emcc

void dandroid_glext_init(void){return;}
void dandroid_glext_quit(void){return;}

#endif
