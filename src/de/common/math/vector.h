#ifndef __MATH_VECTOR_H__
#define __MATH_VECTOR_H__

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
// posibly require gcc extension to compile

typedef int     v2i __attribute__((vector_size(sizeof(int)*2)));
typedef int     v4i __attribute__((vector_size(sizeof(int)*4)));
typedef int     m2i __attribute__((vector_size(sizeof(int)*4)));
typedef int     m4i __attribute__((vector_size(sizeof(int)*16)));
typedef uint8_t v2c __attribute__((vector_size(2)));
typedef uint8_t v4c __attribute__((vector_size(4)));
typedef uint8_t m2c __attribute__((vector_size(4)));
typedef uint8_t m4c __attribute__((vector_size(16)));
typedef float   v2f __attribute__((vector_size(sizeof(float)*2)));
typedef float   v4f __attribute__((vector_size(sizeof(float)*4)));
typedef float   m2f __attribute__((vector_size(sizeof(float)*4)));
typedef float   m4f __attribute__((vector_size(sizeof(float)*16)));
typedef double  v2d __attribute__((vector_size(sizeof(double)*2)));
typedef double  v4d __attribute__((vector_size(sizeof(double)*4)));
typedef double  m2d __attribute__((vector_size(sizeof(double)*4)));
typedef double  m4d __attribute__((vector_size(sizeof(double)*16)));

// some functions will ignore forth coordinate for convinience of use (dot, len, cross, ...)
typedef v4d v3d;
typedef v4f v3f;
typedef v4i v3i;
typedef v4c v3c;

// doesn't have functions (todo), do these even matter?
typedef long     v2l __attribute__((vector_size(sizeof(long)*2)));
typedef long     v4l __attribute__((vector_size(sizeof(long)*4)));
typedef long     m2l __attribute__((vector_size(sizeof(long)*4)));
typedef long     m4l __attribute__((vector_size(sizeof(long)*16)));

// constructors
#define v4d_(...)((v4d){__VA_ARGS__})
#define v4f_(...)((v4f){__VA_ARGS__})
#define v4i_(...)((v4i){__VA_ARGS__})
#define v4c_(...)((v4c){__VA_ARGS__})
#define v2d_(...)((v2d){__VA_ARGS__})
#define v2f_(...)((v2f){__VA_ARGS__})
#define v2i_(...)((v2i){__VA_ARGS__})
#define v2c_(...)((v2c){__VA_ARGS__})

#define m4d_(...)((m4d){__VA_ARGS__})
#define m4c_(...)((m4c){__VA_ARGS__})
#define m4i_(...)((m4i){__VA_ARGS__})
#define m4c_(...)((m4c){__VA_ARGS__})
#define m2d_(...)((m2d){__VA_ARGS__})
#define m2f_(...)((m2f){__VA_ARGS__})
#define m2i_(...)((m2i){__VA_ARGS__})
#define m2c_(...)((m2c){__VA_ARGS__})

#define v4d_0 ( (v4d){0, 0, 0, 0} )
#define v4f_0 ( (v4f){0, 0, 0, 0} )
#define v4i_0 ( (v4i){0, 0, 0, 0} )
#define v4c_0 ( (v4c){0, 0, 0, 0} )
#define v2d_0 ( (v2d){0, 0} )
#define v2f_0 ( (v2f){0, 0} )
#define v2i_0 ( (v2i){0, 0} )
#define v2c_0 ( (v2c){0, 0} )

#define m4d_0 ( (m4d){0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0} )
#define m4f_0 ( (m4f){0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0} )
#define m4i_0 ( (m4i){0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0} )
#define m4c_0 ( (m4c){0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0} )
#define m2d_0 ( (m2d){0,0, 0,0} )
#define m2f_0 ( (m2f){0,0, 0,0} )
#define m2i_0 ( (m2i){0,0, 0,0} )
#define m2c_0 ( (m2c){0,0, 0,0} )


