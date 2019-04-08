#include "vao.h"
#include "../de_struct.h"

#define DE_GL_DEFAULT_DRAW_MODE GL_TRIANGLES
#define DE_GL_DEFAULT_VBO_USAGE GL_STATIC_DRAW


void dvao_free(dvao* vao){
	if( !vao ) return;
	if( vao->ref ){ vao->ref--; return; }

	for( int i = 0; i < vao->vaps->num_buffers; i++){
		dvbo_free(vao->vbos[i]);
		darr_free(vao->vertex_data[i]);
	}
	free(vao->vbos);
	free(vao->vertex_data);
	free(vao->format);

	dvaps_free(vao->vaps);

	if(De.glext_has_vao)
		glDeleteVertexArrays(1, &vao->id);
	free(vao->modified);
	free(vao);
}





dvao* dvao_new(char* vertex_format, void** vertex_sources, int num_vertices, GLenum mode){
	if(!vertex_format) return NULL;

	dvao* vao = malloc(sizeof(dvao));

	vao->ref = 0;
	vao->mode = mode;
	vao->len = num_vertices;
	vao->format = dvaps_minify(vertex_format);

	vao->vaps = dvaps_new(vao->format);

	int num_buffers = vao->vaps->num_buffers;
	vao->vertex_data = malloc(sizeof(darr*) * num_buffers);
	vao->vbos = malloc(sizeof(dvbo*) * num_buffers);
	vao->modified = malloc(sizeof(v2i) * num_buffers);

	for( int i = 0; i < num_buffers; i++ ){
		vao->modified[i][0] = INT_MAX;
		vao->modified[i][1] = 0;
		int size = num_vertices * dvaps_get_buffer_stride(vao->vaps, i);
		vao->vertex_data[i] = darr_new(1, size, NULL);
		vao->vbos[i] = dvbo_new(size, NULL, 0);
	}

	if(De.glext_has_vao){
		glGenVertexArrays(1, &vao->id);
		glBindVertexArray(vao->id);
		dvaps_bind(vao->vaps, vao->vbos, NULL, 0);

		glBindVertexArray(0);
	}

	if(vertex_sources){
		for( int i = 0; i < vao->vaps->len; i++ ){
			int err = dvao_set(vao, i, vertex_sources[i], 0, num_vertices);
			if(err) DE_LOG("error vao_set: %i\n", err); // todo proper error logging
		}
		dvao_update(vao);
	}
	return vao;
}





dvao* dvao_newr(char* vertex_format, void** vertex_sources, int num_vertices, GLenum mode){
	if(!vertex_format) return NULL;

	dvao* vao = dvao_new(vertex_format, NULL, num_vertices, mode);

	if(vertex_sources){
		for( int i = 0; i < vao->vaps->num_buffers; i++ ){
			int err = dvao_setr(vao, i, vertex_sources[i], 0, num_vertices);
			if(err) DE_LOG("error at vao_set: %i\n", err); // todo proper error logging
		}
	}
	dvao_update(vao);
	return vao;
}





void dvao_bind(dvao* vao){
	if(De.glext_has_vao)
		glBindVertexArray(vao->id);
	else
		dvaps_bind(vao->vaps, vao->vbos, NULL,0);
}





void dvao_set_mode(dvao* vao, GLenum mode){
	if(!vao) return;
	if(mode != 0)
		vao->mode = mode;
	else
		vao->mode = DE_GL_DEFAULT_DRAW_MODE;
}





void dvao_set_usage(dvao* vao, int b_id, GLenum usage){
	if(!vao) return;
	if(b_id >= vao->vaps->num_buffers) return;
	dvbo_set_usage(vao->vbos[b_id], usage);
}





int _dshd_bound_log_once = 0;
int dvao_draw(dvao* vao, uint start, uint len, GLenum mode){
	if(!vao) return 1;
	if(!De.shd_bound){
		_dshd_bound_log_once++ ? :
		(DE_LOG("De.shd_bound is 0!"));
	}
	if(len == 0) len = vao->len;

	dvao_bind(vao);
	if(mode == 0)
		glDrawArrays(vao->mode, start, len);
	else
		glDrawArrays(mode, start, len);

	return 0;
}





