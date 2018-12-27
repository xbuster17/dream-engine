#include "math.h"

int rand2(int x, int y, int seed){
	srand( hash3(x,y,seed));
	return rand();
}

int rand3(int x, int y, int z, int seed){
	srand( hash4(x,y,z,seed));
	return rand();
}