#define v4_cast(v, target_type) ((target_type){(v)[0], (v)[1], (v)[2], (v)[3]})
#define v2_cast(v, target_type) ((target_type){(v)[0], (v)[1]})
#define m4_cast(m, target_type) ((target_type){ \
  (m)[0], (m)[1], (m)[2], (m)[3],              \
  (m)[4], (m)[5], (m)[6], (m)[7],              \
  (m)[8], (m)[9], (m)[10],(m)[11],             \
  (m)[12],(m)[13],(m)[14],(m)[15]             })
#define m2_cast(m, target_type) ((target_type){(m)[0], (m)[1], (m)[2], (m)[3]})

m4f m4f_from_v4f(v4f v1, v4f v2, v4f v3, v4f v4);
v4f v4f_from_m4f(m4f A, uint8_t which_row);



v4d v4d_rand(void);
v4f v4f_rand(void);
v4i v4i_rand(void);
v4c v4c_rand(void);
v2d v2d_rand(void);
v2f v2f_rand(void);
v2i v2i_rand(void);
v2c v2c_rand(void);

void m4d_print(m4d A);
void m4f_print(m4f A);
void m4i_print(m4i A);
void m4c_print(m4c A);
void v4d_print(v4d v);
void v4f_print(v4f v);
void v4i_print(v4i v);
void v4c_print(v4c v);
void m2d_print(m2d A);
void m2f_print(m2f A);
void m2i_print(m2i A);
void m2c_print(m2c A);
void v2d_print(v2d v);
void v2f_print(v2f v);
void v2i_print(v2i v);
void v2c_print(v2c v);



double v4d_dot(v4d v, v4d w);
float  v4f_dot(v4f v, v4f w);
float  v4i_dot(v4i v, v4i w);
float  v4c_dot(v4c v, v4c w);
double v3d_dot(v4d v, v4d w);
float  v3f_dot(v4f v, v4f w);
float  v3i_dot(v4i v, v4i w);
float  v3c_dot(v4c v, v4c w);
double v2d_dot(v2d v, v2d w);
float  v2f_dot(v2f v, v2f w);
float  v2i_dot(v2i v, v2i w);
float  v2c_dot(v2c v, v2c w);

v4d v3d_cross(v4d v1, v4d v2);
v4f v3f_cross(v4f v1, v4f v2);
v4i v3i_cross(v4i v1, v4i v2);
v4c v3c_cross(v4c v1, v4c v2);
#define v4d_cross v3d_cross
#define v4f_cross v3f_cross
#define v4i_cross v3i_cross
#define v4c_cross v3c_cross

v4d v4d_mmul(v4d v, m4d m); // vector matrix multiplication
v4f v4f_mmul(v4f v, m4f m);
v4i v4i_mmul(v4i v, m4i m);
v4c v4c_mmul(v4c v, m4c m);
v4d v4d_mtmul(v4d v, m4d m); // transposed vector matrix multiplication
v4f v4f_mtmul(v4f v, m4f m);
v4i v4i_mtmul(v4i v, m4i m);
v4c v4c_mtmul(v4c v, m4c m);
v2d v2d_mmul(v2d v, m2d m);
v2f v2f_mmul(v2f v, m2f m);
v2i v2i_mmul(v2i v, m2i m);
v2c v2c_mmul(v2c v, m2c m);
v2d v2d_mtmul(v2d v, m2d m);
v2f v2f_mtmul(v2f v, m2f m);
v2i v2i_mtmul(v2i v, m2i m);
v2c v2c_mtmul(v2c v, m2c m);

m4d m4d_mul(m4d A, m4d B); // matrix multiplication
m4f m4f_mul(m4f A, m4f B);
m4i m4i_mul(m4i A, m4i B);
m4c m4c_mul(m4c A, m4c B);
m2d m2d_mul(m2d A, m2d B);
m2f m2f_mul(m2f A, m2f B);
m2i m2i_mul(m2i A, m2i B);
m2c m2c_mul(m2c A, m2c B);

