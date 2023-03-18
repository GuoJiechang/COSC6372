#include "Gz.h"
#include <math.h>
#include <iostream>
#include <string>

using namespace std;

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

void Gz::initFrameSize(GzInt width, GzInt height) {
	//This function need to be updated since we have introduced the viewport.
	//The viewport size is set to the size of the frame buffer.
	wViewport=(GzReal)width;
	hViewport=(GzReal)height;
	frameBuffer.initFrameSize(width, height);
	viewport(0, 0);			//Default center of the viewport 
	transMatrix = Identity(4);
	prjMatrix = Identity(4);
}

void Gz::end() {
	//This function need to be updated since we have introduced the viewport,
	//projection, and transformations.
	//In our implementation, all rendering is done when Gz::end() is called.
	//Depends on selected primitive, different number of vetices, colors, ect.
	//are pop out of the queue.
	switch (currentPrimitive) {
		case GZ_POINTS: {
			while ( (vertexQueue.size()>=1) && (colorQueue.size()>=1) ) {
			}
		} break;
		case GZ_TRIANGLES: {
			
			//Put your triangle drawing implementation here:
			//   - Extract 3 vertices in the vertexQueue
			//   - Extract 3 colors in the colorQueue
			//   - Call the draw triangle function 
			//     (you may put this function in GzFrameBuffer)
			while ( (vertexQueue.size()>=3) && (colorQueue.size()>=3) ) {
				GzVertex v1=vertexQueue.front(); vertexQueue.pop();
				GzVertex v2=vertexQueue.front(); vertexQueue.pop();
				GzVertex v3=vertexQueue.front(); vertexQueue.pop();
				vector<GzVertex> vertices;
				vertices.push_back(v1);
				vertices.push_back(v2);
				vertices.push_back(v3);

				//apply model_view, projection matrix
				for(GzInt i = 0; i < vertices.size(); i++)
				{
					GzMatrix v_mat;
					//create a vector from vertex in object space
					v_mat.fromVertex(vertices[i]);
					//transform the object to world space then to view space
					//v_mat = v_mat;
					//transform to clip space
					v_mat = prjMatrix*viewMatrix*transMatrix*v_mat;
					vertices[i] = v_mat.toVertex();
					//viewport transform to screen space
					vertices[i][0] = (vertices[i][0] + 1.0)*(wViewport/2.0) + xViewport;
					vertices[i][1] = (vertices[i][1] + 1.0)*(hViewport/2.0) + yViewport;
					vertices[i][2] = (vertices[i][2] + 1.0)/2.0;
				}

				GzColor c1=colorQueue.front(); colorQueue.pop();
				GzColor c2=colorQueue.front(); colorQueue.pop();
				GzColor c3=colorQueue.front(); colorQueue.pop();

				vector<GzColor> colors;
				colors.push_back(c1);
				colors.push_back(c2);
				colors.push_back(c3);

				
				frameBuffer.drawTriangle(vertices,colors,status);
			}
		}
	}
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
	
	//set transMatrix and prjMatrix to default
	transMatrix = Identity(4);
	prjMatrix = Identity(4);

	GzMatrix center_v;
	center_v.resize(3,1);
	center_v[0][0] = centerX;
	center_v[1][0] = centerY;
	center_v[2][0] = centerZ;

	GzMatrix eye_v;
	eye_v.resize(3,1);
	eye_v[0][0] = eyeX;
	eye_v[1][0] = eyeY;
	eye_v[2][0] = eyeZ;

	GzMatrix F;
	F.resize(3,1);
	F = center_v - eye_v;
	F.Normalize();

	GzMatrix up_v;
	up_v.resize(3,1);
	up_v[0][0] = upX;
	up_v[1][0] = upY;
	up_v[2][0] = upZ;
	up_v.Normalize();

	GzMatrix s;
	s.resize(3,1);
	s = crossProduct(F,up_v);
	s.Normalize();

	//GzMatrix s_normlized = s;
	//s_normlized.Normalize();

	GzMatrix u;
	u.resize(3,1);
	u = crossProduct(s,F);

	viewMatrix = Identity(4);
	viewMatrix[0][0] = s[0][0];
	viewMatrix[0][1] = s[1][0];
	viewMatrix[0][2] = s[2][0];

	viewMatrix[1][0] = u[0][0];
	viewMatrix[1][1] = u[1][0];
	viewMatrix[1][2] = u[2][0];

	viewMatrix[2][0] = -F[0][0];
	viewMatrix[2][1] = -F[1][0];
	viewMatrix[2][2] = -F[2][0];

	GzMatrix translate = Identity(4);
	translate[0][3] = -eyeX;
	translate[1][3] = -eyeY;
	translate[2][3] = -eyeZ;

	viewMatrix = viewMatrix * translate;

}

