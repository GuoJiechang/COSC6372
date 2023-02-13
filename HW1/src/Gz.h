#ifndef __GZ_H_
#define __GZ_H_

#include "GzCommon.h"
#include "GzImage.h"
#include "GzFrameBuffer.h"
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



};

#endif
