#include "vector.h"

/* extract vector from matrix  */
#define M4_V4()           \
  v1[0],v1[1],v1[2],v1[3],\
  v2[0],v2[1],v2[2],v2[3],\
  v3[0],v3[1],v3[2],v3[3],\
  v4[0],v4[1],v4[2],v4[3],
m4d m4d_v4d(v4d v1, v4d v2, v4d v3, v4d v4){return (m4d){M4_V4()};}
m4f m4f_v4f(v4f v1, v4f v2, v4f v3, v4f v4){return (m4f){M4_V4()};}
m4i m4i_v4i(v4i v1, v4i v2, v4i v3, v4i v4){return (m4i){M4_V4()};}
m4c m4c_v4c(v4c v1, v4c v2, v4c v3, v4c v4){return (m4c){M4_V4()};}
#undef M4_V4
v4f v4f_m4f(m4f A, uint8_t which){
	if (which > 3) which = 3;
	return (v4f){
		A[0 + (4 * which)],
		A[1 + (4 * which)],
		A[2 + (4 * which)],
		A[3 + (4 * which)],
	};
}


/* PRINT */
#include <stdio.h>
#define V4_PRINT_BODY(s) s" \t"s" \t"s" \t"s"\n",v[0],v[1],v[2],v[3]
#define M2_PRINT_BODY(s) s" \t"s"\n"s" \t"s"\n",A[0],A[1],A[2],A[3]
#define V2_PRINT_BODY(s) s" \t"s"\n",v[0],v[1]
#define M4_PRINT_BODY(s)\
  s" \t"s" \t"s" \t"s"\n"     \
  s" \t"s" \t"s" \t"s"\n"     \
  s" \t"s" \t"s" \t"s"\n"     \
  s" \t"s" \t"s" \t"s"\n",    \
  A[0],A[1],A[2],A[3],  \
  A[4],A[5],A[6],A[7],  \
  A[8],A[9],A[10],A[11],\
  A[12],A[13],A[14],A[15]
void m4d_print(m4d A){printf(M4_PRINT_BODY("%.5f"));}
void m4f_print(m4f A){printf(M4_PRINT_BODY("%.3f"));}
void m4i_print(m4i A){printf(M4_PRINT_BODY("%i"));}
void m4c_print(m4c A){printf(M4_PRINT_BODY("%c"));}
void v4d_print(v4d v){printf(V4_PRINT_BODY("%.5f"));}
void v4f_print(v4f v){printf(V4_PRINT_BODY("%.3f"));}
void v4i_print(v4i v){printf(V4_PRINT_BODY("%i"));}
void v4c_print(v4c v){printf(V4_PRINT_BODY("%c"));}
void m2d_print(m2d A){printf(M2_PRINT_BODY("%.5f"));}
void m2f_print(m2f A){printf(M2_PRINT_BODY("%.3f"));}
void m2i_print(m2i A){printf(M2_PRINT_BODY("%i"));}
void m2c_print(m2c A){printf(M2_PRINT_BODY("%c"));}
void v2d_print(v2d v){printf(V2_PRINT_BODY("%.5f"));}
void v2f_print(v2f v){printf(V2_PRINT_BODY("%.3f"));}
void v2i_print(v2i v){printf(V2_PRINT_BODY("%i"));}
void v2c_print(v2c v){printf(V2_PRINT_BODY("%c"));}
#undef M4_PRINT_BODY
#undef V4_PRINT_BODY
#undef M2_PRINT_BODY
#undef V2_PRINT_BODY


/* RANDOM */
#define V4_RAND() {rand(),rand(),rand(),rand()}
#define V3_RAND() {rand(),rand(),rand(),0}
#define V2_RAND() {rand(),rand()}
v4d v4d_rand(void){return (v4d)V4_RAND()/(double)RAND_MAX;}
v4f v4f_rand(void){return (v4f)V4_RAND()/(float)RAND_MAX;}
v4i v4i_rand(void){return (v4i)V4_RAND();}
v4c v4c_rand(void){return (v4c)V4_RAND();}
v4d v3d_rand(void){return (v4d)V3_RAND()/(double)RAND_MAX;}
v4f v3f_rand(void){return (v4f)V3_RAND()/(float)RAND_MAX;}
v4i v3i_rand(void){return (v4i)V3_RAND();}
v4c v3c_rand(void){return (v4c)V3_RAND();}
v2d v2d_rand(void){return (v2d)V2_RAND()/(double)RAND_MAX;}
v2f v2f_rand(void){return (v2f)V2_RAND()/(float)RAND_MAX;}
v2i v2i_rand(void){return (v2i)V2_RAND();}
v2c v2c_rand(void){return (v2c)V2_RAND();}


