#ifndef __DE_SHADER_LIB_H__
#define __DE_SHADER_LIB_H__
// #include "../common/common.h" // QUOTE
// compile me with shd.h

#define DSHDA_POS dpos
#define DSHDA_NOR dnor
#define DSHDA_TC dtc
#define DSHDA_COL dcol
// please add attributes in order to the shader
#define DSHDA_USER duser
#define DSHDA_POS_INDEX 0
#define DSHDA_NOR_INDEX 1
#define DSHDA_TC_INDEX  2
#define DSHDA_COL_INDEX 3
#define DSHDA_POS_TYPE f4
#define DSHDA_NOR_TYPE f4
#define DSHDA_TC_TYPE  f2
#define DSHDA_COL_TYPE f4
#define DSHDA_POS_VARY dpos_v
#define DSHDA_NOR_VARY dnor_v
#define DSHDA_TC_VARY dtc_v
#define DSHDA_COL_VARY dcol_v
#define DSHDU_EYE deye
#define DSHDU_MVP dMVP
#define DSHDU_MV  dMV
#define DSHDU_M   dM
#define DSHDU_N   dN
#define DSHDU_MAT_DIFF dmtrl_diff
#define DSHDU_MAT_SPEC dmtrl_spec
#define DSHDU_MAT_SHIN dmtrl_shin

#define DSHDL_OUT_VSH "\n#define dshd_out gl_Position\n"
#define DSHDL_OUT_FSH "\n#define dshd_out gl_FragColor\n"
#define DSHDL_PI \
	"\n#ifndef PI \n"\
	"\n#define PI 3.14159265359\n"\
	"\n#endif\n"\

#define DSHDL_BASE_3D_UNIF QUOTE( \
uniform vec4 DSHDU_EYE;    \n\
uniform mat4 DSHDU_MVP;    \n\
uniform mat4 DSHDU_MV;     \n\
uniform mat4 DSHDU_M;      \n\
uniform mat4 DSHDU_N;      \n\
)\

#define DSHDL_MTRL_UNIF QUOTE( \
uniform vec4 DSHDU_MAT_DIFF; \n\
uniform vec4 DSHDU_MAT_SPEC; \n\
uniform float DSHDU_MAT_SHIN;\n\
)\




#define DSHDL_LIGHT_SETUP(pos, nor, M, N, diff, spec, shin) \
	"\n#define _DSHDU_LIGHT_APOS "pos       \
	"\n#define _DSHDU_LIGHT_ANOR "nor       \
	"\n#define _DSHDU_LIGHT_UM "M           \
	"\n#define _DSHDU_LIGHT_UN "N           \
	"\n#define _DSHDU_LIGHT_MTRL_DIFF "diff           \
	"\n#define _DSHDU_LIGHT_MTRL_SPEC "spec           \
	"\n#define _DSHDU_LIGHT_MTRL_SHIN "shin           \
	"\n"\


#define DSHDL_LIGHT_SMOOTH \
	"DSHDA_POS_VARY = DSHDA_POS;"\
	"DSHDA_NOR_VARY = DSHDA_NOR;"\


#define DSHDL_LIGHT(x) QUOTE(\
uniform vec4 dlight_pos[x];   \n\
uniform vec4 dlight_col[x];   \n\
uniform float dlight_pow[x];  \n\
int dlight_count = x;         \n\
vec4 dlight(int i){                                                                          \n\
\t    vec4 inor = normalize(_DSHDU_LIGHT_UN * _DSHDU_LIGHT_ANOR);                              \n\
\t    vec4 spec = vec4(0.0, 0.0, 0.0, 1.0);                                                    \n\
\t    vec4 v = normalize( DSHDU_EYE - _DSHDU_LIGHT_UM * _DSHDU_LIGHT_APOS);                    \n\
\t    vec4 ldirv = dlight_pos[i] - _DSHDU_LIGHT_APOS;                                          \n\
\t    float dist2 = dot(ldirv, ldirv);                                                         \n\
\t    vec4 ldir = normalize(ldirv);                                                            \n\
\t    vec4 r = reflect(-ldir, inor);                                                           \n\
\t    float cosTheta = max(dot(ldir, inor), 0.0);                                              \n\
\t    vec4 diff = _DSHDU_LIGHT_MTRL_DIFF * dlight_col[i] * dlight_pow[i] * cosTheta / dist2;   \n\
\t    if(cosTheta > 0.0)                                                                       \n\
\t\t        spec = ( _DSHDU_LIGHT_MTRL_SPEC  *  dlight_col[i]  *  dlight_pow[i]  *               \n\
\t\t\t            pow(max(dot(r,v), 0.0),  _DSHDU_LIGHT_MTRL_SHIN )) / (dist2*.5);                 \n\
\t    return diff + spec;                                                                      \n\
}\
)\







// char* Dshdl_uber_attr[16] = {
// 	QUOTE(DSHDA_POS),
// 	QUOTE(DSHDA_NOR),
// 	QUOTE(DSHDA_TC),
// 	QUOTE(DSHDA_COL),
// }
#define DSHDL_UBER_ATTR QUOTE(f 4 dpos, f 4 dnor, f 2 dtc, f 4 dcol)

#define DSHDL_UBER_VSH DE_SHD_HEADERV \
	DSHDL_OUT_VSH \
	QUOTE(\
attribute vec4 DSHDA_POS;  \n\
attribute vec4 DSHDA_NOR;  \n\
attribute vec2 DSHDA_TC;  \n\
attribute vec4 DSHDA_COL;  \n\
attribute vec4 DSHDA_USER;  \n\
)\
	DSHDL_LIGHT_SETUP( QUOTE(DSHDA_POS), QUOTE(DSHDA_NOR), \
		QUOTE(DSHDU_M), QUOTE(DSHDU_N),\
		QUOTE(DSHDU_MAT_DIFF), QUOTE(DSHDU_MAT_SPEC), QUOTE(DSHDU_MAT_SHIN)\
	)\
	DSHDL_UBER_VFH \


#define DSHDL_UBER_FSH DE_SHD_HEADERF \
	DSHDL_OUT_FSH \
	DSHDL_LIGHT_SETUP( QUOTE(DSHDA_POS_VARY), QUOTE(DSHDA_NOR_VARY), \
		QUOTE(DSHDU_M), QUOTE(DSHDU_N),\
		QUOTE(DSHDU_MAT_DIFF), QUOTE(DSHDU_MAT_SPEC), QUOTE(DSHDU_MAT_SHIN)\
)\
	DSHDL_UBER_VFH \


#define DSHDL_UBER_VFH QUOTE(\
varying vec4 DSHDA_POS_VARY;  \n\
varying vec4 DSHDA_NOR_VARY;  \n) \
	DSHDL_BASE_3D_UNIF \
	DSHDL_MTRL_UNIF \
	DSHDL_LIGHT(4)\





// typedef struct dshdl_uber_unif{
// 	v4f pos, rot; // eye
// 	m4v M, V, P,
// 		N, VP, MVP;
// } dshdl_uber_unif;

// dshdl_uber_unif* dshdl_uber_unif_new( dcam* );
// void dshdl_uber_unif_init( dshdl_uber_unif* , dcam* );
// void dshdl_uber_unif_bind( dshdl_uber_unif* , m4f Model_matrix );
// void dshdl_uber_bind( dshd*, m4f M, dcam* );

// dmtrl_
// dcam_





#endif
