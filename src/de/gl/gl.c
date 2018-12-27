#include "gl.h"
#include "../de_struct.h"

#define DE_GL_DEFAULT_CLEAR_FLAGS GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT
#define DE_GL_DEFAULT_DEPTH_FUNC              GL_LESS
#define DE_GL_DEFAULT_BLEND_FUNC              GL_ONE_MINUS_SRC_ALPHA
#define DE_GL_DEFAULT_ALPHA_FUNC              GL_ALWAYS

int dgl_init(void){
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	dvsync(1);

	dviewport( De.size[0], De.size[1] );

	return 0;
}


void dgl_quit(void){ return; }



void dclear(GLint flags){
	if(!flags)
		flags = DE_GL_DEFAULT_CLEAR_FLAGS;
		// flags = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT;
	glClear(flags);
}



void dclear_color(v4f color){
	glClearColor(color[0], color[1], color[2], color[3]);
}



void ddisplay(void){
	SDL_GL_SwapWindow(De.win);
}



void dvsync(int i){
	if(i == -1){ // adaptive vsync
		if( SDL_GL_SetSwapInterval(i) == -1){ // not supported, using regular vsync
			DE_LOG("adaptive vsync unsuported, using 1 as interval");
			i = 1;
		} else {
			De.vsync = i;
			return;
		}
	}
	De.vsync = i;
	SDL_GL_SetSwapInterval(i);
}



void dcullf(char face){
	switch(face){
		case 'b': case'B': case 1: glCullFace(GL_BACK); glEnable(GL_CULL_FACE); break;
		case 'f': case 'F': case 2: glCullFace(GL_FRONT); glEnable(GL_CULL_FACE); break;
		case 0: default: glDisable(GL_CULL_FACE); break;
	}
}



void dviewport(int x, int y){
	De.ctx_size = (v2i){x, y};
	De.ctx_res = ((float)y) / ((float)x);
	glViewport(0,0,x,y);
}



void dblend(bool enable){
	if(enable)
		glEnable(GL_BLEND);
	else
		glDisable(GL_BLEND);
}



void dalpha( bool b, GLenum fnc, GLclampf ref ){
	if(fnc == 0) fnc = DE_GL_DEFAULT_ALPHA_FUNC;
	if(b){
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc( fnc, ref );
	}
	else
		glDisable(GL_ALPHA_TEST);
}



void ddepth(bool enable, GLenum func){
	if(enable) glEnable(GL_DEPTH_TEST);
	else glDisable(GL_DEPTH_TEST);
	if(!func) func = DE_GL_DEFAULT_DEPTH_FUNC;
	glDepthFunc(func);
}




