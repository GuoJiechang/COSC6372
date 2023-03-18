#include "GzMatrix.h"
#include<math.h>


//============================================================================
//Implementations in Assignment #3
//============================================================================


//Class represents for matrices-----------------------------------------------
GzVertex GzMatrix::toVertex() {
	assert((nRow()==4)&&(nCol()==1));
	//You need to complete this function to use the Matrix to represents
	//vertices with homogeneous coordinate.
	//Convert to vertex, remember to divide X, Y, Z coordinates by W
	//See http://en.wikipedia.org/wiki/Homogeneous_coordinates#Use_in_computer_graphics
	//    http://en.wikipedia.org/wiki/Transformation_matrix
	//Or google: "homogeneous coordinates"
	GzVertex v;
	v[0] = at(0).at(0)/at(3).at(0);
	v[1] = at(1).at(0)/at(3).at(0);
	v[2] = at(2).at(0)/at(3).at(0);
	return v;
}

void GzMatrix::fromVertex(const GzVertex& v) {
	//You need to complete this function to use the Matrix to represents
	//vertices with homogeneous coordinate.
	//Convert from vertex, w is set to 1
	//See http://en.wikipedia.org/wiki/Homogeneous_coordinates#Use_in_computer_graphics
	//    http://en.wikipedia.org/wiki/Transformation_matrix
	//Or google: "homogeneous coordinates"
	resize(4,1);
	at(0).at(0) = v[0];
	at(1).at(0) = v[1];
	at(2).at(0) = v[2];
	at(3).at(0) = 1.0;
}

void GzMatrix::Normalize()
{
	int m = nRow();
	int n = nCol();
	if(n != 1 && m < 1)
	{
		return;
	}

	GzReal length = 0.0;
	for(GzInt i=0; i < m; i++)
	{
		length += at(i).at(0) *  at(i).at(0);
	}
	length = sqrt(length);

	for(GzInt i=0; i < m; i++)
	{
		at(i).at(0) /= length;
	}
}

void GzMatrix::resize(GzInt _nRow, GzInt _nCol) {
	vector<vector<GzReal> >::resize(_nRow);
	for (GzInt i=0; i<_nRow; i++) 
		at(i).resize(_nCol);
}

GzInt GzMatrix::nRow() const {
	return (GzInt)size();
}

GzInt GzMatrix::nCol() const {
	if (size()==0) return 0;
	return at(0).size();
}
//----------------------------------------------------------------------------


//Matrix operators------------------------------------------------------------
GzMatrix operator + (const GzMatrix& a, const GzMatrix& b) {
	assert((a.nRow()==b.nRow())&&(a.nCol()==b.nCol()));
	int m=a.nRow();
	int n=a.nCol();
	GzMatrix c;
	c.resize(m, n);
	for (int i=0; i<m; i++)
		for (int j=0; j<n; j++) c[i][j]=a[i][j]+b[i][j];
	return c;
}

GzMatrix operator - (const GzMatrix& a, const GzMatrix& b) {
	assert((a.nRow()==b.nRow())&&(a.nCol()==b.nCol()));
	int m=a.nRow();
	int n=a.nCol();
	GzMatrix c;
	c.resize(m, n);
	for (int i=0; i<m; i++)
		for (int j=0; j<n; j++) c[i][j]=a[i][j]-b[i][j];
	return c;
}

GzMatrix operator * (const GzMatrix& a, const GzMatrix& b) {
	assert(a.nCol()==b.nRow());
	int m=a.nRow();
	int n=a.nCol();
	int p=b.nCol();
	GzMatrix c;
	c.resize(m, p);
	for (int i=0; i<m; i++)
		for (int j=0; j<p; j++) {
			c[i][j]=0;
			for (int k=0; k<n; k++) c[i][j]+=a[i][k]*b[k][j];
		}
	return c;
}
GzMatrix crossProduct(const GzMatrix &a, const GzMatrix &b)
{
	assert(a.nCol() == 1);
	assert(a.nRow() == 3);
	assert(a.nCol() == b.nCol());
	assert(a.nRow() == b.nRow());
	GzMatrix c;
	int m=a.nRow();
	int n=a.nCol();
	c.resize(m,n);

	c[0][0] = a[1][0]*b[2][0] - a[2][0]*b[1][0];
	c[1][0] = a[2][0]*b[0][0] - a[0][0]*b[2][0];
	c[2][0] = a[0][0]*b[1][0] - a[1][0]*b[0][0];

    return c;
}
//----------------------------------------------------------------------------

//Some default matrices-------------------------------------------------------
GzMatrix Zeros(GzInt _n) {
	GzMatrix a;
	a.resize(_n, _n);
	for (int i=0; i<_n; i++)
		for (int j=0; j<_n; j++) a[i][j]=0;
	return a;
}

GzMatrix Identity(GzInt _n) {
	GzMatrix a=Zeros(_n);
	for (int i=0; i<_n; i++) a[i][i]=1;
	return a;
}
//----------------------------------------------------------------------------


//============================================================================
//End of Implementations in Assignment #3
//============================================================================
