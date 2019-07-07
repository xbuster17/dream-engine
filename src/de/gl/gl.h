#ifndef __DE_GL_H__
#define __DE_GL_H__

#include "../libs.h"
#include "../conf.h"
#include "../common/common.h"

#include "vao.h"
#include "shd.h"
#include "tex.h" // fbo & rbo

int dgl_init(void);
void dgl_quit(void);

void dclear(GLint flags);
void dclear_color(v4f color);
void ddisplay(void);

void dvsync(int n); // 0=disable, 1..n=sync every n screen frames
void dcullf(char c); // 'f'=front, 'b'=back, '\0'=off

void dviewport(int x, int y);

void dblend(bool enable);
// void dalpha(bool enable, GLenum function, GLclampf reference);
void ddepth(bool enable, GLenum func);


#endif
