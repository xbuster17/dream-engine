#include "de/de.h"

#include "game/mgame/mgame.h"

int SDL_main(int argc, char**argv){ (void)argv, (void)argc;

	de_init(0,0,0);
	// de_init(256,256,0);
	// de_init(0,0,SDL_WINDOW_FULLSCREEN);

	mgame_main();

	de_quit();

	exit(0);
	return 0;
}


