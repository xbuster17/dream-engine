#ifndef __DE_MATH_SHAPES_H__
#define __DE_MATH_SHAPES_H__
#include <stdbool.h>
#include "vector.h"

typedef struct frustum{
	v4f planes[6];// left right top bottom near far
} frustum;

typedef struct aabb{
	v4f min;
	v4f max;
} aabb;

frustum frustum_create(m4f m, bool normalize);
bool point_in_frustum(frustum f, v4f pt);
bool sphere_in_frustum(frustum f, v4f pt, float rad);
bool aabb_in_frustum(frustum f, aabb box);
bool point_in_aabb(aabb box, v4f p);
#endif
