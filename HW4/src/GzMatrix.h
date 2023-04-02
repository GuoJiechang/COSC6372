#ifndef __GZ_MATRIX_H_
#define __GZ_MATRIX_H_

#include "GzCommon.h"
#include "GzVector.h"
#include <cassert>
using namespace std;

//============================================================================
//Declarations in Assignment #3
//============================================================================


//Class represents for matrices-----------------------------------------------
class GzMatrix:public vector<vector<GzReal> > {
public:
	//Note: Since this calss inherits std::vector, copy constructor and 
	//      operator = are not required to define. However, you should define
	//      them in general case.
	void resize(GzInt _nRow, GzInt _nCol);		//Resize to _nRow by _nCol
	GzInt nRow() const;							//Return the number of rows
	GzInt nCol() const;							//Return the number of columns
	//Converter between GzVertex and GzMatrix with homogeneous coordinate
	//GzMatrix is a 4 by 1 vector: [0][0]=X, [1][0]=Y, [2][0]=Z, [3][0]=W
	GzVertex toVertex();						//Convert to vertex
	void fromVertex(const GzVertex& v);			//Convert from vertex
	GzMatrix inverse3x3();						//Return the inverse of the 3 by 3 matrix
	GzMatrix transpose();						//Return the transopose matrix
};
//----------------------------------------------------------------------------

//Matrix operators------------------------------------------------------------
GzMatrix operator + (const GzMatrix& a, const GzMatrix& b);		//Returns a+b
GzMatrix operator - (const GzMatrix& a, const GzMatrix& b);		//Returns a-b
GzMatrix operator * (const GzMatrix& a, const GzMatrix& b);		//Returns a*b
//----------------------------------------------------------------------------

//Some default matrices-------------------------------------------------------
GzMatrix Zeros(GzInt _n);		//Zero matrix _n by _n
GzMatrix Identity(GzInt _n);	//Identity matrix _n by _n
//----------------------------------------------------------------------------


//============================================================================
//End of Declarations in Assignment #3
//============================================================================

#endif
