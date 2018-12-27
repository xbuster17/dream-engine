#include <stdlib.h> // alloca
#include <string.h> // memcpy
#include "util.h"
#include "../libs.h"

void dswap(void*a, void*b, size_t size){
	void* temp = alloca(size);
	memcpy(temp, a, size);
	memcpy(a,    b, size);
	memcpy(b, temp, size);
}



void ddelay(int ms){
	SDL_Delay(ms);
}
