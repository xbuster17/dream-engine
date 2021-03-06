#ifndef __DE_GL_TEXTURE_H__
#define __DE_GL_TEXTURE_H__

#include "../libs.h"
#include "../conf.h"
#include "../common/common.h"

typedef struct dtex{
	GLuint id;
	int ref;
	int x;
	int y;
	// bool is_cube; // todo
	// GLenum type;   // default GL_TEXTURE_2D
	GLenum min;    // default GL_LINEAR
	GLenum mag;    // default GL_LINEAR
	GLenum wrap_s; // default GL_REPEAT
	GLenum wrap_t; // default GL_REPEAT
} dtex;

dtex* dtex_open(char* file_path);
dtex* dtex_new(v4c* src, int x, int y); // unsigned byte rgba
dtex* dtex_newf(v4c* pixels, int x, int y, GLenum format);
dtex* dtex_newt(v4c* pixels, int x, int y, GLenum internal_format, GLenum format, GLenum type);

void dtex_free(dtex*);
dtex* dtex_ref(dtex*);

void dtex_bind(dtex*, int unit); // NULL => unbind
dtex* dtex_from_surface(SDL_Surface* sur);

void dtex_set(dtex* tex, v4c* pixels, int x, int y);
void dtex_filter(dtex* tex, GLenum min, GLenum mag);
void dtex_wrap(dtex* tex, GLenum wrap_s, GLenum wrap_t);

void dtex_resize(dtex* tex, int x, int y);

//todo
dtex* dtex_new_cube(dtex* t[6], int x, int y); // top,bottom,left,right,front,back todo


void dtex_draw(dtex*);
void dtex_drawa(dtex*, float alpha);


typedef struct dfbo{
	GLuint id;
	int ref;
	GLuint rbo;
	int x,y;
	float res;
	dtex* color;
	dtex* depth; // can be null?
} dfbo;

dfbo* dfbo_new(int x, int y);
int   dfbo_free(dfbo*);
dfbo* dfbo_ref(dfbo*);

void dfbo_bind(dfbo*);
void dfbo_tex(dfbo*, dtex* color, dtex* depth); // null ignores changes

void dfbo_draw(dfbo*);
void dfbo_draw_depth(dfbo* fbo, float alpha);

dfbo* dfbo_bound(void);
void dfbo_resize(dfbo* fbo, int x, int y);




#endif
