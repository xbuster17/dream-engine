#ifndef __DE_CAMERA_H__
#define __DE_CAMERA_H__

// #include "../libs.h"
// #include "../conf.h"
#include "../common/common.h"

// typedef struct dcam{
// 	v4f pos;
// 	v4f rot;
// 	float fov;
// 	float nearclip;
// 	float farclip;
// 	float res;
// 	m4f view;
// 	m4f proj;
// 	m4f vp;
// } dcam;
void dcam_reset(void); // sets everything to default values

void dcam_update(void); // regenerate matrices

// these call update for you
void dcam_view(v4f pos, v4f rot);
void dcam_pos(v4f pos);
void dcam_rot(v4f rot);
void dcam_proj(float fov, float near, float far); // 0 or negative to leave unchanged


#endif
