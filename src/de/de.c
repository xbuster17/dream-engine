#include "de.h"
#include <string.h>

struct De De;

int de_init(int x, int y, int flags){

	memset(&De, 0, sizeof(typeof(De)));

	#ifdef ANDROID
		De.is_android = true;
	#else
		De.is_android = false;
	#endif

	if( dwindow_init( x, y, flags ) ){
		exit(0);
	}

	if( dgl_init() ){
		exit(0);
	}

	dinput_init();


	dcam_reset();

	if(dsnd_init()){
		exit(0);
	}

	
	// /* SDL_Image_Init */
	// // img_flags |= IMG_INIT_TIF; // windows and android error: not suported
	// // int img_flags = IMG_INIT_JPG | IMG_INIT_PNG; // no emcc
	// int img_flags = IMG_INIT_PNG;
	// int initted = IMG_Init(img_flags);
	// if((initted&img_flags) != img_flags) {
	// 	DE_LOG("IMG_Init: %s\n", IMG_GetError());
	// 	return 3;
	// }



	// SDLNet_Init();

	return 0;
}



void de_quit(void){
	dinput_quit();
	// SDLNet_Quit();
	dsnd_quit();
	dgl_quit();
	dwindow_quit();
}

void de_main_loop( void(*f)(void) ){
#ifndef __EMSCRIPTEN__
	while(
		(De.game_loop_quit == 0) &&  
		(De.should_quit == 0)        
	)f();
#else
	emscripten_set_main_loop(f, 0, 1);
#endif
}


void de_main_tick( void ){
	dinput_update();
	ddisplay();
	dclear(0);
	De.game_loop_frame++;
#ifdef __EMSCRIPTEN__
	if(De.game_loop_quit || De.should_quit)
	emscripten_set_main_loop(NULL, 0, 1);
#endif
}



void de_run( void(*f)(void) ){ // shouldnt work
	#ifndef __EMSCRIPTEN__
	DE_GAME_LOOP() f();
	#else
	emscripten_set_main_loop(f, 0, 1);
	#endif
}