int dvao_drawi(dvao* vao, dibo* ibo, uint start_index, uint len, GLenum mode){
	if(!vao) return 1;
	if(!ibo) return 2;
	if(!De.shd_bound){
		_dshd_bound_log_once++ ? :
		(DE_LOG("De.shd_bound is 0!"));
	}
	if(len == 0) len = ibo->len;

	dvao_bind(vao);
	if(mode == 0)
		glDrawArrays(vao->mode, start_index, len);
	else
		glDrawArrays(mode, start_index, len);

	return 0;
}





int dvao_set(dvao* vao, int attr_id, void* src, int target_vertex, int vertex_count){
	if(!vao) return 1;
	if(!src) return 3;
	if(attr_id >= vao->vaps->len) return 2;

	int vbo_index = vao->vaps->vap[attr_id].vbo_index;
	int attr_size = dvaps_type_size(vao->vaps->vap[attr_id].type) * vao->vaps->vap[attr_id].dim;
	int stride = vao->vaps->vap[attr_id].stride;
	int offset = vao->vaps->vap[attr_id].offset;
	darr* dst =  vao->vertex_data[vbo_index];

	vao->len = max(vao->len, target_vertex + vertex_count);
	vao->modified[vbo_index][0] = min(offset + target_vertex * stride, vao->modified[vbo_index][0]);
	vao->modified[vbo_index][1] = max(attr_size + offset + (target_vertex + vertex_count) * stride, vao->modified[vbo_index][1]);

	//slow
	// for(int i = target_vertex; i < target_vertex+vertex_count; i++){
	// 	for(int j = 0; j < attr_size; j++){
	// 		darr_set(dst, offset + i*stride + j, src+j);
	// 	}
	// 	src += attr_size;
	// }

	darr_set(dst, (target_vertex + vertex_count) * stride + stride, src); //just to extend the array to necesary
	for(int i = target_vertex; i < target_vertex+vertex_count; i++){
		memcpy(dst->data + i * stride + offset , src, attr_size);
		src += attr_size;
	}

	return 0;
}





int dvao_setr(dvao* vao, int vbo_id,void* src, int target_vertex, int vertex_count){
	if(!vao){ DE_LOG("vao is null"); return 1;}
	if(!src){ DE_LOG("src is null"); return 3;}
	if(vbo_id >= vao->vaps->num_buffers){DE_LOG("vbo out of bounds"); return 2;}

	int stride = dvaps_get_buffer_stride(vao->vaps, vbo_id);

	darr* dst =  vao->vertex_data[vbo_id];

	//slow
	// for(int i = target_vertex; i < target_vertex + vertex_count; i++){
	// 	for(int j = 0; j < stride; j++){
	// 		darr_set(dst, i*stride + j, src+j);
	// 	}
	// 	src += stride;
	// }

	darr_set(dst, (target_vertex + vertex_count) * stride + stride, src); //just to extend the array if needed
	memcpy(dst->data + target_vertex * stride, src, vertex_count * stride);


	vao->len = max(vao->len, target_vertex + vertex_count);
	vao->modified[vbo_id][0] = min(vao->modified[vbo_id][0], target_vertex*stride);
	vao->modified[vbo_id][1] = max(vao->modified[vbo_id][1], (1+target_vertex+vertex_count)*stride);

	return 0;
}





