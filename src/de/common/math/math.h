#ifndef __DE_MATH_H__
#define __DE_MATH_H__
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

#ifndef PI
#define PI    3.14159265359
#endif
#ifndef DE_PI
#define DE_PI 3.14159265359
#endif

#define min(a,b) ((a) < (b) ? (a) : (b))
#define max(a,b) ((a) > (b) ? (a) : (b))

#define lerp(a,b,w) ( ( (a)*( 1-(w) ) )+( (b)*(w) ) )
#define clamp(in,min,max)\
	(((in)>=(max))?(max):(((in)<=(min))?(min):(in)))

// maps variable x from range[a,b] to [c,d]
#define mapval(x, a,b, c,d) /*maps variable x from range[a,b] to [c,d]*/\
	( (((x) - (a)) / ((b) - (a))) * ((d) - (c)) + (c) )

#define deg2rad(deg) (((deg)*PI)/180.f)
#define rad2deg(rad) (((rad)*180.f)/PI)

#define hash4(x,y,z,seed) (((x)*73856093 ^ (y)*19349663 ^ (z)*179426549 ^ (seed)*19349663))
#define hash3(x,y,seed) (((x)*73856093 ^ (y)*15257039477 ^ (seed)*19349663))
#define hash2(x,y) (((x)*73856093 ^ (y)*83492791))


// static inline int floor(float x){ int i = (int)x; if(i>x)i--; return i;}
// static inline int ceil(float x){  int i = (int)x; if(i<x)i++; return i;}

int rand2(int x, int y, int seed);
int rand3(int x, int y, int z, int seed);

#define smooth_noise(x,y,seed)\
	lerp(\
		hash3(x-1,y-1,seed),\
		hash3(x+1,y+1,seed),\
		0.5f)


#include "vector.h"
#include "noise/noise.h"
#include "shapes.h"







#endif
