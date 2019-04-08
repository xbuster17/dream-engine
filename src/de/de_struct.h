#ifndef __DE_DE_STRUCT_H__
#define __DE_DE_STRUCT_H__

#include "libs.h"
#include "common/common.h"

extern struct De{
//window
	char* title;
	v2i size;
	v2i wpos;
	v2i ctx_size;
	v2i screen_size;
	float res; // y/x
	float ctx_res; // y/x
	float screen_res; // y/x

	bool window_focus;
	bool fullscreen;
	bool is_android;

	bool game_loop_quit; // if not 0 ends DE_GAME_LOOP
	int game_loop_frame;

	// bool mouse_grab;

	SDL_Window* win;
	SDL_GLContext ctx;





//input
	Uint32* keyboard_state; // holds timestamps for last time pressed or 0 if up
	int keyboard_num_scancodes;

	bool should_quit;


	struct dmouse{
		Uint8 b1;
		Uint8 b2;
		Uint8 b3;
		v2i abs;
		v2i rel;
		v2i scroll;
	} mouse;


	struct dfinger{
		bool down;
		int id;
		v2f abs;
		v2f rel;
		bool captured; // finger belongs to some area and wont be found until released
		int timestamp;
		int mot_timestamp;
	} finger[DE_FINGER_MAX];

	uint fingers_down;

	int finger_click;
	v2f finger_click_pos;
	// adds 1 if a finger is released with a
	// delta timestamp < finger_click_time
	// must be cleared manually

	uint finger_click_time; // in milliseconds






//gl
	// info
	bool gles_v2; // for android
	bool glext_has_vao;
	char* gl_driver;
	char* gl_version;
	char* gl_renderer;

	int vsync;
	int tex_bound[8];
	int vbo_bound;

	struct dfbo* fbo;

	GLuint shd_bound;


	//cam
	// camera width, height and resolution are taken from De.ctx_size and De.ctx_res or,
	// if an fbo is bound, from the bound fbo
	struct dcam {
		//view
		v4f pos;
		v4f rot;
		//projection
		float fov;
		float far_clip;
		float near_clip;
		float res;
		//matrices
		m4f view;
		m4f proj;
		m4f vp; // view-projection
	} cam;

//sound
	int audio_channels;
	int sample_rate;
	int audio_buffer_size;
	Uint16 audio_format;
// AUDIO_S16SYS, AUDIO_S8, AUDIO_U16SYS, AUDIO_S32SYS ... Signed or Unsigned
// LSB or MSB can be used instead of SYS for little or big endian byte order


} De;


#endif