int dvao_get(dvao* vao, int attr_id, int first, int count, void* dst){if(!vao)return 1;
	if(attr_id >= vao->vaps->len) return 2;
	if(!dst) return 3;
	if( first + count > vao->len ){ DE_LOG("vertex > length; %i > %i", first+count, vao->len); return 4;}
	uint vbo_index = vao->vaps->vap[attr_id].vbo_index;
	uint attr_size = dvaps_type_size(vao->vaps->vap[attr_id].type) * vao->vaps->vap[attr_id].dim;
	uint stride = vao->vaps->vap[attr_id].stride;
	uint offset = vao->vaps->vap[attr_id].offset;
	darr* src =  vao->vertex_data[vbo_index];

	for(int i = first; i < first + count; i++){
		for(uint j = 0; j < attr_size; j++){
			void* p = darr_get(src, offset + i*stride + j);
			memcpy( dst + j, p, 1);
		}
		dst += attr_size;
	}


	return 0;
}





int dvao_update(dvao* vao){
	if (!vao) return -1;
	int mods = 0;

	for (int i = 0; i < vao->vaps->num_buffers; ++i){
		if(vao->modified[i][0] > vao->modified[i][1]) continue;
		mods++;
		if(vao->modified[i][1] >= vao->vbos[i]->size){
			int stride = dvaps_get_buffer_stride(vao->vaps, i);
			dvbo_data(vao->vbos[i], vao->vertex_data[i]->data,
				vao->len * stride, vao->vbos[i]->usage);
		}
		else
			dvbo_set(vao->vbos[i], vao->modified[i][0],
				vao->vertex_data[i]->data + vao->modified[i][0],
				vao->modified[i][1] - vao->modified[i][0]);

		vao->modified[i][0] = INT_MAX;
		vao->modified[i][1] = 0;
	}

	return mods;
}


























// vertex attrib pointer helper
dvaps* dvaps_new(char* format){ if(!format) return NULL; // todo implement some basic error loging
	dvaps* vaps = malloc(sizeof(dvaps));
	vaps->ref = 0;
	int err = dvaps_parse(vaps, format);
	if(!err)
		return vaps;
	else
		return NULL;
}

dvaps* dvaps_ref(dvaps* vaps){if(!vaps) return NULL; vaps->ref++; return vaps;}

void dvaps_free(dvaps* vaps){
	if(!vaps) return;
	if(vaps->ref){vaps->ref--; return;}

	for(int i = 0; i<vaps->len; i++ ){
		free(vaps->vap[i].name);
	}
	free(vaps->vap);
	free(vaps);
}





void dvaps_bind(dvaps* vaps, dvbo** buffers, int* offsets, int attribute_shift){ if(!vaps) return;
	for(int i = 0; i < vaps->len; i++){
		dvap* vap = &vaps->vap[i];
		dvbo_bind(buffers[vap->vbo_index]);

		glVertexAttribPointer(i + attribute_shift, vap->dim, vap->type,
			vap->normalize, vap->stride, (void*)vap->offset + (offsets ? offsets[i] : 0) );

		glEnableVertexAttribArray(i);
	}
}





void dvaps_print(dvaps* vaps){ if(!vaps) return;
	for(int i = 0; i<vaps->len; i++ ){
		printf(
			"name:%s, normalize:%i, vbo_index:%i, dim:%i, type:%i, stride:%i, offset:%lu\n",
			vaps->vap[i].name,
			vaps->vap[i].normalize,
			vaps->vap[i].vbo_index,
			vaps->vap[i].dim,
			vaps->vap[i].type,
			vaps->vap[i].stride,
			vaps->vap[i].offset
		);
	}
}





int dvaps_get_buffer_stride(dvaps* vaps, int buffer){
	if( !vaps ){ DE_LOG("dvaps object is null, invalid return!\n"); return 0; }
	// todo
	if( buffer >= vaps->num_buffers ){ DE_LOG("buffer %i doesnt exist, invalid return!",buffer); return 0; }
	int i = 0;
	while( buffer != vaps->vap[i].vbo_index ) i++;
	return vaps->vap[i].stride;
}





GLenum dvaps_type_(char c, bool u){
	switch (c){
		case 'i': if(u) return GL_UNSIGNED_INT;   else return GL_INT;
		case 's': if(u) return GL_UNSIGNED_SHORT; else return GL_SHORT;
		case 'c': if(u) return GL_UNSIGNED_BYTE;  else return GL_BYTE;
		case 'f': return GL_FLOAT;
		default: DE_LOG("%s:e:unknown type:%i, must be i,s,c or f , invalid return(-1)\n", __func__, c); return -1;
	}
}





