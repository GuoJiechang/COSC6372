#include "GzVector.h"

GzVector::GzVector():vector<double>(3, 0) {
}

GzVector::GzVector(double x, double y, double z):vector<double>(3) {
	at(0)=x; at(1)=y; at(2)=z;
}

double GzVector::length() {
	return sqrt(at(0)*at(0)+at(1)*at(1)+at(2)*at(2));
}

void GzVector::normalize() {
	double l=length();
	at(0)/=l;
	at(1)/=l;
	at(2)/=l;
}

GzVector operator + (const GzVector& a, const GzVector& b) {
	return GzVector(a[0]+b[0], a[1]+b[1], a[2]+b[2]);
}

GzVector operator - (const GzVector& a, const GzVector& b) {
	return GzVector(a[0]-b[0], a[1]-b[1], a[2]-b[2]);
}

double dotProduct(const GzVector& a, const GzVector& b) {
	return a[0]*b[0]+a[1]*b[1]+a[2]*b[2];
}

GzVector crossProduct(const GzVector& a, const GzVector& b) {
	GzVector res;
	res[0]=a[1]*b[2]-a[2]*b[1];
	res[1]=a[2]*b[0]-a[0]*b[2];
	res[2]=a[0]*b[1]-a[1]*b[0];
	return res;
}

GzVector operator * (GzReal a, const GzVector& b) {
	GzVector res;
	for (int i=0; i<3; i++) res[i]=a*b[i];
	return res;
}

GzVector operator * (const GzVector& a, GzReal b) {
	GzVector res;
	for (int i=0; i<3; i++) res[i]=a[i]*b;
	return res;
}

GzVector operator / (const GzVector& a, GzReal b) {
	GzVector res;
	for (int i=0; i<3; i++) res[i]=a[i]/b;
	return res;
}

