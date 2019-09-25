#ifndef __DE_GL_VAO_H__
#define __DE_GL_VAO_H__

#include "../libs.h"
#include "../conf.h"
#include "../common/common.h"

// vertex format string
// made from a type character(s) ie: f, c, s, i, uc, us, ui, unc, usn, iun
// f:float, c:char, s:short, i:int, u:unsigned, n:normalized.
// followed by a dimension character ('1' to '4')
// and a secuence of characters for attribute name
// all white space is culled.

// attributes are separated with ',' if they should be stored in a single, interleaved, vbo
// or ';' that creates a new parallel vbo for the next attributes
// example: "f4p,f4n;f2t,i2b" maps to buffers:
// vbo0 = ppppnnnn ppppnnnn ...
// vbo1 = ttbb     ttbb ...
// each character representing a datum of their respective type
// binding the vao binds all the vbos
// read opengl vertex specification docs for more details

#define DE_GL_DEFAULT_DRAW_MODE GL_TRIANGLES
#define DE_GL_DEFAULT_VBO_USAGE GL_STATIC_DRAW




// vertex buffer object sub module
typedef struct dvbo{
	GLuint id;
	int ref;
	GLsizei size;
	GLenum usage; // GL_STATIC_DRAW, ...
} dvbo;

dvbo* dvbo_new(uint initial_size, void* initial_data, GLenum usage);
void dvbo_free(dvbo*);
void dvbo_bind(dvbo*);

void dvbo_data(dvbo* vbo, void* data, uint size, GLenum usage); // realloc
void dvbo_set(dvbo*, uint offset, void* data, uint size);
void dvbo_set_usage(dvbo* vbo, GLenum usage);










// vertex attrib pointer helper
#define DVAP_TYPE_CHAR     GL_BYTE
#define DVAP_TYPE_UCHAR    GL_UNSIGNED_BYTE
#define DVAP_TYPE_SHORT    GL_SHORT
#define DVAP_TYPE_USHORT   GL_UNSIGNED_SHORT
#define DVAP_TYPE_INT      GL_INT
#define DVAP_TYPE_UINT     GL_UNSIGNED_INT
#define DVAP_TYPE_FLOAT    GL_FLOAT

#define DVAP_TYPE_CHAR_SIZE     (sizeof(GLbyte))
#define DVAP_TYPE_UCHAR_SIZE    (sizeof(GLubyte))
#define DVAP_TYPE_SHORT_SIZE    (sizeof(short))
#define DVAP_TYPE_USHORT_SIZE   (sizeof(unsigned short))
#define DVAP_TYPE_INT_SIZE      (sizeof(GLint))
#define DVAP_TYPE_UINT_SIZE     (sizeof(GLuint))
#define DVAP_TYPE_FLOAT_SIZE    (sizeof(GLfloat))

// vertex attrib pointer for glVertexAttribPointer
typedef struct dvap {
	char* name; // optional name for an attribute on a vertex shader
	int vbo_index;
	GLboolean normalize;
	// GLuint index; // index is now selected based on the position on the array
	GLint dim;          /*1 to 4*/
	GLenum type;        /*GL_FLOAT, GL_INT, etc...*/
	GLsizei stride;     /*sizeof all components*/
	uintptr_t offset;   /*where the attrib being defined starts in the vbo*/
} dvap;
// array of dvap
typedef struct dvaps{
	int len; // attrib count
	int ref;
	int num_buffers; // how many buffers does the vap want
	dvap* vap;
} dvaps;

void dvaps_print(dvaps*);

dvaps* dvaps_new(char* format);
dvaps* dvaps_ref(dvaps*);
void dvaps_free(dvaps*);
void dvaps_bind(dvaps*, dvbo** buffers, int* buffer_offsets, int attribute_shift);

char* dvaps_minify(char* format); // free me when done
int dvaps_parse(dvaps*, char* format);

int dvaps_get_buffer_stride(dvaps*, int buffer);
uint dvaps_type_size(GLenum type);










typedef struct dvao{
	int ref;
	GLuint id;
	int len;
	char* format;
	dvaps* vaps; // internal vertex specification
	dvbo** vbos;

	darr** vertex_data;

	v2i* modified; // sequence of intervals to track updates, byte offset
	GLenum mode;
} dvao;


dvao* dvao_new(char* vertex_format, void** vertex_sources, int num_vertices, GLenum mode);

// faster, for interleaved attributes
dvao* dvao_newr(char* vertex_format, void** vertex_sources, int num_vertices, GLenum mode);

dvao* dvao_ref(dvao*);

void dvao_free(dvao*);

void dvao_bind(dvao*);

int dvao_draw(dvao*, uint start, uint len, GLenum mode);
// start and len 0 for drawing everything
// modes: GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_TRIANGLE_STRIP,
//        GL_TRIANGLE_FAN, GL_TRIANGLES

void dvao_draw_quad(void); // "f2pos" for rendering shaders

// int dvao_get_index(dvao*, char* attribute_name);

void dvao_set_mode(dvao*, GLenum draw_mode);

// copy n bytes starting at src into vao->vertex_data, formated (interleaved) for opengl
// n is attrb_type_size * attrib_dim * vertex_count
// this is ONLY commited to vram after calling dvao_update
int dvao_set(dvao* vao, int attribute_id, void* src, int target_vertex, int vertex_count);

// if src is already interleaved you can use setr
int dvao_setr(dvao* vao, int buffer_index, void* src, int target_vertex, int vertex_count);


int dvao_get(dvao* , int attr_id, int first, int count, void* dst);

// commit changes (if any) to vram
int dvao_update(dvao*);

void dvao_set_usage(dvao* vao, int buffer_index, GLenum usage);
void dvao_set_mode(dvao* vao, GLenum mode);
























typedef struct dibo {
	GLuint id;
	int len;
	GLenum mode;
} dibo;


dibo* dibo_new(Uint32* src, int count, GLenum mode);
// dibo* dibo_new(short* src, int count, GLenum mode);
void dibo_free(dibo*);
void dibo_draw(dibo* ibo, int start, int count, GLenum mode);



#endif
