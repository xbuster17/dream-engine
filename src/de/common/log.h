// temp
// todo android
#ifndef __DE_LOG_H__
#define __DE_LOG_H__
#include <stdio.h>
#include "../conf.h"

#ifdef DE_DEBUG
	#define DE_LOG(...) \
		(printf("DE_LOG: %s:%i: in function '%s': ", __FILE__, __LINE__, __func__), \
		printf(__VA_ARGS__), printf("\n"))

	#if defined(ANDROID)
		#define DE_LOGE(...)                                     \
		{                                                          \
			char* tmp = alloca(2048);                              \
			snprintf(tmp, 2048, __VA_ARGS__);                      \
			SDL_ShowSimpleMessageBox(0,"DE_LOGE ERROR",tmp,NULL);\
		}
	#else
		#define DE_LOGE(...) \
			fprintf(stderr, "DE_LOGE: %s:%i: in function '%s': ", __FILE__, __LINE__, __func__), \
			fprintf(stderr, __VA_ARGS__), fprintf(stderr, "\n")
	#endif /*android*/
#else
	#define DE_LOG(...)
	#define DE_LOGE(...)
#endif /*debug*/


// SDL_MESSAGEBOX_ERROR, SDL_MESSAGEBOX_WARNING, SDL_MESSAGEBOX_INFORMATION
// last argument is parent window.
#define DE_ALERT(x,y) SDL_ShowSimpleMessageBox(0,x,y,NULL)


#endif
