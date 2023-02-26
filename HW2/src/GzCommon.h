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
	GzColor operator * (GzReal s) {
        GzColor res;
        res.at(R) = at(R)*s;
        res.at(G) = at(G)*s;
		res.at(B) = at(B)*s;
		res.at(A) = at(A)*s;
        return res;
    }
	GzColor operator + (GzColor const &obj) {
        GzColor res;
        res.at(R) = at(R) + obj.at(R);
        res.at(G) = at(G) + obj.at(G);
		res.at(B) = at(B) + obj.at(B);
		res.at(A) = at(A) + obj.at(A);
        return res;
    }
};


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
	GzColor color; //color attribute of the vertex
};
//----------------------------------------------------------------------------


//============================================================================
//End of Declarations in Assignment #1
//============================================================================




//============================================================================
//Declarations in Assignment #2
//============================================================================

//Primitive types-------------------------------------------------------------
#define GZ_TRIANGLES 1
//----------------------------------------------------------------------------

//struct for triangle
struct GzTriangle{
	GzTriangle(){};
	GzTriangle(GzVertex a, GzVertex b, GzVertex c)
	{
		vertices.clear();
		vertices.push_back(a);
		vertices.push_back(b);
		vertices.push_back(c);
	}
	vector<GzVertex> vertices;
};

//struct for 2D bounding box
struct Gz2DBoundingBox{
	Gz2DBoundingBox(){}
	GzVertex min;
	GzVertex max;
};
//============================================================================
//End of Declarations in Assignment #2
//============================================================================



#endif
