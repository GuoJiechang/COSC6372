#ifndef __GZ_H_
#define __GZ_H_

#include "GzCommon.h"
#include "GzImage.h"
#include "GzFrameBuffer.h"		//For assignment #1, #2
#include "GzMatrix.h"			//For assignment #3
#include <queue>
using namespace std;

class Gz {



//============================================================================
//Declarations in Assignment #1
//============================================================================
public:
	void initFrameSize(GzInt width, GzInt height);	//Initialize
	GzImage toImage();		//Convert the current rendering result to image

	void clear(GzFunctional buffer);		//Clear buffers to preset values
	void clearColor(const GzColor& color);	//Specify clear values for the color buffer
	void clearDepth(GzReal depth);			//Specify the clear value for the depth buffer

	void enable(GzFunctional f);	//Enable some capabilities
	void disable(GzFunctional f);	//Disable some capabilities
	GzBool get(GzFunctional f);		//Return the value of a selected parameter

	void begin(GzPrimitiveType p);	//Delimit the vertices of a primitive
	void end();						//End of limit

	void addVertex(const GzVertex& v);	//Specify a vertex
	void addColor(const GzColor& c);	//Specify a color

private:
	GzFrameBuffer frameBuffer;			//The frame buffer
	queue<GzVertex> vertexQueue;		//Store vertices in queue for rendering
	queue<GzColor> colorQueue;			//Store colors in queue for rendering
	GzPrimitiveType currentPrimitive;	//The current primitive, set by Gz::begin()
	GzFunctional status;				//Current capabilities
//============================================================================
//End of Declarations in Assignment #1
//============================================================================



//============================================================================
//Declarations in Assignment #3
//============================================================================
public:
	void viewport(GzInt x, GzInt y);	//Set the viewport center at (x, y)

	//Transformations---------------------------------------------------------
	void lookAt(GzReal eyeX, GzReal eyeY, GzReal eyeZ, 
				GzReal centerX, GzReal centerY, GzReal centerZ, 
				GzReal upX, GzReal upY, GzReal upZ);			//Define viewing transformation
	void translate(GzReal x, GzReal y, GzReal z);				//Multiply transMatrix by a translation matrix
	void rotate(GzReal angle, GzReal x, GzReal y, GzReal z);	//Multiply transMatrix by a rotation matrix
	void scale(GzReal x, GzReal y, GzReal z);					//Multiply transMatrix by a scaling matrix
	void multMatrix(GzMatrix mat);								//Multiply transMatrix by the matrix mat
	//End of Transformations--------------------------------------------------

	//Projections-------------------------------------------------------------
	void perspective(GzReal fovy, GzReal aspect, 
						GzReal zNear, GzReal zFar);			//Set up a perspective projection matrix
	void orthographic(GzReal left, GzReal right, GzReal bottom, GzReal top, 
						GzReal nearVal, GzReal farVal);		//Set up a orthographic projection matrix
	//End of Projections------------------------------------------------------
	
private:
	GzMatrix transMatrix;					//The transformation matrix
	GzMatrix prjMatrix;							//The projection matrix
	GzMatrix viewMatrix;
	GzReal xViewport, yViewport;			//The center of the viewport
	GzReal wViewport, hViewport;			//Size of the viewport
//============================================================================
//End of Declarations in Assignment #3
//============================================================================



};





#endif
