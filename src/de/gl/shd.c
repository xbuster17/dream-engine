#include "shd.h"
#include "../de_struct.h"

static void show_shader_log( GLuint object);
static void show_shader_program_log(GLuint object);

static m4f _uniform_null = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};





dshd* dshd_new(char* vertex_src, char* fragment_src){
	if(!vertex_src || !fragment_src) return NULL;


	dshd* shd = malloc(sizeof(dshd));

	int vertex_src_len = strlen(vertex_src);
	int fragment_src_len = strlen(fragment_src);

// vertex shader
	GLuint vs_id = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs_id, 1, (const GLchar**)&vertex_src, (const GLint*)&vertex_src_len);
	glCompileShader(vs_id);

	GLint vs_shader_ok;
	glGetShaderiv(vs_id, GL_COMPILE_STATUS, &vs_shader_ok);
	if (!vs_shader_ok){
		glClearColor(1,0,0,1); //tmp
		show_shader_log(vs_id);
		glDeleteShader(vs_id);
		#ifndef ANDROID
			DE_LOGE("Failed to compile vertex shader: src:\n%s", vertex_src);
		#endif
		shd->program = 0;
		free(shd);
		return NULL;
	}

// fragment shader
	GLuint fs_id = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs_id, 1, (const GLchar**)&fragment_src, (const GLint*)&fragment_src_len);
	glCompileShader(fs_id);

	GLint fs_shader_ok;
	glGetShaderiv(fs_id, GL_COMPILE_STATUS, &fs_shader_ok);
	if (!fs_shader_ok){
		glClearColor(0,0,1,1);
		show_shader_log(fs_id);
		glDeleteShader(fs_id);
		#ifndef ANDROID
			DE_LOGE("Failed to compile fragment shader: src:\n%s", fragment_src);
		#endif
		shd->program = 0;
		free(shd);
		return NULL;
	}

	GLuint shd_program = glCreateProgram();
	shd->program = shd_program;

	glAttachShader(shd_program, vs_id);
	glAttachShader(shd_program, fs_id);


// first link
	if(dshd_link_program(shd_program)) {
		glDeleteProgram(shd_program);
		shd->program = 0;
		free(shd);
		return NULL;
	}


//fetch attributes
	int active_attribs;
	int active_attribs_max_len;
	glGetProgramiv(shd_program, GL_ACTIVE_ATTRIBUTES, &active_attribs);
	glGetProgramiv(shd_program, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &active_attribs_max_len);

	int attrib_sizes[ active_attribs ];
	GLenum attrib_types[ active_attribs ];
	char attrib_names[ active_attribs ][ active_attribs_max_len+1 ];
	GLsizei name_len = 0;


	for(int i = 0; i < active_attribs; i++){
		glGetActiveAttrib(shd_program, (uint)i, active_attribs_max_len, &name_len,
			attrib_sizes + i, attrib_types + i, attrib_names[i] );
		attrib_names[i][ name_len ] = '\0';
	}


//sort attributes by order of appearence
	char* aspos[ active_attribs ]; // attribute's first appearence in source
	int asorti[ active_attribs ]; // sorted indices for attrib_names

	for(int i = 0; i < active_attribs; i++){
		aspos[i] = strstr( vertex_src, attrib_names[i] );
	}

	for(int i = 0; i < active_attribs; i++){
		int mini = 0;
		char* min =  aspos[0];

		for(int j = 0; j < active_attribs; j++){
			if( aspos[j] != NULL ){
				if( min == NULL ){ min = aspos[j]; mini = j; }
				if( aspos[j] < min ) {
					min = aspos[j];
					mini = j;
				}
			}
		}

		aspos[mini] = NULL;
		asorti[i] = mini;
	}


	// bind them
	for(int i = 0; i < active_attribs; i++){
		glBindAttribLocation(shd_program ,i, attrib_names[ asorti[i] ]);
	}



// link again
	if(dshd_link_program(shd_program)) {
		glDeleteProgram(shd_program);
		shd->program = 0;
		free(shd);
		return NULL;
	}

	//test binding
	// for(int i = 0; i < active_attribs; i++){
	// 	if(i != glGetAttribLocation(shd_program ,attrib_names[ asorti[i] ]))
	// 		DE_LOGE("attrib binding didnt work for attrib %i:%s",i, attrib_names[ asorti[i] ]);
	// }