uint dvaps_type_size(GLenum type){
	switch(type){
		case GL_BYTE:
		case GL_UNSIGNED_BYTE:
			return sizeof(GLbyte);
		case GL_SHORT:
		case GL_UNSIGNED_SHORT:
			return sizeof(GLshort);
		case GL_INT:
		case GL_UNSIGNED_INT:
			return sizeof(GLint);
		case GL_FLOAT:
			return sizeof(GLfloat);
		default: return 0;
	}
}





char* dvaps_minify(char* format){
	if( format == NULL ) return NULL;
	int len = strlen(format);
	char* tmp = alloca(len);

	int valid = 0;
	char prev_char = 0;

	for( int i = 0; i<len; i++ ){
		char c = format[i];

		if (c == 0) break;

		if( isspace(c) ) continue;

		bool a = isalnum(c);
		if( (!a) && (c!=',') && (c!=';') ){
			continue;
		}

		if( (c==',') || (c==';') )
			if( c == prev_char )
				continue;

		prev_char = c;
		tmp[valid] = c;
		valid++;
	}
	// cant end in , or ; ...
	if( tmp[valid-1] ==',' || tmp[valid-1] ==';' ){
		valid--;
	}

	tmp[valid] = '\0';
	// return strndup(tmp, len); windows doest have strndup
	return strdup(tmp);
}

















/*
                             PPPPPP   AAAAA   RRRRRR    SSSSSS  EEEEEEE  RRRRRR
                             PP  PP  AA   AA  RR   RR  SS       EE       RR   RR
                             PPPPPP  AAAAAAA  RRRRR      SSS    EEEE     RRRRR
                             PP      AA   AA  RR   RR       SS  EE       RR   RR
                             PP      AA   AA  RR   RR  SSSSSS   EEEEEEE  RR   RR
*///vertex attrib pointer format main parse function~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// insecure parser ahead! (unless string perfectly formed, read header)
int dvaps_parse(dvaps* vaps, char* format){
	if( !vaps || !format ) return 1;
	int error = 0;

	char* minf = dvaps_minify(format);
	int len = strlen(minf);

	int attr_total = 1;
	int num_buffers= 1;

	for( int i=0; i<len; i++ ){
		char c = minf[i];
		if( (c==';') || (c==',') ) {attr_total++;}
		if( c==';' ){
			num_buffers++;
		}
	}

	dstr* names[attr_total];
	for( int i=0; i<attr_total; i++ ){
		names[i] = dstr_new(0);
	}

	dvap* vap = malloc( sizeof(dvap) * attr_total );
	memset(vap, 0, sizeof(dvap) * attr_total);

	vaps->vap = vap;
	vaps->len = attr_total;
	vaps->num_buffers = num_buffers;

	int attr_stride[num_buffers];
	for( int i=0; i<num_buffers; i++ ) {
		attr_stride[i] = 0;
		vaps->vap[i].normalize = GL_FALSE;
	}

	#define _Stype 0 // state attrib type and dimension
	#define _Sname 1 // state attrib name and division

	int state = 0;
	int attr = 0;
	int buffer = 0;
	int offset = 0;
	int size = 0;
	bool u = 0; // unsigned
	char type = 0;

	for( int i=0; i<len; i++ ){
		if( attr > attr_total )
			break; // something went wrong !

		char c = minf[i];

		switch(state){
			case _Stype:
				if( c=='u' ){ u = true; break; }
				else if( c=='n' ){ vap[attr].normalize = GL_TRUE; break; }
				else if( !isdigit(c) ){
					type = c; break;
				}
				else { // terminate
					state = _Sname;
					vap[attr].type = dvaps_type_( type, u );
					vap[attr].dim = atoi( &minf[i] );
					vap[attr].offset = offset;
					size = dvaps_type_size( vap[attr].type );
					offset += size * vap[attr].dim;
					attr_stride[buffer] = offset;
				}
				break;

			case _Sname:
				if( (c==',') || (c==';') || (i==len-1) ){
					vap[attr].vbo_index = buffer;
					if( c==';' ) {
						offset=0;
						buffer++;
					}
					if( i==len-1 ) dstr_add( names[attr], c );

					state = _Stype;
					u = 0; // unsigned
					type = 0;
					attr++;
					break;
				}
				dstr_add( names[attr], c );
				break;
		}
	}
	#undef _Stype
	#undef _Sname


	for( int i = 0; i<attr_total; i++){

		vaps->vap[i].name = strdup( names[i]->data );

		dstr_free( names[i] );

		vaps->vap[i].stride = attr_stride[ vaps->vap[i].vbo_index ];
	}


	free( minf );
	return error;
}






























