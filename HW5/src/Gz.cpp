#include<iostream>
#include "Gz.h"



//============================================================================
//Implementations in Assignment #1
//============================================================================
GzImage Gz::toImage() {
	return frameBuffer.toImage();
}

void Gz::clear(GzFunctional buffer) {
	frameBuffer.clear(buffer);
}

void Gz::clearColor(const GzColor& color) {
	frameBuffer.setClearColor(color);
}

void Gz::clearDepth(GzReal depth) {
	frameBuffer.setClearDepth(depth);
}

void Gz::enable(GzFunctional f) {
	status=status|f;
}

void Gz::disable(GzFunctional f) {
	status=status&(~f);
}

GzBool Gz::get(GzFunctional f) {
	if (status&f) return true; else return false;
}

void Gz::begin(GzPrimitiveType p) {
	currentPrimitive=p;
}

void Gz::addVertex(const GzVertex& v) {
	vertexQueue.push(v);
}

void Gz::addColor(const GzColor& c) {
	colorQueue.push(c);
}
//============================================================================
//End of Implementations in Assignment #1
//============================================================================



//============================================================================
//Implementations in Assignment #3
//============================================================================

//Design function
GzVertex Gz::transAll(GzVertex& v) {
	GzMatrix res;
	res.fromVertex(v);
	res=prjMatrix*transMatrix*res;
	GzVertex vRes=res.toVertex();
	vRes[X]=(vRes[X]+1)*wViewport/2+xViewport;
	vRes[Y]=(vRes[Y]+1)*hViewport/2+yViewport;
	return vRes;
}

void Gz::initFrameSize(GzInt width, GzInt height) {
	//This function need to be updated since we have introduced the viewport.
	//The viewport size is set to the size of the frame buffer.
	wViewport=(GzReal)width;
	hViewport=(GzReal)height;
	frameBuffer.initFrameSize(width, height);
	viewport(0, 0);			//Default center of the viewport 
}

void Gz::viewport(GzInt x, GzInt y) {
	//This function only updates xViewport and yViewport.
	//Viewport calculation will be done in different function, e.g. Gz::end().
	//See http://www.opengl.org/sdk/docs/man/xhtml/glViewport.xml
	//Or google: glViewport
	xViewport=x;
	yViewport=y;
}

//Transformations-------------------------------------------------------------
void Gz::lookAt(GzReal eyeX, GzReal eyeY, GzReal eyeZ, GzReal centerX, GzReal centerY, GzReal centerZ, GzReal upX, GzReal upY, GzReal upZ) {
	//Define viewing transformation
	//See http://www.opengl.org/sdk/docs/man/xhtml/gluLookAt.xml
	//Or google: gluLookAt
	GzVector s, u, UP, f;
	f[X]=centerX-eyeX;
	f[Y]=centerY-eyeY;
	f[Z]=centerZ-eyeZ;
	f.normalize();
	UP[X]=upX;
	UP[Y]=upY;
	UP[Z]=upZ;
	UP.normalize();
	s=crossProduct(f, UP);
	u=crossProduct(s, f);
	//s.normalize();
	//u.normalize();
	
	transMatrix=Identity(4);
	transMatrix[0][0]= s[0]; transMatrix[0][1]= s[1]; transMatrix[0][2]= s[2];
	transMatrix[1][0]= u[0]; transMatrix[1][1]= u[1]; transMatrix[1][2]= u[2];
	transMatrix[2][0]=-f[0]; transMatrix[2][1]=-f[1]; transMatrix[2][2]=-f[2];


	translate(-eyeX, -eyeY, -eyeZ);
}

void Gz::multMatrix(GzMatrix mat) {
	//Multiply transMatrix by the matrix mat
	transMatrix=transMatrix*mat;
}

void Gz::translate(GzReal x, GzReal y, GzReal z) {
	//Multiply transMatrix by a translation matrix
	//See http://www.opengl.org/sdk/docs/man/xhtml/glTranslate.xml
	//    http://en.wikipedia.org/wiki/Translation_(geometry)
	//Or google: glTranslate
	GzMatrix mat=Identity(4);
	mat[0][3]=x;
	mat[1][3]=y;
	mat[2][3]=z;

	multMatrix(mat);
}

