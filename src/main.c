#include "de/de.h"

#include "game/mgame/mgame.h"

#include "game/ex/shd-metaball.h"

void ex_shd_metaball(void);

int SDL_main(int argc, char**argv){ (void)argv, (void)argc;
printf("%c\n", "hello"[1]);
	// Dsnd_init_conf.bufsize = 128;
	// Dsnd_init_conf.sample_rate = 22050;
	// Dsnd_init_conf.format = AUDIO_S16SYS;
	// de_init(0,0,0);
	de_init(512,512,0);

	mgame_main();
	// ex_shd_metaball();
	// golf_main();

	printf("has vao =%i\n", De.glext_has_vao);
// dmouse_hide(0);
// dmouse_grab(0);
// dvsync(0);

	de_quit();

	exit(0);
	return 0;
}