/* DOT PRODUCT */
double v4d_dot(v4d v, v4d w){v *= w; return v[0] + v[1] + v[2] + v[3];}
float  v4f_dot(v4f v, v4f w){v *= w; return v[0] + v[1] + v[2] + v[3];}
float  v4i_dot(v4i v, v4i w){v *= w; return v[0] + v[1] + v[2] + v[3];}
float  v4c_dot(v4c v, v4c w){v *= w; return v[0] + v[1] + v[2] + v[3];}
double v3d_dot(v4d v, v4d w){v *= w; return v[0] + v[1] + v[2];}
float  v3f_dot(v4f v, v4f w){v *= w; return v[0] + v[1] + v[2];}
float  v3i_dot(v4i v, v4i w){v *= w; return v[0] + v[1] + v[2];}
float  v3c_dot(v4c v, v4c w){v *= w; return v[0] + v[1] + v[2];}
double v2d_dot(v2d v, v2d w){v *= w; return v[0] + v[1];}
float  v2f_dot(v2f v, v2f w){v *= w; return v[0] + v[1];}
float  v2i_dot(v2i v, v2i w){v *= w; return v[0] + v[1];}
float  v2c_dot(v2c v, v2c w){v *= w; return v[0] + v[1];}


/* CROSS PRODUCT */
#define V4_CROSS_BODY(res,v1,v2) \
	res[0] = (v1[1]* v2[2]- v1[2]* v2[1]);\
	res[1] = (v1[2]* v2[0]- v1[0]* v2[2]);\
	res[2] = (v1[0]* v2[1]- v1[1]* v2[0]);\
	res[3] = 0;
v4d v3d_cross(v4d v1, v4d v2){v4d res; V4_CROSS_BODY(res,v1,v2); return res;}
v4f v3f_cross(v4f v1, v4f v2){v4f res; V4_CROSS_BODY(res,v1,v2); return res;}
v4i v3i_cross(v4i v1, v4i v2){v4i res; V4_CROSS_BODY(res,v1,v2); return res;}
v4c v3c_cross(v4c v1, v4c v2){v4c res; V4_CROSS_BODY(res,v1,v2); return res;}
#undef V4_CROSS_BODY


/* VECTOR MATRIX MULTIPLICATION */
#define V4_MMUL()\
	v[0]*m[0] + v[1]*m[4] + v[2]*m[8]  + v[3]*m[12],\
	v[0]*m[1] + v[1]*m[5] + v[2]*m[9]  + v[3]*m[13],\
	v[0]*m[2] + v[1]*m[6] + v[2]*m[10] + v[3]*m[14],\
	v[0]*m[3] + v[1]*m[7] + v[2]*m[11] + v[3]*m[15],
#define V4_MTMUL()\
	m[0]*v[0] + m[1]*v[1] + m[2]*v[2] + m[3]*v[3],\
	m[4]*v[0] + m[5]*v[1] + m[6]*v[2] + m[7]*v[3],\
	m[8]*v[0] + m[9]*v[1] + m[10]*v[2] + m[11]*v[3],\
	m[12]*v[0] + m[13]*v[1] + m[14]*v[2] + m[15]*v[3],
v4d v4d_mmul(v4d v, m4d m){v4d res = {V4_MMUL()}; return res;}
v4f v4f_mmul(v4f v, m4f m){v4f res = {V4_MMUL()}; return res;}
v4i v4i_mmul(v4i v, m4i m){v4i res = {V4_MMUL()}; return res;}
v4c v4c_mmul(v4c v, m4c m){v4c res = {V4_MMUL()}; return res;}
v4d v4d_mtmul(v4d v, m4d m){v4d res = {V4_MTMUL()}; return res;}
v4f v4f_mtmul(v4f v, m4f m){v4f res = {V4_MTMUL()}; return res;}
v4i v4i_mtmul(v4i v, m4i m){v4i res = {V4_MTMUL()}; return res;}
v4c v4c_mtmul(v4c v, m4c m){v4c res = {V4_MTMUL()}; return res;}
#define V2_MMUL()\
	v[0]*m[0] + v[1]*m[2],\
	v[0]*m[1] + v[1]*m[3],
#define V2_MTMUL()\
	m[0]*v[0] + m[1]*v[1],\
	m[2]*v[0] + m[3]*v[1],
