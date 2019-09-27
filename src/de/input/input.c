#include "input.h"
#include "../de_struct.h"
#include "../window/window.h" // dwindow_resized()

int dinput_watcher(void* udata, SDL_Event* event);

void dinput_init(void){
//	SDL_SetHint(SDL_HINT_ANDROID_SEPARATE_MOUSE_AND_TOUCH, "1");
	De.mouse.grab=0;

	De.fingers_down = 0;
	De.finger_click = 0;
	De.finger_click_pos = v2f_0;
	De.finger_click_time = 250;
	int num_scancodes;
	SDL_GetKeyboardState( &num_scancodes );
	De.keyboard_num_scancodes = num_scancodes;
	De.keyboard_state = malloc(num_scancodes * sizeof(Uint32));
	memset(De.keyboard_state, 0, num_scancodes * sizeof(Uint32));

	SDL_EventState(SDL_DROPFILE, SDL_ENABLE);

/* android events to exit on background (todo) */
	// SDL_EventState(SDL_APP_WILLENTERBACKGROUND, SDL_ENABLE);
	// SDL_EventState(SDL_APP_DIDENTERBACKGROUND, SDL_ENABLE);
	// SDL_EventState(SDL_APP_DIDENTERFOREGROUND, SDL_ENABLE);
	// SDL_EventState(SDL_APP_WILLENTERFOREGROUND, SDL_ENABLE);

	SDL_AddEventWatch(dinput_watcher, NULL);
}





void dinput_quit(void){
	SDL_DelEventWatch(dinput_watcher, NULL);
	free(De.keyboard_state);
}





void dinput_update(void){
	De.mouse.rel *= 0;
	De.mouse.scroll[0]=0;
	De.mouse.scroll[1]=0;
	/* hack to fix android screen rotation */
	#if ANDROID
		int x,y;
		SDL_GetWindowSize(De.win, &x, &y);
		// dviewport(x, y);
		dwindow_resized(x,y);
	#endif

	SDL_PumpEvents();
}














