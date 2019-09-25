// #include "vector.h"
#include "noise.h"

double noise1(double x){return (Noise1234_noise1(x) );}
double noise2(v2d p){return (Noise1234_noise2(p[0], p[1]) );}
double noise3(v4d p){return (Noise1234_noise3(p[0],p[1],p[2]) );}
double noise4(v4d p){return (Noise1234_noise4(p[0],p[1],p[2],p[3]));}

double noise1p(double x, int p){return (Noise1234_pnoise1(x,p) );}
double noise2p(v2d x, v2i p){return (Noise1234_pnoise2(x[0], x[1], p[0], p[1]) );}
double noise3p(v4d x, v4i p){return (Noise1234_pnoise3(x[0],x[1],x[2],p[0],p[1],p[2]) );}
double noise4p(v4d x, v4i p){return (Noise1234_pnoise4(x[0],x[1],x[2],x[3],p[0],p[1],p[2],p[3]));}

double noise1d(double x, double* d){
	if(d!=NULL)
		return sdnoise1(x, d);
	else return sdnoise1(x, NULL);
}
double noise2d(v2d x, v2d* d){
	if(d==NULL)
		return sdnoise2(x[0],x[1], NULL,NULL);

	double ret, dx,dy;
	ret=sdnoise2(x[0],x[1], &dx, &dy);
		// return sdnoise2(x[0],x[1], &(*d)[0],&(*d)[1]);
	(*d)[0]=dx;
	(*d)[1]=dy;
	return ret;
		
}
double noise3d(v4d x, v4d* d){
	if(d==NULL)
		return sdnoise3(x[0],x[1],x[2], NULL,NULL,NULL);

	double ret, dx,dy,dz;
	ret=sdnoise3(x[0],x[1],x[2], &dx,&dy,&dz);
	(*d)[0]=dx;
	(*d)[1]=dy;
	(*d)[2]=dz;
	return ret;
}
double noise4d(v4d x, v4d* d){
	if(d==NULL)
		return sdnoise4(x[0],x[1],x[2],x[3], NULL,NULL,NULL,NULL);

	double ret, dx,dy,dz,dw;
	ret=sdnoise4(x[0],x[1],x[2],x[3], &dx,&dy,&dz,&dw);
	(*d)[0]=dx;
	(*d)[1]=dy;
	(*d)[2]=dz;
	(*d)[3]=dw;
	return ret;
	
}
