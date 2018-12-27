#ifndef __DE_GL_SHADER_H__
#define __DE_GL_SHADER_H__

#include "../libs.h"
#include "../conf.h"
#include "../common/common.h"

#define DSHD_QUOTEV(x) DE_SHD_HEADERV QUOTE(x)
#define DSHD_QUOTEF(x) DE_SHD_HEADERF QUOTE(x)


typedef struct dshd{
	int ref;
	GLuint program;

	GLuint vert_id;
	GLuint frag_id;

	char* vert_src;
	char* frag_src;

	int attrib_count;
	char** attributes;

	struct dshd_uniforms{ // ignores "gl_" uniform names
		int index; // the return value of glGetUniformLocation
		char* name;
		GLenum type;
		GLint alen; // array length. not supported, TODO
		int size;
		// points to variable holding the value, inits to null which uploads default value
		void* value;
	} *uniforms; // uniforms[ unif_count ];
	int unif_count;

	// here we store a copy of all the values as a matrix array
	// since largest uniform is a 4x4 float matrix
	// after calling dshd_update, each m4f has the same value of *shd->unif[i]->value
	m4f* unif_values;
} dshd;


dshd* dshd_new(char* vertex, char* fragment);
dshd* dshd_ref(dshd*);
void dshd_free(dshd*);
void dshd_bind(dshd*); // NULL: glUseProgram(0)

void dshd_parse_variables(dshd*);

int dshd_link_program(GLuint program);

// returns previous value pointer
void* dshd_unif(dshd*, char* name, void* store_ptr);
void* dshd_unifi(dshd*, int index, void* store);
void dshd_update(dshd*);
void dshd_unif_upload(dshd*, int index);

int dshd_uniform_type_size(GLenum type);



#endif
