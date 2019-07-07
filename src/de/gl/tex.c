#include "tex.h"

#define DE_GL_MAX_TEX_UNIT                    8
#define DE_GL_DEFAULT_TEX_MIN_FILTER          GL_LINEAR
#define DE_GL_DEFAULT_TEX_MAG_FILTER          GL_LINEAR

dtex* Dtex_bound[DE_GL_MAX_TEX_UNIT] = {
	NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL,
};
dfbo* Dfbo_bound = NULL;


void dtex_free(dtex* tex){ if( !tex ) return;
	if( tex->ref ) {
		tex->ref--;
		return;
	}
	glDeleteTextures(1, &tex->id);
	free(tex);
}

dtex* dtex_ref(dtex* t){ if(!t) return NULL;
	t->ref++;
	return t;
}



dtex* dtex_open(char* filename){
	if(!filename) return NULL;
	SDL_Surface* sur = IMG_Load(filename);
	if(sur == NULL){
		DE_LOG("unable to load texture: %s", (char*)IMG_GetError());
		return NULL;
	}
	dtex* ret = dtex_from_surface(sur);
	SDL_FreeSurface(sur);
	return ret;
}



dtex* dtex_new(v4c* pixels, int x, int y){
	return dtex_newf(pixels,x,y, GL_RGBA);
}

dtex* dtex_newf(v4c* pixels, int x, int y, GLenum format){
	return dtex_newt(pixels,x,y,GL_RGBA, format, GL_UNSIGNED_BYTE);
}

dtex* dtex_newt(v4c* pixels, int x, int y, GLenum internal_format, GLenum format, GLenum type){
	dtex* prevt = Dtex_bound[7];

	if(x < 1) x = 1;
	if(y < 1) y = 1;

	dtex* tex = malloc(sizeof(dtex));
	glGenTextures(1, &tex->id);
	tex->ref = 0;
	tex->x = x;
	tex->y = y;
	tex->min = GL_LINEAR;
	tex->mag = GL_LINEAR;
	tex->wrap_s = GL_REPEAT;
	tex->wrap_t = GL_REPEAT;
	// tex->type = GL_TEXTURE_2D;

	dtex_bind(tex, 7);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, tex->min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, tex->mag);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, tex->wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tex->wrap_t);

	if(pixels)
		// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA,
		// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, format,
			// GL_UNSIGNED_BYTE, (GLvoid*)pixels);
		glTexImage2D(GL_TEXTURE_2D, 0, internal_format, x, y, 0, format,
			type, (GLvoid*)pixels);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, internal_format, x, y, 0, format,
			type, NULL);


	dtex_bind(prevt, 7);
	return tex;
}





void dtex_bind(dtex* tex, int slot){
	slot = MIN(slot, DE_GL_MAX_TEX_UNIT-1);
	if(!tex){
		Dtex_bound[slot] = NULL;
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, 0);
		return;
	};
	Dtex_bound[slot] = tex;
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, tex->id);
}





#define DE_TEX_VFLIP(pixels, w,h)\
{{\
	uint x,y,rx,ry;\
	for(x=0, rx=(w)-1; x<(w);   x++, rx--){\
	for(y=0, ry=(h)-1; y<(h)/2; y++, ry--){\
			dswap(\
				&((Uint32*)pixels)[y*(w)+x],\
				&((Uint32*)pixels)[ry*(w)+x], 4);\
		}\
	}\
}}

dtex* dtex_from_surface(SDL_Surface* sur){
	if(sur == NULL){
		return NULL;
	}

	#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		SDL_Surface* s2 = SDL_ConvertSurfaceFormat(sur, SDL_PIXELFORMAT_RGBA8888, 0);
	#else
		SDL_Surface* s2 = SDL_ConvertSurfaceFormat(sur, SDL_PIXELFORMAT_ABGR8888, 0);
	#endif

	DE_TEX_VFLIP(s2->pixels, (uint)sur->w, (uint)sur->h);

	dtex* ret = dtex_new(s2->pixels, sur->w, sur->h);
	SDL_FreeSurface(s2);
	return ret;
}





void dtex_set(dtex* tex, v4c* pixels, int x, int y){
	if(x < 1) x = 1;
	if(y < 1) y = 1;

	tex->x = x;
	tex->y = y;

	glBindTexture(GL_TEXTURE_2D, tex->id);

	if(pixels)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA,
			GL_UNSIGNED_BYTE, (GLvoid*)pixels);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, 0, GL_RGBA,
			GL_UNSIGNED_BYTE, NULL);

}





void dtex_filter(dtex* tex, GLenum min, GLenum mag){if(!tex) return;
	if(!min) min = GL_NEAREST;
	if(!mag) mag = GL_NEAREST;

	glBindTexture(GL_TEXTURE_2D, tex->id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag);
}