v2d v2d_mmul(v2d v, m2d m){v2d res = {V2_MMUL()}; return res;}
v2f v2f_mmul(v2f v, m2f m){v2f res = {V2_MMUL()}; return res;}
v2i v2i_mmul(v2i v, m2i m){v2i res = {V2_MMUL()}; return res;}
v2c v2c_mmul(v2c v, m2c m){v2c res = {V2_MMUL()}; return res;}
v2d v2d_mtmul(v2d v, m2d m){v2d res = {V2_MTMUL()}; return res;}
v2f v2f_mtmul(v2f v, m2f m){v2f res = {V2_MTMUL()}; return res;}
v2i v2i_mtmul(v2i v, m2i m){v2i res = {V2_MTMUL()}; return res;}
v2c v2c_mtmul(v2c v, m2c m){v2c res = {V2_MTMUL()}; return res;}
#undef V4_MMUL
#undef V4_MTMUL
#undef V2_MMUL
#undef V2_MTMUL

/* IDENTITY MATRIX */
#define M4_ID()\
  1, 0, 0, 0,  \
  0, 1, 0, 0,  \
  0, 0, 1, 0,  \
  0, 0, 0, 1,
#define M2_ID()\
  1, 0,        \
  0, 1,
m4d m4d_id(void){ return (m4d){M4_ID()};}
m4f m4f_id(void){ return (m4f){M4_ID()};}
m4i m4i_id(void){ return (m4i){M4_ID()};}
m4c m4c_id(void){ return (m4c){M4_ID()};}
m2d m2d_id(void){ return (m2d){M2_ID()};}
m2f m2f_id(void){ return (m2f){M2_ID()};}
m2i m2i_id(void){ return (m2i){M2_ID()};}
m2c m2c_id(void){ return (m2c){M2_ID()};}
#undef M4_ID
#undef M2_ID

/* MATRIX MATRIX MULTIPLICATION */
#define M4_MUL()\
	m1[0]* m2[0]+ m1[1]* m2[4]+ m1[2]* m2[8]+ m1[3]* m2[12],     \
	m1[0]* m2[1]+ m1[1]* m2[5]+ m1[2]* m2[9]+ m1[3]* m2[13],     \
	m1[0]* m2[2]+ m1[1]* m2[6]+ m1[2]* m2[10]+ m1[3]* m2[14],    \
	m1[0]* m2[3]+ m1[1]* m2[7]+ m1[2]* m2[11]+ m1[3]* m2[15],    \
	m1[4]* m2[0]+ m1[5]* m2[4]+ m1[6]* m2[8]+ m1[7]* m2[12],     \
	m1[4]* m2[1]+ m1[5]* m2[5]+ m1[6]* m2[9]+ m1[7]* m2[13],     \
	m1[4]* m2[2]+ m1[5]* m2[6]+ m1[6]* m2[10]+ m1[7]* m2[14],    \
	m1[4]* m2[3]+ m1[5]* m2[7]+ m1[6]* m2[11]+ m1[7]* m2[15],    \
	m1[8]* m2[0]+ m1[9]* m2[4]+ m1[10]* m2[8]+ m1[11]* m2[12],   \
	m1[8]* m2[1]+ m1[9]* m2[5]+ m1[10]* m2[9]+ m1[11]* m2[13],   \
	m1[8]* m2[2]+ m1[9]* m2[6]+ m1[10]* m2[10]+ m1[11]* m2[14],  \
	m1[8]* m2[3]+ m1[9]* m2[7]+ m1[10]* m2[11]+ m1[11]* m2[15],  \
	m1[12]* m2[0]+ m1[13]* m2[4]+ m1[14]* m2[8]+ m1[15]* m2[12], \
	m1[12]* m2[1]+ m1[13]* m2[5]+ m1[14]* m2[9]+ m1[15]* m2[13], \
	m1[12]* m2[2]+ m1[13]* m2[6]+ m1[14]* m2[10]+ m1[15]* m2[14],\
	m1[12]* m2[3]+ m1[13]* m2[7]+ m1[14]* m2[11]+ m1[15]* m2[15],
m4d m4d_mul(m4d m1, m4d m2){ m4d res = {M4_MUL()}; return res;}
m4f m4f_mul(m4f m1, m4f m2){ m4f res = {M4_MUL()}; return res;}
m4i m4i_mul(m4i m1, m4i m2){ m4i res = {M4_MUL()}; return res;}
m4c m4c_mul(m4c m1, m4c m2){ m4c res = {M4_MUL()}; return res;}
#define M2_MUL()\
	m1[0]*m2[0]+m1[1]*m2[2],\
	m1[0]*m2[1]+m1[1]*m2[3],\
	m1[2]*m2[0]+m1[3]*m2[2],\
	m1[2]*m2[1]+m1[3]*m2[3],
