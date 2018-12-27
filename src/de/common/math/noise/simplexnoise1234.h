/* MODIFIED */
// SimplexNoise1234
// Copyright © 2003-2011, Stefan Gustavson
//
// Contact: stegu@itn.liu.se
//
// This library is public domain software, released by the author
// into the public domain in February 2011. You may do anything
// you like with it. You may even remove all attributions,
// but of course I'd appreciate it if you kept my name somewhere.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.

/** \file
		\brief Declares the SimplexNoise1234 class for producing Perlin simplex noise.
		\author Stefan Gustavson (stegu@itn.liu.se)
*/

/*
 * This is a clean, fast, modern and free Perlin Simplex noise class in C++.
 * Being a stand-alone class with no external dependencies, it is
 * highly reusable without source code modifications.
 *
 *
 * Note:
 * Replacing the "double" type with "double" can actually make this run faster
 * on some platforms. A templatized version of SimplexNoise1234 could be useful.
 */

// class SimplexNoise1234 {

//   public:
//     SimplexNoise1234() {}
//     ~SimplexNoise1234() {}

/** 1D, 2D, 3D and 4D double Perlin noise
 */
double SimplexNoise1234_noise1( double x );
double SimplexNoise1234_noise2( double x, double y );
double SimplexNoise1234_noise3( double x, double y, double z );
double SimplexNoise1234_noise4( double x, double y, double z, double w );

/** 1D, 2D, 3D and 4D double Perlin noise, with a specified integer period
 */
double SimplexNoise1234_pnoise1( double x, int px );
double SimplexNoise1234_pnoise2( double x, double y, int px, int py );
double SimplexNoise1234_pnoise3( double x, double y, double z, int px, int py, int pz );
double SimplexNoise1234_pnoise4( double x, double y, double z, double w,
                              int px, int py, int pz, int pw );

  // private:
// unsigned char SimplexNoise1234_perm[];
double  SimplexNoise1234_grad1( int hash, double x );
double  SimplexNoise1234_grad2( int hash, double x, double y );
double  SimplexNoise1234_grad3( int hash, double x, double y , double z );
double  SimplexNoise1234_grad4( int hash, double x, double y, double z, double t );

// };