void dtex_wrap(dtex* tex, GLenum wrap_s, GLenum wrap_t){if(!tex) return;
	if(!wrap_s) wrap_s = GL_REPEAT;
	if(!wrap_t) wrap_t = GL_REPEAT;

	glBindTexture(GL_TEXTURE_2D, tex->id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
}
























// frame buffer object
// #include "../window/window.h" // dviewport calls when binding fbo
#include "../de_struct.h" // set vieport to window size

dfbo* dfbo_new(int x, int y){
	dfbo* prevfbo = Dfbo_bound;

	dfbo* fbo = malloc(sizeof(dfbo));
	fbo->ref = 0;
	fbo->x = x;
	fbo->y = y;
	fbo->res = (float)y / (float)x;
	glGenFramebuffers(1, &fbo->id);
	glGenRenderbuffers(1, &fbo->rbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo->id);

	glBindRenderbuffer(GL_RENDERBUFFER, fbo->rbo);

#if defined GL_DEPTH24_STENCIL8_OES && !defined GL_DEPTH24_STENCIL8
	#define GL_DEPTH24_STENCIL8 GL_DEPTH24_STENCIL8_OES
#endif

	// #ifdef ANDROID
		// glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8_OES, x, y);
	// #if _WIN32
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, x, y);
	// #endif

	// glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, x, y);
	// glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL, x, y);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, fbo->rbo);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbo->rbo);
	// glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT_OES, GL_RENDERBUFFER, fbo->rbo);
	// glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, fbo->rbo);

	fbo->color = dtex_new(NULL, x, y);
	fbo->depth = NULL;
	// fbo->depth = dtex_newf(NULL, x, y, GL_DEPTH_COMPONENT);
	// fbo->depth = dtex_new_depth_stencil(NULL, x, y);
	dfbo_tex(fbo, fbo->color, fbo->depth);


	// fbo->depth = dtex_new(NULL, x, y);

		// dtex_bind(fbo->depth);
		// glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, x, y, 0, GL_DEPTH_COMPONENT,
		// 	GL_FLOAT, NULL);

	// dfbo_tex(fbo, fbo->color, NULL);


	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

GLenum status=glCheckFramebufferStatus(GL_FRAMEBUFFER);
if (status!=GL_FRAMEBUFFER_COMPLETE){
	DE_LOGE("incomplete fbo!");
}


	dfbo_bind(prevfbo);



return fbo;
}





int dfbo_free(dfbo* fbo){ if (!fbo) return 0;
	if( fbo->ref ) return --fbo->ref;

	glDeleteRenderbuffers(1, &fbo->rbo);
	glDeleteFramebuffers(1, &fbo->id);

	dtex_free(fbo->color);
	if(fbo->depth) dtex_free(fbo->depth);
	free(fbo);

	return 0;
}





void dfbo_bind(dfbo* fbo){
	De.fbo = fbo;
	if (fbo){
		glBindFramebuffer(GL_FRAMEBUFFER, fbo->id);
//moved to camera
		glViewport(0,0, fbo->x, fbo->y);
		// dwindow_resized(fbo->x, fbo->y);
		// De.res = (float)fbo->y / (float)fbo->x;

	} else {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0,0, De.size[0], De.size[1]);
		// dwindow_resized(De.size[0], De.size[1]);
	}
}





void dfbo_tex(dfbo* fbo, dtex* color, dtex* depth){if(!fbo) return;
	dfbo* prevfbo = Dfbo_bound;
	dfbo_bind(fbo);
	dtex* prev6 = Dtex_bound[6];
	dtex* prev7 = Dtex_bound[7];

	if(color){
		dtex_bind(color, 6);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D, color->id, 0);
	}

	if(depth){
		dtex_bind(depth, 7);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		// glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
			GL_TEXTURE_2D, depth->id, 0);
	}

	//restore
	dfbo_bind(prevfbo);
	dtex_bind(prev6, 6);
	dtex_bind(prev7, 7);
}





void dfbo_draw(dfbo* fbo){
	// dfbo_bind(fbo);
	// if(fbo->id)glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbo->rbo);

	// dtex_draw(fbo->color);
	// dfbo_bind(0);
	// glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);

	dtex_draw(fbo->color);
}





























#include "vao.h"
#include "shd.h"
#include "gl.h"
static dvao* tex_draw_vao;
static dshd* tex_draw_shd;
static v4f tex_draw_verts[6];
static int tex_draw_unit = 7;
static float tex_draw_alpha = 1.f;

bool tex_draw_inited = false;

void dtex_draw(dtex* tex){
	dtex_drawa(tex, 1);
}

void dtex_drawa(dtex* tex, float alpha){
	if(!tex_draw_inited){
		tex_draw_inited = true;
		tex_draw_verts[0] = (v4f){-1,-1,0,0};
		tex_draw_verts[1] = (v4f){1, -1,1,0};
		tex_draw_verts[2] = (v4f){-1, 1,0,1};
		tex_draw_verts[3] = tex_draw_verts[2];
		tex_draw_verts[4] = tex_draw_verts[1];
		tex_draw_verts[5] = (v4f){1,1,1,1};

		void* tex_draw_vert_buf[] = {tex_draw_verts};
		tex_draw_vao = dvao_newr("f2p,f2tc", tex_draw_vert_buf, 6, GL_TRIANGLES);
		tex_draw_shd = dshd_new(
			DE_SHD_HEADERV QUOTE(
				attribute vec2 pos;
				attribute vec2 tc;
				varying vec2 vtc;
				void main(){
					vtc = tc;
					gl_Position = vec4(pos.xy, 0.0, 1.0);
				}
			),
			DE_SHD_HEADERF QUOTE(
				uniform sampler2D tex;
				uniform float alpha;
				varying vec2 vtc;
				void main(){
					vec4 tcol=texture2D(tex, vtc);
					// if(tcol.a==0.0) discard;
					tcol.a *= alpha;
					gl_FragColor = tcol;
				}
			)
		);
		dshd_unif(tex_draw_shd, "tex", &tex_draw_unit);
		dshd_unif(tex_draw_shd, "alpha", &tex_draw_alpha);
	}

	dtex* prev_tex = Dtex_bound[tex_draw_unit];

	dcullf(0);
	dblend(1);
	// ddepth(0,0);

	tex_draw_alpha = alpha;

	dtex_bind(tex, tex_draw_unit);
	dshd_bind(tex_draw_shd);
	dshd_update(tex_draw_shd);
	dvao_draw(tex_draw_vao, 0, 0, 0);

	dtex_bind(prev_tex, tex_draw_unit);

	dblend(0);
}