m2d m2d_mul(m2d m1, m2d m2){m2d res = {M2_MUL()}; return res;}
m2f m2f_mul(m2f m1, m2f m2){m2f res = {M2_MUL()}; return res;}
m2i m2i_mul(m2i m1, m2i m2){m2i res = {M2_MUL()}; return res;}
m2c m2c_mul(m2c m1, m2c m2){m2c res = {M2_MUL()}; return res;}
#undef M4_MUL
#undef M2_MUL

/* VECTOR LENGTH */
double v4d_len(v4d v1){ return sqrt(v4d_dot(v1,v1)); }
float  v4f_len(v4f v1){ return sqrt(v4f_dot(v1,v1)); }
float  v4i_len(v4i v1){ return sqrt(v4i_dot(v1,v1)); }
float  v4c_len(v4c v1){ return sqrt(v4c_dot(v1,v1)); }
double v3d_len(v4d v1){ return sqrt(v3d_dot(v1,v1)); }
float  v3f_len(v4f v1){ return sqrt(v3f_dot(v1,v1)); }
float  v3i_len(v4i v1){ return sqrt(v3i_dot(v1,v1)); }
float  v3c_len(v4c v1){ return sqrt(v3c_dot(v1,v1)); }
double v2d_len(v2d v1){ return sqrt(v2d_dot(v1,v1)); }
float  v2f_len(v2f v1){ return sqrt(v2f_dot(v1,v1)); }
float  v2i_len(v2i v1){ return sqrt(v2i_dot(v1,v1)); }
float  v2c_len(v2c v1){ return sqrt(v2c_dot(v1,v1)); }

double v4d_len2(v4d v1){ return v4d_dot(v1,v1); }
float  v4f_len2(v4f v1){ return v4f_dot(v1,v1); }
float  v4i_len2(v4i v1){ return v4i_dot(v1,v1); }
float  v4c_len2(v4c v1){ return v4c_dot(v1,v1); }
double v3d_len2(v4d v1){ return v3d_dot(v1,v1); }
float  v3f_len2(v4f v1){ return v3f_dot(v1,v1); }
float  v3i_len2(v4i v1){ return v3i_dot(v1,v1); }
float  v3c_len2(v4c v1){ return v3c_dot(v1,v1); }
double v2d_len2(v2d v1){ return v2d_dot(v1,v1); }
float  v2f_len2(v2f v1){ return v2f_dot(v1,v1); }
float  v2i_len2(v2i v1){ return v2i_dot(v1,v1); }
float  v2c_len2(v2c v1){ return v2c_dot(v1,v1); }

/* VECTOR NORMALIZE */
v4d v4d_normalize(v4d v1){double l=v4d_len(v1); double s = l? 1.f/l :0; v4d ret = v1 * s; return ret;}
v4d v3d_normalize(v4d v1){double l=v3d_len(v1); double s = l? 1.f/l :0; v4d ret = v1 * s; return ret;}
v2d v2d_normalize(v2d v1){double l=v2d_len(v1); double s = l? 1.f/l :0; v2d ret = v1 * s; return ret;}
v4f v4f_normalize(v4f v1){float l=v4f_len(v1); float s = l? 1.f/l :0; v4f ret = v1 * s; return ret;}
v4f v3f_normalize(v4f v1){float l=v3f_len(v1); float s = l? 1.f/l :0; v4f ret = v1 * s; return ret;}
v2f v2f_normalize(v2f v1){float l=v2f_len(v1); float s = l? 1.f/l :0; v2f ret = v1 * s; return ret;}

/* ANGLE BETWEEN VECTORS */
double v3d_ang(v4d v1, v4d v2){//Image:[0,PI]
	v1[3]=0; v2[3]=0;
	return acos(v4d_dot(v1,v2)/(v4d_len(v1)*v4d_len(v2)));
}
float v3f_ang(v4f v1, v4f v2){//Image:[0,PI]
	v1[3]=0; v2[3]=0;
	return acosf(v4f_dot(v1,v2)/(v4f_len(v1)*v4f_len(v2)));
}
double v4d_ang(v4d v1, v4d v2){//Image:[0,PI]
	return acos(v4d_dot(v1,v2)/(v4d_len(v1)*v4d_len(v2)));
}
float v4f_ang(v4f v1, v4f v2){//Image:[0,PI]
	return acosf(v4f_dot(v1,v2)/(v4f_len(v1)*v4f_len(v2)));
}

v4f v4f_reflect(v4f v, v4f n){
	return v -2.f * (float)v4f_dot(v, n) * n;
}