int dinput_watcher(void* udata, SDL_Event* e){ (void) udata;
	int keycode_index = 0;
	// if(De.mouse.grab){
	// 	De.mouse.rel*=0;
	// 	De.mouse.abs*=0;
	// }

	switch(e->type){

		case SDL_QUIT: De.should_quit = true; break;

//keyboard
		case SDL_KEYDOWN:
			if(e->key.repeat) break;
			keycode_index = e->key.keysym.scancode % De.keyboard_num_scancodes;
			De.keyboard_state[ keycode_index ] = e->key.timestamp;
			break;

		case SDL_KEYUP:
			if(e->key.repeat) break;
			keycode_index = e->key.keysym.scancode % De.keyboard_num_scancodes;
			De.keyboard_state[ keycode_index ] = 0;
			break;




//mouse
		case SDL_MOUSEBUTTONDOWN:
			switch (e->button.button){
				case SDL_BUTTON_LEFT:   De.mouse.b1 = e->button.clicks; break;
				case SDL_BUTTON_RIGHT:  De.mouse.b2 = e->button.clicks; break;
				case SDL_BUTTON_MIDDLE: De.mouse.b3 = e->button.clicks; break;
				default: break;
			};
			break;

		case SDL_MOUSEBUTTONUP:
			switch (e->button.button){
				case SDL_BUTTON_LEFT:   De.mouse.b1 = 0; break;
				case SDL_BUTTON_RIGHT:  De.mouse.b2 = 0; break;
				case SDL_BUTTON_MIDDLE: De.mouse.b3 = 0; break;
				default: break;
			};
			break;

		case SDL_MOUSEMOTION:
			De.mouse.abs[0] = e->motion.x;
			De.mouse.abs[1] = e->motion.y;
			De.mouse.rel[0] = e->motion.xrel;
			De.mouse.rel[1] = e->motion.yrel;
			break;

		case SDL_MOUSEWHEEL:
			De.mouse.scroll[0] = e->wheel.x;
			De.mouse.scroll[1] = e->wheel.y;
			De.mouse.scroll_total[0] += e->wheel.x;
			De.mouse.scroll_total[1] += e->wheel.y;
			break;




//touch input posiiton (0, 0) is top-left and (1,1) is bottom-right
		case SDL_FINGERDOWN:
			for (int i = 0; i < DE_FINGER_MAX; ++i){
				if(De.finger[i].down == false){
					De.finger[i].id = e->tfinger.fingerId;
					De.finger[i].down = true;
					De.finger[i].captured = false;
					De.finger[i].timestamp = e->tfinger.timestamp;
					De.finger[i].abs = (v2f){e->tfinger.x, e->tfinger.y};
					De.finger[i].rel = v2f_0;
					De.fingers_down++;
					return 1;
				}
			}
			break;

		case SDL_FINGERUP:
			for (int i = 0; i < DE_FINGER_MAX; ++i){
				if(De.finger[i].id == e->tfinger.fingerId){
					// finger click
					if(e->tfinger.timestamp - De.finger[i].timestamp < De.finger_click_time){
						De.finger_click += 1;
						De.finger_click_pos = (v2f){ e->tfinger.x, e->tfinger.y };
					}
					De.finger[i].id = -1;
					De.finger[i].down = false;
					De.finger[i].captured = false;
					De.finger[i].abs = v2f_0;
					De.finger[i].rel = v2f_0;
					De.fingers_down--;
					return 1;
				}
			}
			break;

		case SDL_FINGERMOTION:
			for (int i = 0; i < DE_FINGER_MAX; ++i){
				if(De.finger[i].id == e->tfinger.fingerId){
					v2f finger_pos = {e->tfinger.x, e->tfinger.y};
					De.finger[i].rel += (v2f){e->tfinger.dx, e->tfinger.dy};
					De.finger[i].abs = finger_pos;
					De.finger[i].mot_timestamp = e->tfinger.timestamp;
					return 1;
				}
			}
			break;




//window
		case SDL_WINDOWEVENT:
			switch(e->window.event){
				case SDL_WINDOWEVENT_SIZE_CHANGED:
				case SDL_WINDOWEVENT_RESIZED:
					dwindow_resized(e->window.data1, e->window.data2);
					break;
				case SDL_WINDOWEVENT_CLOSE: De.should_quit = true;
					break;

				case SDL_WINDOWEVENT_ENTER:
				case SDL_WINDOWEVENT_TAKE_FOCUS:
				case SDL_WINDOWEVENT_FOCUS_GAINED: De.window_focus = true; 
					break;
				case SDL_WINDOWEVENT_FOCUS_LOST: De.window_focus = false; 
					break;
				case SDL_WINDOWEVENT_MOVED:
					De.wpos = (v2i){e->window.data1, e->window.data2};
					break;

				default: break;
			}
			break;


		default: break;
	}

	return 1; // does not drop event from queue (if used on SDL_SetEventFilter)
}




























// MISC FUNCTIONS
Uint32 dinput_getk(enum dinput_keycode keycode){
	return De.keyboard_state[SDL_GetScancodeFromKey(keycode) % De.keyboard_num_scancodes];
}



Uint32 dinput_usek(enum dinput_keycode keycode){
	int index = SDL_GetScancodeFromKey(keycode) % De.keyboard_num_scancodes;
	int val = De.keyboard_state[index];
	De.keyboard_state[index] = 0;
	return val;
}



void dmouse_grab(bool b){ 
	De.mouse.grab=b;
	#ifdef __EMSCRIPTEN__ // TODO fixme drift
	if (b){
		emscripten_request_pointerlock("canvas", true);
		dmouse_hide(1);
	}else{ 
		emscripten_exit_pointerlock();
		dmouse_hide(0);
	}
	#else
	SDL_SetRelativeMouseMode(b);
	#endif
}



void dmouse_hide(bool b){ SDL_ShowCursor(!b); }



struct dfinger* dfinger_in_radius(float x, float y, float rad){
	for (int i = 0; i < DE_FINGER_MAX; ++i){
		if(De.finger[i].down && !De.finger[i].captured){
			float dist2 = v2f_len2((v2f){x,y} - De.finger[i].abs);
			if( dist2 < rad*rad ){
				return &De.finger[i];
			}
		}
	}
	return NULL;
}