void Gz::rotate(GzReal angle, GzReal x, GzReal y, GzReal z) {
	//Multiply transMatrix by a rotation matrix
	//See http://www.opengl.org/sdk/docs/man/xhtml/glRotate.xml
	//    http://en.wikipedia.org/wiki/Rotation_(geometry)
	//Or google: glRotate
	GzReal l=sqrt(x*x+y*y+z*z);
	assert(l>1e-10);
	x/=l; y/=l; z/=l;
	
	GzReal PI=3.14159265358f;
	GzReal s=sin(angle*PI/180);
	GzReal c=cos(angle*PI/180);

	GzMatrix mat=Identity(4);
	mat[0][0]=x*x*(1-c)+c;		mat[0][1]=x*y*(1-c)-z*s;	mat[0][2]=x*z*(1-c)+y*s;
	mat[1][0]=y*x*(1-c)+z*s;	mat[1][1]=y*y*(1-c)+c;		mat[1][2]=y*z*(1-c)-x*s;
	mat[2][0]=x*z*(1-c)-y*s;	mat[2][1]=y*z*(1-c)+x*s;	mat[2][2]=z*z*(1-c)+c;

	multMatrix(mat);
}

void Gz::scale(GzReal x, GzReal y, GzReal z) {
	//Multiply transMatrix by a scaling matrix
	//See http://www.opengl.org/sdk/docs/man/xhtml/glScale.xml
	//    http://en.wikipedia.org/wiki/
	//Or google: glScale
	GzMatrix mat=Identity(4);
	mat[0][0]=x;
	mat[1][1]=y;
	mat[2][2]=z;

	multMatrix(mat);
}
//End of Transformations------------------------------------------------------

//Projections-----------------------------------------------------------------
void Gz::perspective(GzReal fovy, GzReal aspect, GzReal zNear, GzReal zFar) {
	//Set up a perspective projection matrix
	//See http://www.opengl.org/sdk/docs/man/xhtml/gluPerspective.xml
	//Or google: gluPerspective
	prjMatrix=Zeros(4);
	GzReal PI=3.14159265358f;
	GzReal f=1/tan(fovy*PI/360);
	prjMatrix[0][0]=f/aspect;
	prjMatrix[1][1]=f;
	prjMatrix[2][2]=-(zFar+zNear)/(zNear-zFar);
	prjMatrix[2][3]=-2*zFar*zNear/(zNear-zFar);
	prjMatrix[3][2]=-1;
	frameBuffer.IsPerspective(true);
}

void Gz::orthographic(GzReal left, GzReal right, GzReal bottom, GzReal top, GzReal nearVal, GzReal farVal) {
	//Set up a orthographic projection matrix
	//See http://www.opengl.org/sdk/docs/man/xhtml/glOrtho.xml
	//Or google: glOrtho
	GzReal tx=-(right+left)/(right-left);
	GzReal ty=-(top+bottom)/(top-bottom);
	GzReal tz=(farVal+nearVal)/(farVal-nearVal);

	prjMatrix=Identity(4);
	prjMatrix[0][3]=tx;
	prjMatrix[1][3]=ty;
	prjMatrix[2][3]=tz;
	prjMatrix[0][0]=2/(right-left);
	prjMatrix[1][1]=2/(top-bottom);
	prjMatrix[2][2]=2/(farVal-nearVal);
	frameBuffer.IsPerspective(false);
}
//End of Projections----------------------------------------------------------

//============================================================================
//End of Implementations in Assignment #3
//============================================================================



//============================================================================
//Implementations in Assignment #4
//============================================================================

void Gz::addNormal(const GzVector& v) {
	GzVector vn=v;
	vn.normalize();
	normalQueue.push(vn);
}

void Gz::shadeModel(const GzInt model) {
	frameBuffer.shadeModel(model);
}

