#ifndef __DE_MATH_SHAPES_H__
#define __DE_MATH_SHAPES_H__
#include <stdbool.h>
#include "vector.h"

typedef struct dfrustum{
	v4f planes[6];// left right top bottom near far
} dfrustum;

typedef struct daabb{
	v4f min;
	v4f max;
} daabb;


dfrustum dfrustum_make(m4f m, bool normalize);
daabb daabb_make(v4f min, v4f max);
bool dfrustum_point(dfrustum f, v4f pt);
bool dfrustum_has_sphere(dfrustum f, v4f pt, float rad);
bool dfrustum_aabb(dfrustum f, daabb box);
bool daabb_point(daabb box, v4f p);

#endif
