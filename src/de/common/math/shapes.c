#include "shapes.h"

// frustum
// takes in view-projection matrix
frustum frustum_create(m4f m, bool normalize){
/* source: Fast Extraction of Viewing Frustum Planes from the World-View-Projection Matrix - Gil Gribb, Klaus Hartmann*/
	v4f v0 = {m[0],m[4],m[8],m[12]};
	v4f v1 = {m[1],m[5],m[9],m[13]};
	v4f v2 = {m[2],m[6],m[10],m[14]};
	v4f v3 = {m[3],m[7],m[11],m[15]};
	// v4f v0 = {m[0],m[1],m[2],m[3]};
	// v4f v1 = {m[4],m[5],m[6],m[7]};
	// v4f v2 = {m[8],m[9],m[10],m[11]};
	// v4f v3 = {m[12],m[13],m[14],m[15]};
	frustum fr = {
		.planes[0] = v3 + v0,// left
		.planes[1] = v3 - v0,// right
		.planes[2] = v3 - v1,// top
		.planes[3] = v3 + v1,// bottom
		.planes[4] = v3 + v2,// near
		.planes[5] = v3 - v2 // far
	};
	if(normalize){
		fr.planes[0] /= (float)v4f_len(fr.planes[0]);
		fr.planes[1] /= (float)v4f_len(fr.planes[1]);
		fr.planes[2] /= (float)v4f_len(fr.planes[2]);
		fr.planes[3] /= (float)v4f_len(fr.planes[3]);
		fr.planes[4] /= (float)v4f_len(fr.planes[4]);
		fr.planes[5] /= (float)v4f_len(fr.planes[5]);
	}
	return fr;
}



bool dePointInFrustum(frustum f, v4f pt){
	int i ;
	for(i=0 ; i < 6; i++){
		if(v4f_dot(f.planes[i], pt) < 0){
			return false;
		}
	}
	return true;
}



bool deSphereInFrustum(frustum f, v4f pt, float rad){
	int i ;
	pt[3] = 0;
	for(i=0 ; i < 6; i++){
		if(v4f_dot(f.planes[i], pt) < -rad){
			return false;
		}
	}
	return true;
}


bool deAabbInFrustum(frustum f, aabb box){
	int i;//,out, min;
	for (i=0; i<6; i++){
		int out = 0;
		out += ((v4f_dot(f.planes[i], v4f_(box.min[0], box.min[1], box.min[2], 1.0f)) <0.0)?1:0);
		out += ((v4f_dot(f.planes[i], v4f_(box.max[0], box.min[1], box.min[2], 1.0f)) <0.0)?1:0);
		out += ((v4f_dot(f.planes[i], v4f_(box.min[0], box.max[1], box.min[2], 1.0f)) <0.0)?1:0);
		out += ((v4f_dot(f.planes[i], v4f_(box.max[0], box.max[1], box.min[2], 1.0f)) <0.0)?1:0);
		out += ((v4f_dot(f.planes[i], v4f_(box.min[0], box.min[1], box.max[2], 1.0f)) <0.0)?1:0);
		out += ((v4f_dot(f.planes[i], v4f_(box.max[0], box.min[1], box.max[2], 1.0f)) <0.0)?1:0);
		out += ((v4f_dot(f.planes[i], v4f_(box.min[0], box.max[1], box.max[2], 1.0f)) <0.0)?1:0);
		out += ((v4f_dot(f.planes[i], v4f_(box.max[0], box.max[1], box.max[2], 1.0f)) <0.0)?1:0);
		if (out == 8) return false;
	}
	//check frustum outside/inside box
	// int out;
	// out=0; for ( int  i=0; i<8; i++ ) out += ((fru.mPoints[i].x > box.max[0])?1:0); if( out==8 ) return false;
	// out=0; for ( int  i=0; i<8; i++ ) out += ((fru.mPoints[i].x < box.min[0])?1:0); if( out==8 ) return false;
	// out=0; for ( int  i=0; i<8; i++ ) out += ((fru.mPoints[i].y > box.max[0])?1:0); if( out==8 ) return false;
	// out=0; for ( int  i=0; i<8; i++ ) out += ((fru.mPoints[i].y < box.min[0])?1:0); if( out==8 ) return false;
	// out=0; for ( int  i=0; i<8; i++ ) out += ((fru.mPoints[i].z > box.max[0])?1:0); if( out==8 ) return false;
	// out=0; for ( int  i=0; i<8; i++ ) out += ((fru.mPoints[i].z < box.min[0])?1:0); if( out==8 ) return false;
	return true;
}

bool dePointInAabb(aabb box, v4f p){
	return
		(p[0] > box.min[0]) &&
		(p[0] < box.max[0]) &&
		(p[1] > box.min[1]) &&
		(p[1] < box.max[1]) &&
		(p[2] > box.min[2]) &&
		(p[2] > box.max[2]);

}