// fetch uniforms
	int active_uniforms;
	int active_uniforms_max_len;
	glGetProgramiv(shd_program, GL_ACTIVE_UNIFORMS, &active_uniforms);
	glGetProgramiv(shd_program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &active_uniforms_max_len);

	shd->uniforms = malloc(sizeof(struct dshd_uniforms) * active_uniforms);

	char unif_name[active_uniforms_max_len];
	int fi = 0;
	GLenum unif_type = 0;
	int unif_alen = 0;

	for(int i = 0; i < active_uniforms; i++){
		glGetActiveUniform(shd_program, i, active_uniforms_max_len, NULL,
			&unif_alen, &unif_type, unif_name);
		shd->uniforms[i].size = 0;
		shd->uniforms[i].value = &_uniform_null;
		shd->uniforms[i].name = NULL;
		shd->uniforms[i].index = -1;

		if( strstr(unif_name, "gl_") ) continue;

		shd->uniforms[fi].size = dshd_uniform_type_size(unif_type);
		shd->uniforms[fi].value = &_uniform_null;

		shd->uniforms[fi].index = glGetUniformLocation(shd->program, unif_name);
		shd->uniforms[fi].type = unif_type;
		shd->uniforms[fi].alen = unif_alen;
		// shd->uniforms[fi].name = strndup(unif_name, active_uniforms_max_len);
		shd->uniforms[fi].name = strdup(unif_name);

		fi++;
	}

	shd->unif_count = fi;


//finish
	glDetachShader(shd_program, vs_id);
	glDetachShader(shd_program, fs_id);
	glDeleteShader(vs_id);
	glDeleteShader(fs_id);


	shd->vert_id = vs_id;
	shd->frag_id = fs_id;

	shd->unif_values = malloc(sizeof(m4f) * fi);
	for(int i = 0; i < fi; i++)
		shd->unif_values[i] = _uniform_null;

	shd->ref = 0;

	return shd;
}





dshd* dshd_ref(dshd* shd){ if(!shd) return NULL;
	shd->ref++;
	return shd;
}





void dshd_free(dshd* shd){
	if(!shd) return;
	if(shd->ref){ shd->ref--; return; }

	for (int i = 0; i < shd->unif_count; i++){
		free(shd->uniforms[i].name);
	}
	free(shd->uniforms);
	glDeleteProgram(shd->program);
}






























int dshd_link_program(GLuint program){
	glLinkProgram(program);
	GLint program_ok;
	glGetProgramiv(program, GL_LINK_STATUS, &program_ok);
	if (!program_ok){
		show_shader_program_log(program);
		// glDeleteProgram(program);
		// shd->program = 0;
		// free(shd);
		return 1;
	} else
	return 0;
}





void dshd_bind(dshd* shd){
	if(!shd) De.shd_bound = 0;
	else De.shd_bound = shd->program;
	glUseProgram(De.shd_bound);
}





int dshd_unif_loc(dshd* shd, char* name){ if(!shd || !name) return -1;
	return glGetUniformLocation(shd->program, name);
}





// bind memory to a uniform value
void* dshd_unif(dshd* shd, char* name, void* store_ptr){ if(!shd) return NULL;
	if( store_ptr == NULL ) store_ptr = &_uniform_null;

	int index = glGetUniformLocation(shd->program, name);
	if (index==-1) {
		DE_LOGE("uniform name not found: %s", name);
		return NULL;
	}

	void* prev = NULL;
	for (int i = 0; i < shd->unif_count; ++i){
		if(index == shd->uniforms[i].index){
			prev = shd->uniforms[i].value;
			shd->uniforms[i].value = store_ptr;
		}
	}
	return prev;
}





void* dshd_unifi(dshd* shd, int index, void* store_ptr){ if(!shd) return NULL;
	if(index < 0) return NULL;
	if( store_ptr == NULL ) store_ptr = &_uniform_null;

	void* prev = NULL;
	for (int i = 0; i < shd->unif_count; ++i){
		if(index == shd->uniforms[i].index){
			prev = shd->uniforms[i].value;
			shd->uniforms[i].value = store_ptr;
		}
	}
	return prev;
}





void dshd_update(dshd* shd){ if(!shd) return;
	dshd_bind(shd);
	int diff = 0;
	for(int i = 0; i < shd->unif_count; i++){
		// assert(shd->uniforms[i].size);
		//has different value than stored?
		diff = memcmp(
			shd->unif_values + i, shd->uniforms[i].value,
			shd->uniforms[i].size
		);
		if( diff ){
			memcpy( shd->unif_values + i, shd->uniforms[i].value,
				shd->uniforms[i].size);
			dshd_unif_upload(shd, i);
		}
	}
}





