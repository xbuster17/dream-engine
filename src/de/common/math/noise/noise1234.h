/* MODIFIED */

// Noise1234
// Author: Stefan Gustavson (stegu@itn.liu.se)
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
		\brief Declares the Noise1234 class for producing Perlin noise.
		\author Stefan Gustavson (stegu@itn.liu.se)
*/

/*
 * This is a clean, fast, modern and free Perlin noise class in C++.
 * Being a stand-alone class with no external dependencies, it is
 * highly reusable without source code modifications.
 *
 * Note:
 * Replacing the "float" type with "double" can actually make this run faster
 * on some platforms. A templatized version of Noise1234 could be useful.
 */

// class Noise1234 {

//   public:
    // Noise1234() {}
    // ~Noise1234() {}

/** 1D, 2D, 3D and 4D double Perlin noise, SL "noise()"
 */
double Noise1234_noise1( double x );
double Noise1234_noise2( double x, double y );
double Noise1234_noise3( double x, double y, double z );
double Noise1234_noise4( double x, double y, double z, double w );

/** 1D, 2D, 3D and 4D double Perlin periodic noise, SL "pnoise()"
 */
double Noise1234_pnoise1( double x, int px );
double Noise1234_pnoise2( double x, double y, int px, int py );
double Noise1234_pnoise3( double x, double y, double z, int px, int py, int pz );
double Noise1234_pnoise4( double x, double y, double z, double w,
                              int px, int py, int pz, int pw );

  // private:
extern unsigned char Noise1234_perm[];
double  Noise1234_grad1( int hash, double x );
double  Noise1234_grad2( int hash, double x, double y );
double  Noise1234_grad3( int hash, double x, double y , double z );
double  Noise1234_grad4( int hash, double x, double y, double z, double t );

// };
