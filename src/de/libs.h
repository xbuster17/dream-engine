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
#include <stdint.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>  /*mkdir*/
typedef unsigned int uint;
typedef unsigned short ushort;

/* SDL gl */
#if defined (ANDROID)
	#include <jni.h>
	#include <GLES2/gl2.h>
	#include <GLES2/gl2ext.h>
	#include <GLES/gl.h>
	#include <GLES/glext.h>
	#include <android/log.h>
	#include "../../android/jni/SDL2/include/SDL.h"
	#include "../../android/jni/SDL2_image/SDL_image.h"
	#include "../../android/jni/SDL2_mixer/SDL_mixer.h"
	#include "../../android/jni/SDL2_net/SDL_net.h"
	#include "../../android/jni/SDL2_ttf/SDL_ttf.h"

	/* vao extension */
	#include "android/glext.h"
	// extern PFNGLBINDVERTEXARRAYOESPROC     glBindVertexArray;
	// extern PFNGLDELETEVERTEXARRAYSOESPROC  glDeleteVertexArrays;
	// extern PFNGLGENVERTEXARRAYSOESPROC     glGenVertexArrays;
	// extern PFNGLISVERTEXARRAYOESPROC       glIsVertexArray;
#else
	#include <GL/glew.h>
	#include <GL/gl.h>
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
	#include <SDL2/SDL_mixer.h>
	#include <SDL2/SDL_net.h>
	#include <SDL2/SDL_ttf.h>
#endif


#ifdef main
	#undef main
#endif
#ifndef ANDROID
	#ifndef SDL_main
		#define SDL_main main
	#endif
#endif

#endif