void Gz::material(GzReal _kA, GzReal _kD, GzReal _kS, GzReal _s) {
	frameBuffer.material(_kA, _kD, _kS, _s);
}
	
void Gz::addLight(const GzVector& v, const GzColor& c) {
	frameBuffer.addLight(v, c);
}

//============================================================================
//End of Implementations in Assignment #4
//============================================================================



//============================================================================
//Implementations in Assignment #5
//============================================================================
void Gz::texture(const GzImage& t) {
	frameBuffer.texture(t);
}

void Gz::addTexCoord(const GzTexCoord& tc) {
	texCoordQueue.push(tc);
}

void Gz::end() {
	//Again this function need to be updated if the GZ_TEXTURE is turned on.
	//Remember to pop texture coordinates from texCoordQueue.
	//Note that in this assignment, we only consider texture mapping without lighting.
	//Also we only need to implement the triangle rasterization.
	
	if (get(GZ_TEXTURE)) {
		switch (currentPrimitive) {
			case GZ_TRIANGLES: {
				while ( (vertexQueue.size()>=3) && (texCoordQueue.size()>=3) )
				{
					vector<GzVertex> v(3);
					vector<GzTexCoord> tc(3);
					vector<GzVertex> vView(3);
					for (int i=0; i<3; i++) {
						GzVertex vert = vertexQueue.front();
						vertexQueue.pop();

						v[i]=transAll(vert);
						vView[i]=transView(vert);

						tc[i] = texCoordQueue.front(); texCoordQueue.pop();
					}
					frameBuffer.drawTriangleWTexture(v, vView, tc, status);
				}
			}
		}
	}

	if (get(GZ_LIGHTING)) {
		frameBuffer.loadLightTrans(transMatrix);
		switch (currentPrimitive) {
			case GZ_POINTS: {
				while ( (vertexQueue.size()>=1) && (colorQueue.size()>=1) && (normalQueue.size()>=1)) {
					GzVertex v=transAll(vertexQueue.front()); vertexQueue.pop();
					GzColor c=colorQueue.front(); colorQueue.pop();
					GzVector n=normalQueue.front(); normalQueue.pop();
					frameBuffer.drawPointWLight(v, c, n, status);
				}
			} break;
			case GZ_TRIANGLES: {
				while ( (vertexQueue.size()>=3) && (colorQueue.size()>=3) && (normalQueue.size()>=3)) {
					vector<GzVertex> v(3);
					vector<GzColor> c(3);
					vector<GzVector> n(3);
					for (int i=0; i<3; i++) {
						v[i]=transAll(vertexQueue.front()); vertexQueue.pop();
						c[i]=colorQueue.front(); colorQueue.pop();
						n[i]=normalQueue.front(); normalQueue.pop();
					}
					frameBuffer.drawTriangleWLight(v, c, n, status);
				}
			} break;
		}
	} else {
		switch (currentPrimitive) {
			case GZ_POINTS: {
				while ( (vertexQueue.size()>=1) && (colorQueue.size()>=1) ) {
					GzVertex v=transAll(vertexQueue.front()); vertexQueue.pop();
					GzColor c=colorQueue.front(); colorQueue.pop();
					frameBuffer.drawPoint(v, c, status);
				}
			} break;
			case GZ_TRIANGLES: {
				while ( (vertexQueue.size()>=3) && (colorQueue.size()>=3) ) {
					vector<GzVertex> v(3);
					vector<GzColor> c(3);
					for (int i=0; i<3; i++) {
						v[i]=transAll(vertexQueue.front()); vertexQueue.pop();
						c[i]=colorQueue.front(); colorQueue.pop();
					}
					frameBuffer.drawTriangle(v, c, status);
				}
			}
		}
	}
}

//============================================================================
//End of Implementations in Assignment #5
//============================================================================

GzVertex Gz::transView(GzVertex &v)
{
    GzMatrix res;
	res.fromVertex(v);
	res=transMatrix*res;
	GzVertex vRes=res.toVertex();
	return vRes;
}