void dshd_unif_upload(dshd* shd, int index){ if(!shd) return;
	if(index >= shd->unif_count) return;
	GLenum type = shd->uniforms[index].type;
	void* data = shd->unif_values + index;
	int loc = shd->uniforms[index].index;
	switch(type){
		// case GL_SAMPLER_1D:
		case GL_SAMPLER_2D:
		// case GL_SAMPLER_3D:
		case GL_SAMPLER_CUBE:
		// case GL_SAMPLER_1D_SHADOW:
		// case GL_SAMPLER_2D_SHADOW:
		default:
			glUniform1i(loc, ((int*)data)[0]); break;

		case GL_FLOAT:      glUniform1f(loc, ((float*)data)[0]); break;
		case GL_FLOAT_VEC2: glUniform2f(loc, ((float*)data)[0], ((float*)data)[1]); break;
		case GL_FLOAT_VEC3: glUniform3f(loc, ((float*)data)[0], ((float*)data)[1], ((float*)data)[2]); break;
		case GL_FLOAT_VEC4: glUniform4f(loc, ((float*)data)[0], ((float*)data)[1], ((float*)data)[2], ((float*)data)[3]); break;
		case GL_FLOAT_MAT2: glUniformMatrix2fv(loc, ((GLsizei)1ul), GL_FALSE, (const float*)data); break;
		case GL_FLOAT_MAT3: glUniformMatrix3fv(loc, ((GLsizei)1ul), GL_FALSE, (const float*)data); break;
		case GL_FLOAT_MAT4: glUniformMatrix4fv(loc, ((GLsizei)1ul), GL_FALSE, (const float*)data); break;
		case GL_INT:        glUniform1i(loc, *(int*)data); break;
		case GL_INT_VEC2:   glUniform2i(loc, ((int*)data)[0], ((int*)data)[1] ); break;
		case GL_INT_VEC3:   glUniform3i(loc, ((int*)data)[0], ((int*)data)[1], ((int*)data)[2]); break;
		case GL_INT_VEC4:   glUniform4i(loc, ((int*)data)[0], ((int*)data)[1], ((int*)data)[2], ((int*)data)[3]); break;
	}
}





int dshd_uniform_type_size(GLenum type){
	switch(type){
		// case GL_SAMPLER_1D:
		// case GL_SAMPLER_1D_SHADOW: case GL_SAMPLER_2D_SHADOW:
		case GL_SAMPLER_2D:
		// case GL_SAMPLER_3D:
		case GL_SAMPLER_CUBE:
		case GL_INT:
			return sizeof(GLint);

		case GL_FLOAT:      return sizeof(GL_FLOAT);
		case GL_FLOAT_VEC2: return sizeof(GL_FLOAT) * 2;
		case GL_FLOAT_VEC3: return sizeof(GL_FLOAT) * 3;
		case GL_FLOAT_VEC4: return sizeof(GL_FLOAT) * 4;
		case GL_FLOAT_MAT2: return sizeof(GL_FLOAT) * 2*2;
		case GL_FLOAT_MAT3: return sizeof(GL_FLOAT) * 3*3;
		case GL_FLOAT_MAT4: return sizeof(GL_FLOAT) * 4*4;
		case GL_INT_VEC2:   return sizeof(GL_INT) * 2;
		case GL_INT_VEC3:   return sizeof(GL_INT) * 3;
		case GL_INT_VEC4:   return sizeof(GL_INT) * 4;
		default:
			DE_LOG("wrong type %i, invalid return", type);
			return 0;
	}
}

























void show_shader_log( GLuint object){
	GLint log_length;
	char* log_;
	glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
	log_ = malloc(log_length);
	glGetShaderInfoLog(object, log_length, NULL, log_);
	DE_LOGE("shader_log: %s", log_);
	free(log_);
}





void show_shader_program_log(GLuint object){
	GLint log_length;
	char* log_;
	glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
	log_ = malloc(log_length);
	glGetProgramInfoLog(object, log_length, NULL, log_);
	#ifndef ANDROID
		DE_LOGE("shader_program_log: %s", log_);
	#endif
	free(log_);
}