v4d v4d_reflect(v4d v, v4d n){
	return v -2.0 * v4d_dot(v, n) * n;
}


/* TRANSFORMATION MATRICES */
// m4f m4fScale(m4f mat, v4f scale){
// 	m4f scaler = {
// 		scale[0],0,0,0,
// 		0,scale[1],0,0,
// 		0,0,scale[2],0,
// 		0,0,0,scale[3],
// 	};
// 	return m4fMul(scaler, mat);
// }
m4f m4f_scale(m4f mat, v4f scale){
	m4f scaler = {
		scale[0],0,0,0,
		0,scale[1],0,0,
		0,0,scale[2],0,
		0,0,0,1,
	};
	return m4f_mul(scaler, mat);
}
m4f m4f_translation(v4f v){
	m4f translation = (m4f){
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		v[0],v[1],v[2],1.0
	};
	return translation;
}
m4f m4f_translate(m4f m, v4f v){
	m4f translation = m4f_translation(v);
	v[3]=1;
	return m4f_mul(translation, m);
}
m4f m4f_rotation(v4f rot){
	m4f ret = m4f_id();
	ret = m4f_rotate_x(ret, rot[0]);
	ret = m4f_rotate_y(ret, rot[1]);
	ret = m4f_rotate_z(ret, rot[2]);
	return ret;
}
m4f m4f_rotate(m4f m, v4f rot){
	return m4f_mul(m, m4f_rotation(rot));
}

m4f m4f_rotate_x(m4f m, float theta){if(theta==0)return m; else return m4f_mul(m4f_rotation_x(theta), m);}
m4f m4f_rotate_y(m4f m, float theta){if(theta==0)return m; else return m4f_mul(m4f_rotation_y(theta), m);}
m4f m4f_rotate_z(m4f m, float theta){if(theta==0)return m; else return m4f_mul(m4f_rotation_z(theta), m);}
m4f m4f_rotation_x(float theta){
	return (m4f){
		1.0,          0.0,          0.0, 0.0,
		0.0,  cosf(theta),  sinf(theta), 0.0,
		0.0, -sinf(theta),  cosf(theta), 0.0,
		0.0,          0.0,          0.0, 1.0,
	};
}
m4f m4f_rotation_y(float theta){
	return (m4f){
		cosf(theta),  0.0,  sinf(theta), 0.0,
		0.0,          1.0,          0.0, 0.0,
		-sinf(theta), 0.0,  cosf(theta), 0.0,
		0.0,          0.0,          0.0, 1.0,
	};
}
m4f m4f_rotation_z(float theta){
	return (m4f){
		cosf(theta), -sinf(theta), 0.0, 0.0,
		sinf(theta),  cosf(theta), 0.0, 0.0,
		0.0,                  0.0, 1.0, 0.0,
		0.0,                  0.0, 0.0, 1.0,
	};
}
m2f m2f_rotation(float theta){
	return (m2f){
		cosf(theta), -sinf(theta),
		sinf(theta), cosf(theta)
	};
}
m2f m2f_shear(v2f v){ // ???
	return (m2f){
		1, v[0],
		v[1], 1
	};
}

void m4f_rot_v4f(m4f* m, v4f v1, float radians){
	v4f v1_norm = v4f_normalize(v1);
	double radcos = cos(radians);
	double invradcos = 1.f / radcos;
	double radsin = sin(radians);
	double v01 = v1_norm[0]* v1_norm[1];
	double v02 = v1_norm[0]* v1_norm[2];
	double v12 = v1_norm[1]* v1_norm[2];
	*m = (m4f){
		radcos + invradcos * v1_norm[0]* v1_norm[0],
			invradcos * v01 + v1_norm[2]* radsin,
			invradcos * v02 - v1_norm[1]* radsin,
			0.f,
		invradcos * v01 - v1_norm[2]* radsin,
			radcos + invradcos * v1_norm[1]* v1_norm[1],
			invradcos * v12 + v1_norm[0]* radsin,
			0.f,
		invradcos * v02 + v1_norm[1]* radsin,
			invradcos * v12 - v1_norm[0]* radsin,
			radcos + invradcos * v1_norm[2]* v1_norm[2],
			0.f,
		0.f, 0.f, 0.f, 1.f
	};
}

