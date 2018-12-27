#ifndef __DE_ANDROID_GLEXT_H__
#define __DE_ANDROID_GLEXT_H__

#if defined (ANDROID)
#include <GLES2/gl2ext.h>
extern PFNGLBINDVERTEXARRAYOESPROC     glBindVertexArray;
extern PFNGLDELETEVERTEXARRAYSOESPROC  glDeleteVertexArrays;
extern PFNGLGENVERTEXARRAYSOESPROC     glGenVertexArrays;
extern PFNGLISVERTEXARRAYOESPROC       glIsVertexArray;
#endif

void dandroid_glext_init(void);
void dandroid_glext_quit(void);

#endif
