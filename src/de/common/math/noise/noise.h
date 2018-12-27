#ifndef __DE_NOISE_H__
#define __DE_NOISE_H__
#include "../vector.h"

/* perlin noise: R -> [-1, 1] */
double noise1(double x);
double noise2(v2d x);
double noise3(v4d x);
double noise4(v4d x);

/* periodic perlin: R, Z!=0 -> [-1, 1] */
/* specify the repeat period in p. p should be !=0 in every component*/
double noise1p(double x, int p);
double noise2p(v2d x, v2i p);
double noise3p(v4d x, v4i p);
double noise4p(v4d x, v4i p);

/* simplex noise with analytical derivative: R>0 -> [-1, 1] */
/* doesn't work (correctly) with negative or large numbers. */
/* if d is not null then it will be filled with the derivative. */
double noise1d(double x, double* d);
double noise2d(v2d x, v2d* d);
double noise3d(v4d x, v4d* d);
double noise4d(v4d x, v4d* d);



/* Perlin and Simplex noise thanks to:
 * Copyright © 2003-2011, Stefan Gustavson
 *
 * Contact: stefan.gustavson@gmail.com
 *
 * This library is public domain software, released by the author
 * into the public domain in February 2011.
 */
#include "simplexnoise1234.h" // not used
#include "sdnoise1234.h" // simplex with derivative
#include "noise1234.h" // perlin noise and periodic perlin noise

#endif