struct dfinger* dfinger_in_area(float x, float y, float width, float height){
	for (int i = 0; i < DE_FINGER_MAX; ++i){
		if(De.finger[i].down && !De.finger[i].captured){
			float px = De.finger[i].abs[0];
			float py = De.finger[i].abs[1];
			bool in_area =
				px > x && px < x+width &&
				py > y && py < y+height;
			if(in_area)
				return &De.finger[i];
		}
	}
	return NULL;
}





void dfinger_click_time(int ms){ De.finger_click_time = ms;}











































// TEST
bool dinput_test_keycodes(void){
	assert( SDLK_UNKNOWN      == '\0' );
	assert( SDLK_BACKSPACE    == '\b' );
	assert( SDLK_TAB          == '\t' );
	assert( SDLK_RETURN       == '\r' );
	assert( SDLK_ESCAPE       == '\033' );
	assert( SDLK_SPACE        == ' ' );
	assert( SDLK_EXCLAIM      == '!' );
	assert( SDLK_QUOTEDBL     == '"' );
	assert( SDLK_HASH         == '#' );
	assert( SDLK_DOLLAR       == '$' );
	assert( SDLK_PERCENT      == '%' );
	assert( SDLK_AMPERSAND    == '&' );
	assert( SDLK_QUOTE        == '\'' );
	assert( SDLK_LEFTPAREN    == '(' );
	assert( SDLK_RIGHTPAREN   == ')' );
	assert( SDLK_ASTERISK     == '*' );
	assert( SDLK_PLUS         == '+' );
	assert( SDLK_COMMA        == ',' );
	assert( SDLK_MINUS        == '-' );
	assert( SDLK_PERIOD       == '.' );
	assert( SDLK_SLASH        == '/' );
	assert( SDLK_0            == '0' );
	assert( SDLK_1            == '1' );
	assert( SDLK_2            == '2' );
	assert( SDLK_3            == '3' );
	assert( SDLK_4            == '4' );
	assert( SDLK_5            == '5' );
	assert( SDLK_6            == '6' );
	assert( SDLK_7            == '7' );
	assert( SDLK_8            == '8' );
	assert( SDLK_9            == '9' );
	assert( SDLK_COLON        == ':' );
	assert( SDLK_SEMICOLON    == ';' );
	assert( SDLK_LESS         == '<' );
	assert( SDLK_EQUALS       == '=' );
	assert( SDLK_GREATER      == '>' );
	assert( SDLK_QUESTION     == '?' );
	assert( SDLK_AT           == '@' );
	assert( SDLK_LEFTBRACKET  == '[' );
	assert( SDLK_BACKSLASH    == '\\' );
	assert( SDLK_RIGHTBRACKET == ']' );
	assert( SDLK_CARET        == '^' );
	assert( SDLK_UNDERSCORE   == '_' );
	assert( SDLK_BACKQUOTE    == '`' );
	assert( SDLK_a            == 'a' );
	assert( SDLK_b            == 'b' );
	assert( SDLK_c            == 'c' );
	assert( SDLK_d            == 'd' );
	assert( SDLK_e            == 'e' );
	assert( SDLK_f            == 'f' );
	assert( SDLK_g            == 'g' );
	assert( SDLK_h            == 'h' );
	assert( SDLK_i            == 'i' );
	assert( SDLK_j            == 'j' );
	assert( SDLK_k            == 'k' );
	assert( SDLK_l            == 'l' );
	assert( SDLK_m            == 'm' );
	assert( SDLK_n            == 'n' );
	assert( SDLK_o            == 'o' );
	assert( SDLK_p            == 'p' );
	assert( SDLK_q            == 'q' );
	assert( SDLK_r            == 'r' );
	assert( SDLK_s            == 's' );
	assert( SDLK_t            == 't' );
	assert( SDLK_u            == 'u' );
	assert( SDLK_v            == 'v' );
	assert( SDLK_w            == 'w' );
	assert( SDLK_x            == 'x' );
	assert( SDLK_y            == 'y' );
	assert( SDLK_z            == 'z' );
	assert( SDLK_DELETE       == '\177' );
	return 0;
}
