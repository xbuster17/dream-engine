#ifndef __DE_INPUT_H__
#define __DE_INPUT_H__

#include "../libs.h"
#include "keycode_enum.h"

void dinput_init(void);
void dinput_quit(void);

void dinput_update(void);

#define dgetk dinput_getk
#define dusek dinput_usek

Uint32 dinput_getk(enum dinput_keycode keycode);
Uint32 dinput_usek(enum dinput_keycode keycode);

void dmouse_grab(bool b);
void dfinger_click_time(int ms);

struct dfinger* dfinger_in_radius(float x, float y, float rad);
struct dfinger* dfinger_in_area(float x, float y, float width, float height);

bool dinput_test_keycodes(void);

#endif
