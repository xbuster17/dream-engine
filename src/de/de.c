#include "de.h"
#include <string.h>

struct De De;

int de_init(int x, int y, int flags){

	memset(&De, 0, sizeof(typeof(De)));

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

