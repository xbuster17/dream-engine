// #include "../libs.h"
// #include "../gl/vao.h"
#include "obj_loader.h"

dvao* dmesh_open_obj(char* file){
// dvao* dload_obj(char* file){

	// DE_LOG("loading asset %s", file);

// void mesh__obj_parseff(mesh_t* out__mesh, char* file_path){

	dvao* ret = NULL;

	// void* vvecs[16];

/*open file*/
	SDL_RWops* obj = NULL;
	obj = SDL_RWFromFile(file , "r");
	if (!obj) {
		printf("%s\n", SDL_GetError());
		return ret;
	}
	size_t size = obj->size(obj);
	char* src = malloc(sizeof(char)* size + 1);
	if (!obj->read(obj, src, size, 1)){
		printf("%s\n", SDL_GetError());
		obj->close(obj);
		free(src);
		return ret;
	}
	obj->close(obj);
	src[size] = '\0';




/* commence parse */
/* commence parse */
/* commence parse */


	uint i = 0;
	uint vcount = 0;
	uint vncount = 0;
	uint vtcount = 0;
	uint ecount = 0;


	/**/
	/*count buffers (fixme \/ there.. test me now would break in case of "v " or "vn " or "vt " or "f " substrings) */
	while (i < size){
		if(src[i] == '#') goto skip_line;
		if (src[i] == 'v'){ i++;
			if (src[i] == ' ')
				vcount++;
			if (src[i] == 'n'){ i++;
				if (src[i] == ' ')
					vncount++;
			}
			if (src[i] == 't'){ i++;
				if (src[i] == ' ')
					vtcount++;
			}
		}
		if (src[i] == 'f'){ i++;
			if (src[i] == ' ')
				ecount++;
		}
		i++;
		continue;
		skip_line:
		while((i<size) && src[i] != '\n') i++;
	}

bool has_normals = false;
bool has_texcoords = false;
if(vncount) has_normals    = true;
if(vtcount) has_texcoords = true;

	float* vec = malloc( (1+vcount) * 4 * sizeof(float));
	float* nvec = malloc((1+vncount) * 4 * sizeof(float));
	float* tvec = malloc((1+vtcount) * 2 * sizeof(float));
	unsigned short* ele = malloc(ecount * 3 * sizeof(unsigned short));
	unsigned short* nele = malloc(ecount * 3 * sizeof(unsigned short));
	unsigned short* tele = malloc(ecount * 3 * sizeof(unsigned short));

	uint vi  = 0;
	uint vni = 0;
	uint vti = 0;
	uint ei  = 0;

	/*load buffers*/
	i = 0;
	while (i < size){
		if (src[i] == 'v'){ i++;
			if (src[i] == ' '){ i++; /*4 vertices*/
				vec[vi] = atof(&src[i]);
				while (src[i] != ' ') i++;
				vi++; i++;
				vec[vi] = atof(&src[i]);
				while (src[i] != ' ') i++;
				vi++; i++;
				vec[vi] = atof(&src[i]);
				vi++; vec[vi] = 1.0; vi++;
				//vi+=4; /*room for normals*/
			} else if (src[i] == 'n'){ i++;
					if (src[i] == ' '){ i++; /*4 normals*/
						nvec[vni] = atof(&src[i]);
						while (src[i] != ' ') i++;
						vni++; i++;
						nvec[vni] = atof(&src[i]);
						while (src[i] != ' ') i++;
						vni++; i++;
						nvec[vni] = atof(&src[i]);
						vni++; nvec[vni] = 0.0; vni++; continue;
					}
			} else if (src[i] == 't'){ i++;
				if (src[i] == ' '){ i++; /*2 texcoords*/
					tvec[vti] = atof(&src[i]);
					while (src[i] != ' ') i++;
					vti++; i++;
					tvec[vti] = atof(&src[i]);
					vti++; continue;
				}
			}
		}
		if (src[i] == 'f'){ i++;
			// if (src[i] == ' '){ i++; /*3 indices, 3 normal indices*/
			// 	ele[ei] = atoi(&src[i])-1;
			// 	while (src[i] != '/') i++;
			// 	while (src[i] == '/') i++;
			// 	nele[ei] = atoi(&src[i])-1;
			// 	while (src[i] != ' ') i++;
			// 	ei++; ei++;
			// 	ele[ei] = atoi(&src[i])-1;
			// 	while (src[i] != '/') i++;
			// 	while (src[i] == '/') i++;
			// 	nele[ei] = atoi(&src[i])-1;
			// 	while (src[i] != ' ') i++;
			// 	ei++; ei++;
			// 	ele[ei] = atoi(&src[i])-1;
			// 	while (src[i] != '/') i++;
			// 	while (src[i] == '/') i++;
			// 	nele[ei] = atoi(&src[i])-1;
			// 	ei++; ei++;
			// }
			if (src[i] == ' '){ i++; /*3 indices, 3 normal indices, 3 texcords*/
				ele[ei] = atoi(&src[i]);
				if(has_texcoords){
					while (src[i] != '/') i++;
					i++;
					tele[ei] = atoi(&src[i]);
				}
				if(has_normals){
					while (src[i] != '/') i++;
					i++;
					nele[ei] = atoi(&src[i]);
				}
				ei++;

				while (src[i] != ' ') i++;
				i++;

				ele[ei] = atoi(&src[i]);
				if(has_texcoords){
					while (src[i] != '/') i++;
					i++;
					tele[ei] = atoi(&src[i]);
				}
				if(has_normals){
					while (src[i] != '/') i++;
					i++;
					nele[ei] = atoi(&src[i]);
				}
				ei++;

				while (src[i] != ' ') i++;
				i++;

				ele[ei] = atoi(&src[i]);
				if(has_texcoords){
					while (src[i] != '/') i++;
					i++;
					tele[ei] = atoi(&src[i]);
				}
				if(has_normals){
					while (src[i] != '/') i++;
					i++;
					nele[ei] = atoi(&src[i]);
				}
				ei++;
			}
		}
		i++;
	}


char* format_string = NULL;
float* vvec = NULL;
// float* vvec = malloc(sizeof(float)* 10 * (ei+1) );

if((!has_normals) && (!has_texcoords)){
	/*make pos*/
	format_string = "f4p";

	vvec = malloc(sizeof(float)* 4 * (ei+1) );
	for (i = 0; i < ei; i++){ /*ei and ei should be the same still*/
		vvec[(i * 4)+ 0] =  vec[( (ele[i]-1)* 4) + 0];
		vvec[(i * 4)+ 1] =  vec[( (ele[i]-1)* 4) + 1];
		vvec[(i * 4)+ 2] =  vec[( (ele[i]-1)* 4) + 2];
		vvec[(i * 4)+ 3] =  vec[( (ele[i]-1)* 4) + 3];
	}
} else if ((!has_normals) && has_texcoords){
	/*make texc*/
	format_string = "f4p, f2tc";

	vvec = malloc(sizeof(float)* 6 * (ei+1) );
	for (i = 0; i < ei; i++){ /*ei and ei should be the same still*/
		vvec[(i * 6)+ 0] =  vec[( (ele[i]-1)* 4) + 0];
		vvec[(i * 6)+ 1] =  vec[( (ele[i]-1)* 4) + 1];
		vvec[(i * 6)+ 2] =  vec[( (ele[i]-1)* 4) + 2];
		vvec[(i * 6)+ 3] =  vec[( (ele[i]-1)* 4) + 3];
		vvec[(i * 6)+ 4] = tvec[(((tele[i]-1 )* 2) + 0 )];
		vvec[(i * 6)+ 5] = tvec[(((tele[i]-1 )* 2) + 1 )];
	}
} else if (has_normals && (!has_texcoords)){
	/*make normals*/
	format_string = "f4p, f4n";

	vvec = malloc(sizeof(float)* 8 * (ei+1) );
	for (i = 0; i < ei; i++){ /*ei and ei should be the same still*/
		vvec[(i * 8)+ 0] =  vec[( (ele[i]-1)* 4) + 0];
		vvec[(i * 8)+ 1] =  vec[( (ele[i]-1)* 4) + 1];
		vvec[(i * 8)+ 2] =  vec[( (ele[i]-1)* 4) + 2];
		vvec[(i * 8)+ 3] =  vec[( (ele[i]-1)* 4) + 3];
		// vvec[(i * 4)+ 3] = nvec[(nele[i]* 4) + i%3];
		vvec[(i * 8)+ 4] = nvec[((nele[i]-1)* 4) + 0];
		vvec[(i * 8)+ 5] = nvec[((nele[i]-1)* 4) + 1];
		vvec[(i * 8)+ 6] = nvec[((nele[i]-1)* 4) + 2];
		vvec[(i * 8)+ 7] = nvec[((nele[i]-1)* 4) + 3];
	}
} else if (has_normals && has_texcoords){
	/*make normals texc*/
	format_string = "f4p, f4n, f2tc";

	vvec = malloc(sizeof(float)* 10 * (ei+1) );
	for (i = 0; i < ei; i++){
		vvec[(i * 10)+ 0] =  vec[(( (ele[i]-1 )* 4u) + 0u )];
		vvec[(i * 10)+ 1] =  vec[(( (ele[i]-1 )* 4u) + 1u )];
		vvec[(i * 10)+ 2] =  vec[(( (ele[i]-1 )* 4u) + 2u )];
		vvec[(i * 10)+ 3] =  vec[(( (ele[i]-1 )* 4u) + 3u )];
		vvec[(i * 10)+ 4] = nvec[(((nele[i]-1 )* 4u) + 0u )];
		vvec[(i * 10)+ 5] = nvec[(((nele[i]-1 )* 4u) + 1u )];
		vvec[(i * 10)+ 6] = nvec[(((nele[i]-1 )* 4u) + 2u )];
		vvec[(i * 10)+ 7] = nvec[(((nele[i]-1 )* 4u) + 3u )];
		vvec[(i * 10)+ 8] = tvec[(((tele[i]-1 )* 2u) + 0u )];
		vvec[(i * 10)+ 9] = tvec[(((tele[i]-1 )* 2u) + 1u )];
	}
}

	// // vbo__create(&out->vbo, ((ei+1)* 8)* sizeof(float), (char*)vvec);
	// ret.tris = deArrNew(sizeof(deTri), ei/2, NULL);
	// deTri tri;
	// for(i=0; i < ei/3; i++){
	// 	tri.p0 = deVec4f(
	// 		vvec[(i * 4*3)+ 0],
	// 		vvec[(i * 4*3)+ 1],
	// 		vvec[(i * 4*3)+ 2],
	// 		vvec[(i * 4*3)+ 3]
	// 	);
	// 	tri.p1 = deVec4f(
	// 		vvec[(i * 4*3)+ 4],
	// 		vvec[(i * 4*3)+ 5],
	// 		vvec[(i * 4*3)+ 6],
	// 		vvec[(i * 4*3)+ 7]
	// 	);
	// 	tri.p2 = deVec4f(
	// 		vvec[(i * 4*3)+ 8],
	// 		vvec[(i * 4*3)+ 9],
	// 		vvec[(i * 4*3)+10],
	// 		vvec[(i * 4*3)+11]
	// 	);
	// 	// tri.col = DE_GL_DEF_COL;
	// 	tri.col = deV4fRand() / (float)INT_MAX;
	// 	tri.col[3] = .5;
	// 	deArrAdd(ret.tris, &tri);
	// }


	// free(nvec);
	// free(nele);
	// free(vec);
	// free(ele);



	// ret = deVaoNew((char*)vvec, format_string, ei);

	void* vvecs[2] = {
		vvec,
		NULL
	};

// for(int i=0; i< 10 * (ei+1); i++){
// 	printf("%f \n", vvec[i]);
// }

	ret = dvao_newr(format_string, vvecs, ei, GL_TRIANGLES);

	free(vvec);
	free(vec);
	free(nvec);
	free(tvec);
	free(ele);
	free(nele);
	free(tele);

	free(src);

	// ret.pos = deVec4f(0,0,0,1);
	// ret.rot = deVec4f(0,0,0,1);
	// ret.sca = deVec4f(1,1,1,1);

	// ret.mvp = deM4fID();

	return ret;
}




