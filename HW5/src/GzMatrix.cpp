#include "GzMatrix.h"



//============================================================================
//Implementations in Assignment #3
//============================================================================


//Class represents for matrices-----------------------------------------------
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

GzVertex GzMatrix::toVertex() {
	assert((nRow()==4)&&(nCol()==1));
	//Convert to vertex, remember to divide X, Y, Z coordinates by W
	//See http://en.wikipedia.org/wiki/Homogeneous_coordinates#Use_in_computer_graphics
	//    http://en.wikipedia.org/wiki/Transformation_matrix
	//Or google: "homogeneous coordinates"
	GzVertex v;
	GzReal w=at(3)[0];
	v[X]=at(0)[0]/w;
	v[Y]=at(1)[0]/w;
	v[Z]=at(2)[0]/w;
	return v;
}

void GzMatrix::fromVertex(const GzVertex& v) {
	//Convert from vertex, w is set to 1
	//See http://en.wikipedia.org/wiki/Homogeneous_coordinates#Use_in_computer_graphics
	//    http://en.wikipedia.org/wiki/Transformation_matrix
	//Or google: "homogeneous coordinates"
	resize(4, 1);
	at(0)[0]=v[X];
	at(1)[0]=v[Y];
	at(2)[0]=v[Z];
	at(3)[0]=1;
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



//============================================================================
//Implementations in Assignment #4
//============================================================================



GzMatrix GzMatrix::inverse3x3() {
	assert((nRow()==3)&&(nCol()==3));
	
	GzVector x0, x1, x2;
	x0[0]=at(0)[0]; x1[0]=at(0)[1]; x2[0]=at(0)[2]; 
	x0[1]=at(1)[0]; x1[1]=at(1)[1]; x2[1]=at(1)[2];
	x0[2]=at(2)[0]; x1[2]=at(2)[1]; x2[2]=at(2)[2];

	GzReal detA=dotProduct(x0, crossProduct(x1, x2));
	assert(fabs(detA)>1e-6);

	GzVector r0, r1, r2;
	r0=crossProduct(x1, x2);
	r1=crossProduct(x2, x0);
	r2=crossProduct(x0, x1);

	GzMatrix res;
	res.resize(3, 3);
	res[0][0]=r0[0]/detA; res[0][1]=r0[1]/detA; res[0][2]=r0[2]/detA; 
	res[1][0]=r1[0]/detA; res[1][1]=r1[1]/detA; res[1][2]=r1[2]/detA; 
	res[2][0]=r2[0]/detA; res[2][1]=r2[1]/detA; res[2][2]=r2[2]/detA; 

	return res;
}

GzMatrix GzMatrix::transpose() {
	GzMatrix res;
	res.resize(nCol(), nRow());
	for (int i=0; i<nRow(); i++)
		for (int j=0; j<nCol(); j++) res[j][i]=at(i)[j];
	return res;
}



//============================================================================
//End of Implementations in Assignment #4
//============================================================================
