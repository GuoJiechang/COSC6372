#ifndef __GZ_VECTOR_H_
#define __GZ_VECTOR_H_

#include "GzCommon.h"
#include <cmath>
using namespace std;

//============================================================================
//Declarations in Assignment #4
//============================================================================

//3D vector data type---------------------------------------------------------
struct GzVector:public vector<GzReal> {
	GzVector();
	GzVector(GzReal x, GzReal y, GzReal z);
	GzReal length();
	void normalize();
};
//----------------------------------------------------------------------------

//Vector operators------------------------------------------------------------
GzVector operator + (const GzVector& a, const GzVector& b);
GzVector operator - (const GzVector& a, const GzVector& b);
GzReal dotProduct(const GzVector& a, const GzVector& b);
GzVector crossProduct(const GzVector& a, const GzVector& b);

GzVector operator * (GzReal a, const GzVector& b);
GzVector operator * (const GzVector& a, GzReal b);
GzVector operator / (const GzVector& a, GzReal b);
//----------------------------------------------------------------------------

//============================================================================
//End of Declarations in Assignment #4
//============================================================================

#endif