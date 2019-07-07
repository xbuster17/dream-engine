#ifndef __DE_DE_H__
#define __DE_DE_H__
#ifdef __cplusplus
extern "C" {
#endif

#include "libs.h"
#include "common/common.h"
#include "window/window.h"
#include "input/input.h"
#include "gl/gl.h"
#include "cam/cam.h"
#include "android/glext.h"
#include "sound/sound.h"
#include "mesh/mesh.h"

#include "common/common.h"
#include "de_struct.h"

int de_init(int x, int y, int flags);
void de_quit(void);

// warning SDL_PumpEvents can only be called on the main thread, the thread that set the video mode
#define DE_GAME_LOOP()                   \
		for(                             \
			De.game_loop_quit = 0,       \
			De.game_loop_frame = 0       \
			;                            \
			(De.game_loop_quit == 0)     \
			&&                           \
			(De.should_quit == 0)        \
			;                            \
			ddisplay(),                  \
			dclear(0),                   \
			dinput_update(),             \
			De.game_loop_frame++         \
		) /* your code here */



#ifdef __cplusplus
}
#endif
#endif