void Gz::translate(GzReal x, GzReal y, GzReal z) {
	//Multiply transMatrix by a translation matrix
	//See http://www.opengl.org/sdk/docs/man/xhtml/glTranslate.xml
	//    http://en.wikipedia.org/wiki/Translation_(geometry)
	//Or google: glTranslate
	GzMatrix translate = Identity(4);
	translate[0][3] = x;
	translate[1][3] = y;
	translate[2][3] = z;

	multMatrix(translate);
}

void Gz::rotate(GzReal angle, GzReal x, GzReal y, GzReal z) {
	//Multiply transMatrix by a rotation matrix
	//See http://www.opengl.org/sdk/docs/man/xhtml/glRotate.xml
	//    http://en.wikipedia.org/wiki/Rotation_(geometry)
	//Or google: glRotate
	double pi = 3.14159265359;
    angle = angle * pi / 180.0;

	GzReal c = cos(angle);
	GzReal s = sin(angle);
	GzMatrix axis;
	axis.resize(3,1);
	axis[0][0] = x;
	axis[1][0] = y;
	axis[2][0] = z;
	axis.Normalize();
	x = axis[0][0];
	y = axis[1][0];
	z = axis[2][0];

	GzMatrix rotate = Identity(4);
	rotate[0][0] = c + x*x*(1-c);
	rotate[0][1] = x*y*(1-c) - z*s;
	rotate[0][2] = x*z*(1-c) + y*s;

	rotate[1][0] = y*x*(1-c) + z*s;
	rotate[1][1] = c + y*y*(1-c);
	rotate[1][2] = y*z*(1-c) - x*s;

	rotate[2][0] = z*x*(1-c) - y*s;
	rotate[2][1] = z*y*(1-c) + x*s;
	rotate[2][2] = c + z*z*(1-c);

	multMatrix(rotate);
}

void Gz::scale(GzReal x, GzReal y, GzReal z) {
	//Multiply transMatrix by a scaling matrix
	//See http://www.opengl.org/sdk/docs/man/xhtml/glScale.xml
	//    http://en.wikipedia.org/wiki/
	//Or google: glScale
	GzMatrix scale = Identity(4);
	scale[0][0] = x;
	scale[1][1] = y;
	scale[2][2] = z;

	multMatrix(scale);
}

//This function was updated on September 26, 2010
void Gz::multMatrix(GzMatrix mat) {
	//Multiply transMatrix by the matrix mat

	//transMatrix=mat*transMatrix;

	//the order of the matrix multiply do matters. The matrix being set first should be applied to the vertex first.
	//the oder of the transformation should be scale, rotation, translate.
	// V_world = T*R*S*V_local
	transMatrix= transMatrix * mat;
}
//End of Transformations------------------------------------------------------

//Projections-----------------------------------------------------------------
void Gz::perspective(GzReal fovy, GzReal aspect, GzReal zNear, GzReal zFar) {
	//Set up a perspective projection matrix
	//See http://www.opengl.org/sdk/docs/man/xhtml/gluPerspective.xml
	//Or google: gluPerspective
	double pi = 3.14159265359;
    fovy = fovy * pi / 180.0;
	GzReal f = tan(fovy/2.0);
	f = 1.0/f;
	
	prjMatrix = Zeros(4);
	prjMatrix[0][0] = f/aspect;
	prjMatrix[1][1] = f;
	prjMatrix[2][2] = -(zFar+zNear)/(zNear-zFar);
	prjMatrix[2][3] = -(2.0*zFar*zNear)/(zNear-zFar);
	prjMatrix[3][2] = -1.0;
}

void Gz::orthographic(GzReal left, GzReal right, GzReal bottom, GzReal top, GzReal nearVal, GzReal farVal) {
	//Set up a orthographic projection matrix
	//See http://www.opengl.org/sdk/docs/man/xhtml/glOrtho.xml
	//Or google: glOrtho
	GzReal tx = -(right + left)/(right - left);
	GzReal ty = -(top + bottom)/(top - bottom);
	GzReal tz = -(farVal + nearVal)/(farVal - nearVal);

	prjMatrix = Identity(4);
	prjMatrix[0][0] = 2.0/(right-left);
	prjMatrix[1][1] = 2.0/(top-bottom);
	prjMatrix[2][2] = 2.0/(farVal-nearVal);

	prjMatrix[0][3] = tx;
	prjMatrix[1][3] = ty;
	prjMatrix[2][3] = -tz;

}
//End of Projections----------------------------------------------------------


//============================================================================
//End of Implementations in Assignment #3
//============================================================================