double v4d_len(v4d v1);
float  v4f_len(v4f v1);
float  v4i_len(v4i v1);
float  v4c_len(v4c v1);
double v3d_len(v4d v1);
float  v3f_len(v4f v1);
float  v3i_len(v4i v1);
float  v3c_len(v4c v1);
double v2d_len(v2d v1);
float  v2f_len(v2f v1);
float  v2i_len(v2i v1);
float  v2c_len(v2c v1);

double v4d_len2(v4d v1);
float  v4f_len2(v4f v1);
float  v4i_len2(v4i v1);
float  v4c_len2(v4c v1);
double v3d_len2(v4d v1);
float  v3f_len2(v4f v1);
float  v3i_len2(v4i v1);
float  v3c_len2(v4c v1);
double v2d_len2(v2d v1);
float  v2f_len2(v2f v1);
float  v2i_len2(v2i v1);
float  v2c_len2(v2c v1);

v4d v4d_normalize(v4d v1);
v4d v3d_normalize(v4d v1);
v4f v4f_normalize(v4f v1);
v4f v3f_normalize(v4f v1);
v2d v2d_normalize(v2d v1);
v2f v2f_normalize(v2f v1);

m4d m4d_id(void);
m4f m4f_id(void);
m4i m4i_id(void);
m4c m4c_id(void);
m2d m2d_id(void);
m2f m2f_id(void);
m2i m2i_id(void);
m2c m2c_id(void);
#define M4D_ID ((m4d){1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1})
#define M4F_ID ((m4f){1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1})
#define M4I_ID ((m4i){1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1})
#define M4C_ID ((m4c){1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1})
#define M2D_ID ((m2d){1,0, 0,1})
#define M2F_ID ((m2f){1,0, 0,1})
#define M2I_ID ((m2i){1,0, 0,1})
#define M2C_ID ((m2c){1,0, 0,1})

float  v3f_ang(v4f v1, v4f v2);// Image:[0,PI]
double v3d_ang(v4d v1, v4d v2);// Image:[0,PI]
float  v4f_ang(v4f v1, v4f v2);// Image:[0,PI]
double v4d_ang(v4d v1, v4d v2);// Image:[0,PI]

v4f v4f_reflect(v4f v, v4f normal);
v4d v4d_reflect(v4d v, v4d normal);

m4f m4f_inverse(m4f mat);
m4f m4f_transpose(m4f m);


m4f m4f_scale(m4f mat, v4f scale);
m4f m4f_scale(m4f mat, v4f scale);

void m4f_rot_v4f(m4f* m, v4f v1, float radians);
m4f m4f_rotate(m4f m, v4f rot);
m4f m4f_rotate_x(m4f m, float radians);
m4f m4f_rotate_y(m4f m, float radians);
m4f m4f_rotate_z(m4f m, float radians);
m4f m4f_rotation(v4f rot);
m4f m4f_rotation_x(float radians);
m4f m4f_rotation_y(float radians);
m4f m4f_rotation_z(float radians);

m2f m2f_rotate(m2f mat, float radians);
m2f m2f_rotatation(float radians);
m2f m2f_shear(v2f v);

// graphic tools
m4f m4f_translate(m4f m, v4f v);
m4f m4f_translation(v4f v);

m4f m4f_view(v4f pos, v4f rot);
m4f m4f_model(v4f pos, v4f rot, v4f sca); //
m4f m4f_timodel(m4f model); // transpose (inverse (model)) after removing translation


// m4f m4f_view(v4f pos, v4f rot) rotate and translate matrix
m4f m4f_proj(float angle_of_view, float aspect_ratio, float z_near, float z_far);
m4f m4f_view_frustum(float angle_of_view, float aspect_ratio, float z_near, float z_far);
m4f m4f_ortho_project(v2f canvas_size, float near, float far);

m4f m4f_look_at(v4f eye, v4f center, v4f up);

#endif
