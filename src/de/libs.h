#ifndef __DE_LIBS_H__
#define __DE_LIBS_H__

#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include <math.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdint.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>  /*mkdir*/


typedef unsigned int uint;
typedef unsigned short ushort;

#if defined (ANDROID)
	#include <jni.h>
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
	// #include <GLES/gl.h>
	// #include <GLES/glext.h>
	#include <android/log.h>
	#include "../../de_android/jni/SDL2/include/SDL.h"
	#include "../../de_android/jni/SDL2_image/SDL_image.h"
	#include "../../de_android/jni/SDL2_mixer/SDL_mixer.h"
	#include "../../de_android/jni/SDL2_net/SDL_net.h"
	#include "../../de_android/jni/SDL2_ttf/SDL_ttf.h"
	#include "android/glext.h"
#else
	#ifdef _WIN32
		#include <GL/glew.h>
		#include <GL/gl.h>
	#else
	
		#include <GLES3/gl3.h>
		// #include <GLES2/gl2.h>
		// #include <GLES2/gl2ext.h>
		// #include <GLES/gl.h>
		// #include <GLES/glext.h>

// extern PFNGLBINDVERTEXARRAYOESPROC     glBindVertexArray;
// extern PFNGLDELETEVERTEXARRAYSOESPROC  glDeleteVertexArrays;
// extern PFNGLGENVERTEXARRAYSOESPROC     glGenVertexArrays;
// extern PFNGLISVERTEXARRAYOESPROC       glIsVertexArray;

		// #include "android/glext.h"
	#endif

	#include <SDL2/SDL.h>
	#include <SDL2/SDL_mixer.h>
	// #include <SDL2/SDL_net.h>
	// #include <SDL2/SDL_ttf.h>
	// #include <SDL2/SDL_image.h>
#endif

#ifndef __EMSCRIPTEN__
#define EMSCRIPTEN_KEEPALIVE
#else
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

// ensure 'SDL_main' is defined and 'main' is not
#ifdef main
	#undef main
#endif
#ifndef ANDROID
	#ifndef SDL_main
		#define SDL_main main
	#endif
#endif

#endif
