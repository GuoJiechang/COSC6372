#ifndef __GZ_COMMON_H_
#define __GZ_COMMON_H_

#include <vector>
using namespace std;



//============================================================================
//Declarations in Assignment #1
//============================================================================

//Common data type------------------------------------------------------------
typedef int GzInt;
typedef bool GzBool;
typedef double GzReal;
typedef unsigned int GzFunctional;
typedef unsigned int GzPrimitiveType;
//----------------------------------------------------------------------------

//Funtional constants---------------------------------------------------------
#define GZ_DEPTH_TEST   0x00000001
#define GZ_COLOR_BUFFER 0x00000002
#define GZ_DEPTH_BUFFER 0x00000004
//----------------------------------------------------------------------------

//Primitive types-------------------------------------------------------------
#define GZ_POINTS 0
//----------------------------------------------------------------------------

//3D coordinate data type-----------------------------------------------------
#define X 0
#define Y 1
#define Z 2

struct GzVertex:public vector<GzReal> {
	GzVertex():vector<GzReal>(3, 0) {}
	GzVertex(GzReal x, GzReal y, GzReal z):vector<GzReal>(3, 0) {
		at(X)=x; at(Y)=y; at(Z)=z;
	}
};
//----------------------------------------------------------------------------

//Color data type-------------------------------------------------------------
#define R 0
#define G 1
#define B 2
#define A 3

#include <iostream>
using namespace std;

struct GzColor:public vector<GzReal> {
	GzColor():vector<GzReal>(4, 0) {at(A)=1;}
	GzColor(GzReal r, GzReal g, GzReal b):vector<GzReal>(4, 0) {
		at(R)=r; at(G)=g; at(B)=b; at(A)=1;
	}
	GzColor(GzReal r, GzReal g, GzReal b, GzReal a):vector<GzReal>(4, 0) {
		at(R)=r; at(G)=g; at(B)=b; at(A)=a;
	}
};
//----------------------------------------------------------------------------

//============================================================================
//End of Declarations in Assignment #1
//============================================================================



#endif
