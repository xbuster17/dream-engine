#include "math.h"

int rand2(int x, int y, int seed){
	srand( hash3(x,y,seed));
	return rand();
}

int rand3(int x, int y, int z, int seed){
	srand( hash4(x,y,z,seed));
	return rand();
}

float px2gl(int val, int min, int max){
	return (2.f*val-(min+max)) / (max-min);
}

int gl2px(float val, int min, int max){
	return ((val*(max-min))+max+min)/2.f;
}