/* PROJECTION MATRICES */
// #define m4fViewFrustum(...) m4fProj(__VA_ARGS__)
m4f m4f_proj(
	float fov, float aspect_ratio,
	float near, float far
){
	// float h = cosf(0.5f*fov)/sinf(0.5f*fov);
	// float w = h * aspect_ratio;
	// float a = - (near+far)/(near-far);
	// float b = - ((2*far*near)/(far-near));
	// return (m4f){
	// 	w, 0, 0, 0,
	// 	0, h, 0, 0,
	// 	0, 0, a, 1,
	// 	0, 0, b, 0
	// };


  const float h = 1.0f/tan(fov*.5*(M_PI/180.0));
  float neg_depth = near-far;
m4f m=M4F_ID;
  m[0] = h / (1.0/aspect_ratio);
  m[1] = 0;
  m[2] = 0;
  m[3] = 0;

  m[4] = 0;
  m[5] = h;
  m[6] = 0;
  m[7] = 0;

  m[8] = 0;
  m[9] = 0;
  m[10] = (far + near)/neg_depth;
  m[11] = -1;

  m[12] = 0;
  m[13] = 0;
  m[14] = 2.0f*(near*far)/neg_depth;
  m[15] = 0;

  return m;


}


m4f m4f_view(v4f pos, v4f rot){
	m4f m = m4f_id();
	m = m4f_rotate(m, -rot);
	m = m4f_translate(m, -pos);
	return m;
}

	// m4f M = m4f_id();
	// M = m4f_translate(M, m->pos);
	// if(m->rot[2]) M = m4f_rotate_z(M, m->rot[2]);
	// if(m->rot[1]) M = m4f_rotate_y(M, m->rot[1]);
	// if(m->rot[0]) M = m4f_rotate_x(M, m->rot[0]);
	// M = m4f_scale(M, m->sca);

m4f m4f_model(v4f pos, v4f rot, v4f sca){
	m4f m = m4f_id();
	// m = m4f_rotate(m, rot);
	m = m4f_translate(m, pos);
	if(rot[2]) m = m4f_rotate_z(m, rot[2]);
	if(rot[1]) m = m4f_rotate_y(m, rot[1]);
	if(rot[0]) m = m4f_rotate_x(m, rot[0]);
	m = m4f_scale(m, sca);

	return m;
}

m4f m4f_timodel(m4f M){
                               M[3]=0;
                               M[7]=0;
                               M[11]=0;
	M[12]=0; M[13]=0; M[14]=0; M[15]=1;
	return m4f_transpose(m4f_inverse(M));
}


//from wikipedia
m4f m4f_ortho(float left, float right, float top, float bottom, float near, float far){
	return (m4f){
		2.0/(right-left), 0,                0,               -(right+left)/(right-left),
		0,                2.0/(top-bottom), 0,               -(top+bottom)/(top-bottom),
		0,                0,                -2.0/(far-near), -(far+near)/(far-near),
		0,                0,                0,               1,
	};
}

m4f m4f_invortho(float left, float right, float top, float bottom, float near, float far){
	return (m4f){
		(right-left)/2.0, 0,                0,               (right+left)/2.0,
		0,                (top-bottom)/2.0, 0,               (top+bottom)/2.0,
		0,                0,                (far-near)/-2.0, -(far+near)/2.0,
		0,                0,                0,               1,
	};
}

//fixme
m4f m4f_ortho_project(v2f size, float near, float far){
// m4f m4fOrthoProject(float left, float right, float top, float bottom, float near, float far){
	return (m4f){
		2.0/size[0], 0, 0, 0,
		0, 2.0/size[1], 0, 0,
		0, 0, (-2.0)/(far-near), /*-((far+near)/(far-near)),*/ 0,
		0,0,0,1
	};
}