dvbo* dvbo_new(uint isize, void* idata, GLenum usage){
	if(usage == 0)
		usage = DE_GL_DEFAULT_VBO_USAGE;
	dvbo* vbo = malloc(sizeof(dvbo));
	vbo->ref = 0;
	vbo->usage = usage;
	vbo->size = isize;
	glGenBuffers(1, &(vbo->id));
	dvbo_data(vbo, idata, isize, usage);
	return vbo;
}





void dvbo_free(dvbo* vbo){
	if(!vbo) NULL;
	if(vbo->ref){ vbo->ref--; return; }

	glDeleteBuffers(1, &vbo->id);
	free(vbo);
}





void dvbo_bind(dvbo* vbo){
	if(vbo == NULL) printf("vbo is null! \n");
	if(vbo == NULL) return;
	glBindBuffer(GL_ARRAY_BUFFER, vbo->id);
}





void dvbo_data(dvbo* vbo, void* data, uint size, GLenum usage){
	if (vbo == NULL) printf("vbo data is null! \n");;
	if (vbo == NULL) return;
	if (usage == 0)
		usage = DE_GL_DEFAULT_VBO_USAGE;
	vbo->usage = usage;
	dvbo_bind(vbo);
	glBufferData(GL_ARRAY_BUFFER, size, data, vbo->usage);

}





void dvbo_set(dvbo* vbo, uint offset, void* data, uint size){
	if (vbo == NULL) return;
	// vbo->size = size;
	dvbo_bind(vbo);
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}





void dvbo_set_usage(dvbo* vbo, GLenum usage){
	if(!vbo) return;
	if(usage == 0) usage = DE_GL_DEFAULT_VBO_USAGE;
	vbo->usage = usage;
}


























dibo* dibo_new(short* src, int len, GLenum mode){
	dibo* ibo = malloc(sizeof(dibo));
	ibo->len = len;
	ibo->mode = mode;

	glGenBuffers(1, &ibo->id);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo->id);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		len*sizeof(short), (char*)src, GL_STATIC_DRAW);
	return ibo;
}





void dibo_free(dibo* ibo){
	glDeleteBuffers(1, &ibo->id);
	free(ibo);
}





void dibo_draw(dibo* ibo, int start, int count, GLenum mode){ if(!ibo) return;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo->id);
	uintptr_t first = start;
	if(!mode) mode = ibo->mode;
	// glDrawElements(mode, count, GL_UNSIGNED_INT, (void*)first);
	glDrawElements(mode, count, GL_UNSIGNED_SHORT, (void*)first);
}




























// tools/extra
static float Dvao_draw_quad_verts[] = {
	-1.0, -1.0,
	 1.0, -1.0,
	-1.0,  1.0,
	 1.0,  1.0
};
void dvao_draw_quad(void){ if(!De.shd_bound) return;
	void* vecs[1] = {Dvao_draw_quad_verts};
	dvao* vao = dvao_new("f2pos", vecs, 4, GL_TRIANGLE_STRIP);
	dvao_draw(vao, 0,4,0);
	dvao_free(vao);
}
