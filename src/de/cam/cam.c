#include "cam.h"
#include "../de_struct.h"

#include "../gl/tex.h" // fbo

void dcam_reset(void){
	De.cam.pos = (v4f){0,0,0,1};
	De.cam.rot = (v4f){0,0,0,0};
	De.cam.view = (m4f){1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
	De.cam.proj = (m4f){1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
	De.cam.vp = (m4f){1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1};
	De.cam.fov = PI/6.f;
	De.cam.near_clip = .05;
	De.cam.far_clip = 500;
	De.cam.res = 1;
	dcam_update();
}



void dcam_update(void){
	float res = De.fbo ? De.fbo->res : De.ctx_res;
	// De.cam.dir = v4f_mtmul((v4f){0,0,-1,0}, m4f_rotation(De.cam.rot));
	De.cam.dir = v4f_normalize( De.cam.tar - De.cam.pos);
	// De.cam.tar = De.cam.pos + De.cam.dir;
	// De.cam.view = m4f_view(De.cam.pos, De.cam.rot);
	De.cam.view = m4f_look_at(De.cam.pos, De.cam.tar, (v4f){0,1,0,0});
	// De.cam.view = m4f_look_at(De.cam.pos, (v4f){0,1,0,1}, (v4f){0,1,0,0});

	De.cam.res = res;
	De.cam.proj = m4f_proj(De.cam.fov, res, De.cam.near_clip, De.cam.far_clip);
	// De.cam.proj = m4f_ortho(-128, 128,128, -128, 5, 500);
	De.cam.vp = m4f_mul(De.cam.view, De.cam.proj);

}



static void dcam_update_view(void); // avoid calculating proj
static void dcam_update_proj(void); // avoid calculating view

void dcam_view(v4f pos, v4f rot){
	De.cam.pos = pos;
	De.cam.rot = rot;
	dcam_update_view();
}
void dcam_pos(v4f pos){ dcam_view(pos, De.cam.rot); }
void dcam_rot(v4f rot){ dcam_view(De.cam.pos, rot); }



void dcam_proj(float fov, float near, float far){
	if(fov != 0) De.cam.fov = fov;
	if(near != 0) De.cam.near_clip = near;
	if(far != 0) De.cam.far_clip = far;
	dcam_update_proj();
}



void dcam_update_view(void){
	De.cam.dir = v4f_mtmul((v4f){0,0,-1,0}, m4f_rotation(-De.cam.rot));
	De.cam.view = m4f_view(De.cam.pos, De.cam.rot);
	De.cam.vp = m4f_mul(De.cam.view, De.cam.proj);
}

void dcam_update_proj(void){
	De.cam.proj = m4f_proj(De.cam.fov, De.cam.res, De.cam.near_clip, De.cam.far_clip);
	De.cam.vp = m4f_mul(De.cam.view, De.cam.proj);
}