//fixme
m4f m4f_look_at(v4f eye, v4f center, v4f up){
// 	v4f X, Y, Z;
// 	eye[3] = 0;
// 	center[3] = 0;
// 	up[3] = 0;
// // Create a new coordinate system:
// 	Z = eye - center;
// 	Z = v4fNormalize(Z);
// 	Y = up;
// 	X = v4fCross(Y, Z);
// // Recompute Y = Z cross X:
// 	Y = v4fCross(Z, X);
// // Cross-product gives area of parallelogram, which is < 1.0 for non-perpendicular unit-length vectors; so normalize X, Y here:
// 	X = v4fNormalize(X);
// 	Y = v4fNormalize(Y);
// 	m4f ret = {
// 		X[0], X[1], X[2], v4fDot(-X, eye),
// 		Y[0], Y[1], Y[2], v4fDot(-Y, eye),
// 		Z[0], Z[1], Z[2], v4fDot(-Z, eye),
// 		0.0,   0.0,  0.0, 1.0f
// 	};


	// v4f F = center - eye;
	// v4f f = v4f_normalize(F);
	// up = v4f_normalize(up);
	// v4f s = v4f_cross(f,up);
	// v4f u = v4f_cross(v4f_normalize(s),f);
	// m4f ret = {
	// 	 s[0],  s[1],  s[2], 0,
	// 	 u[0],  u[1],  u[2], 0,
	// 	-f[0], -f[1], -f[2], 0,
	// 	    0,     0,     0, 1
	// };
center[3]=0;
eye[3]=0;
up[3]=0;
    v4f f = v3f_normalize(center - eye);
    // v4f f = v3f_normalize(eye - center); // huh?
    v4f u = v3f_normalize(up);
    v4f s = v3f_normalize(v3f_cross(f, u));
    u = v3f_cross(s, f);
m4f ret=m4f_id();
    // mat4x4 Result(1);
    // ret[0][0] = s.x;
    // ret[1][0] = s.y;
    // ret[2][0] = s.z;
    // ret[0][1] = u.x;
    // ret[1][1] = u.y;
    // ret[2][1] = u.z;
    // ret[0][2] =-f.x;
    // ret[1][2] =-f.y;
    // ret[2][2] =-f.z;
    // ret[3][0] =-v3f_dot(s, eye);
    // ret[3][1] =-v3f_dot(u, eye);
    // ret[3][2] = v3f_dot(f, eye
    ret[0*4+0] = s[0]; // right handed
    ret[1*4+0] = s[1];
    ret[2*4+0] = s[2];
    ret[0*4+1] = u[0];
    ret[1*4+1] = u[1];
    ret[2*4+1] = u[2];
    ret[0*4+2] =-f[0];
    ret[1*4+2] =-f[1];
    ret[2*4+2] =-f[2];
    ret[3*4+0] =-v3f_dot(s, eye);
    ret[3*4+1] =-v3f_dot(u, eye);
    ret[3*4+2] = v3f_dot(f, eye);
    // ret[0*4+0] = s[0]; // left handed
    // ret[1*4+0] = s[1];
    // ret[2*4+0] = s[2];
    // ret[0*4+1] = u[0];
    // ret[1*4+1] = u[1];
    // ret[2*4+1] = u[2];
    // ret[0*4+2] = f[0];
    // ret[1*4+2] = f[1];
    // ret[2*4+2] = f[2];
    // ret[3*4+0] =-v3f_dot(s, eye);
    // ret[3*4+1] =-v3f_dot(u, eye);
    // ret[3*4+2] =-v3f_dot(f, eye);
    return ret;
}


/*
	another matrix multiplication
*/
// #include <xmmintrin.h> // incompat android
// void m4fMul(m4f* dst, m4f* A, m4f* B){
// 	__m128 row1 = _mm_load_ps(&(*B)[0]);
// 	__m128 row2 = _mm_load_ps(&(*B)[4]);
// 	__m128 row3 = _mm_load_ps(&(*B)[8]);
// 	__m128 row4 = _mm_load_ps(&(*B)[12]);
// 	for(int i=0; i<4; i++) {
// 		__m128 brod1 = _mm_set1_ps((*A)[4 * i + 0]);
// 		__m128 brod2 = _mm_set1_ps((*A)[4 * i + 1]);
// 		__m128 brod3 = _mm_set1_ps((*A)[4 * i + 2]);
// 		__m128 brod4 = _mm_set1_ps((*A)[4 * i + 3]);
// 		__m128 row = _mm_add_ps(
// 			_mm_add_ps(
// 				mmMulPs(brod1, row1),
// 				mmMulPs(brod2, row2)),
// 			_mm_add_ps(
// 				mmMulPs(brod3, row3),
// 				mmMulPs(brod4, row4)));
// 		_mm_store_ps(&(*dst)[4*i], row);
// 	}
// }

// #define m4Get(matrix,row,column) matrix[((row)*4)+(column)]

m4f m4f_transpose(m4f m){
	int n,i,j;
	m4f t;
	for(n = 0; n < 16; n++) {
		i = n/4;
		j = n%4;
		t[n] = m[4*j + i];
	}
	return t;

}

m4f m4f_inverse(m4f m){
	m4f inv;
	float det;

	inv[0] = m[5]  * m[10] * m[15] -
	             m[5]  * m[11] * m[14] -
	             m[9]  * m[6]  * m[15] +
	             m[9]  * m[7]  * m[14] +
	             m[13] * m[6]  * m[11] -
	             m[13] * m[7]  * m[10];

	    inv[4] = -m[4]  * m[10] * m[15] +
	              m[4]  * m[11] * m[14] +
	              m[8]  * m[6]  * m[15] -
	              m[8]  * m[7]  * m[14] -
	              m[12] * m[6]  * m[11] +
	              m[12] * m[7]  * m[10];

	    inv[8] = m[4]  * m[9] * m[15] -
	             m[4]  * m[11] * m[13] -
	             m[8]  * m[5] * m[15] +
	             m[8]  * m[7] * m[13] +
	             m[12] * m[5] * m[11] -
	             m[12] * m[7] * m[9];

	    inv[12] = -m[4]  * m[9] * m[14] +
	               m[4]  * m[10] * m[13] +
	               m[8]  * m[5] * m[14] -
	               m[8]  * m[6] * m[13] -
	               m[12] * m[5] * m[10] +
	               m[12] * m[6] * m[9];

	    inv[1] = -m[1]  * m[10] * m[15] +
	              m[1]  * m[11] * m[14] +
	              m[9]  * m[2] * m[15] -
	              m[9]  * m[3] * m[14] -
	              m[13] * m[2] * m[11] +
	              m[13] * m[3] * m[10];

	inv[5] = m[0]  * m[10] * m[15] -
	         m[0]  * m[11] * m[14] -
	         m[8]  * m[2] * m[15] +
	         m[8]  * m[3] * m[14] +
	         m[12] * m[2] * m[11] -
	         m[12] * m[3] * m[10];

	inv[9] = -m[0]  * m[9] * m[15] +
	          m[0]  * m[11] * m[13] +
	          m[8]  * m[1] * m[15] -
	          m[8]  * m[3] * m[13] -
	          m[12] * m[1] * m[11] +
	          m[12] * m[3] * m[9];

	inv[13] = m[0]  * m[9] * m[14] -
	          m[0]  * m[10] * m[13] -
	          m[8]  * m[1] * m[14] +
	          m[8]  * m[2] * m[13] +
	          m[12] * m[1] * m[10] -
	          m[12] * m[2] * m[9];

	inv[2] = m[1]  * m[6] * m[15] -
	         m[1]  * m[7] * m[14] -
	         m[5]  * m[2] * m[15] +
	         m[5]  * m[3] * m[14] +
	         m[13] * m[2] * m[7] -
	         m[13] * m[3] * m[6];

	inv[6] = -m[0]  * m[6] * m[15] +
	          m[0]  * m[7] * m[14] +
	          m[4]  * m[2] * m[15] -
	          m[4]  * m[3] * m[14] -
	          m[12] * m[2] * m[7] +
	          m[12] * m[3] * m[6];

	inv[10] = m[0]  * m[5] * m[15] -
	          m[0]  * m[7] * m[13] -
	          m[4]  * m[1] * m[15] +
	          m[4]  * m[3] * m[13] +
	          m[12] * m[1] * m[7] -
	          m[12] * m[3] * m[5];

	inv[14] = -m[0]  * m[5] * m[14] +
	           m[0]  * m[6] * m[13] +
	           m[4]  * m[1] * m[14] -
	           m[4]  * m[2] * m[13] -
	           m[12] * m[1] * m[6] +
	           m[12] * m[2] * m[5];

	inv[3] = -m[1] * m[6] * m[11] +
	          m[1] * m[7] * m[10] +
	          m[5] * m[2] * m[11] -
	          m[5] * m[3] * m[10] -
	          m[9] * m[2] * m[7] +
	          m[9] * m[3] * m[6];

	inv[7] = m[0] * m[6] * m[11] -
	         m[0] * m[7] * m[10] -
	         m[4] * m[2] * m[11] +
	         m[4] * m[3] * m[10] +
	         m[8] * m[2] * m[7] -
	         m[8] * m[3] * m[6];

	inv[11] = -m[0] * m[5] * m[11] +
	           m[0] * m[7] * m[9] +
	           m[4] * m[1] * m[11] -
	           m[4] * m[3] * m[9] -
	           m[8] * m[1] * m[7] +
	           m[8] * m[3] * m[5];

	inv[15] = m[0] * m[5] * m[10] -
	          m[0] * m[6] * m[9] -
	          m[4] * m[1] * m[10] +
	          m[4] * m[2] * m[9] +
	          m[8] * m[1] * m[6] -
	          m[8] * m[2] * m[5];

	det = m[0] * inv[0] + m[1] * inv[4] + m[2] * inv[8] + m[3] * inv[12];

	if (det == 0)
		return (m4f){0};

	det = 1.0 / det;
	// for (i = 0; i < 16; i++)
	//     invOut[i] = inv[i] * det;
	// inv *= det;
	return inv*det;
}
