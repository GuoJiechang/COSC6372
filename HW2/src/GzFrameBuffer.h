#ifndef __GZ_FRAME_BUFFER_H_
#define __GZ_FRAME_BUFFER_H_

#include "GzCommon.h"
#include "GzImage.h"
#include <vector>
using namespace std;

//Frame buffer with Z-buffer -------------------------------------------------
class GzFrameBuffer {
public:
	//The common interface
	void initFrameSize(GzInt width, GzInt height);
	GzImage toImage();

	void clear(GzFunctional buffer);
	void setClearColor(const GzColor& color);
	void setClearDepth(GzReal depth);

	void drawPoint(const GzVertex& v, const GzColor& c, GzFunctional status);

	void drawTriangle(const GzTriangle& tri, GzFunctional status);

	void drawLine(const GzVertex& v1, const GzVertex& v2, GzFunctional status);

private:
	//Draw pixel
	void drawPixel(const GzInt x, const GzInt y, const GzReal z, const GzColor& c);
	//Draw triangle use bounding box method
	void drawTriangleUseBbox(const GzTriangle& tri);
	Gz2DBoundingBox calculateBoundingBox(const GzTriangle& tri);

	//Draw triangle use scan line method
	void drawTriangleUseScanline(const GzTriangle& tri);
	void fillBottomFlatTri(const GzTriangle& tri);
	void fillTopFlatTri(const GzTriangle& tri);

	
	//The function to check if a pixel is inside the triangle
	bool insideTriangle(const GzTriangle& tri, const GzReal x, const GzReal y);

	void barycentricInterpolation(const GzReal x, const GzReal y, const GzTriangle& tri, GzReal &w1, GzReal &w2, GzReal &w3);

private:
	//pixel values for the final image
	vector<vector<GzColor> > colorBuffer;
	//depth value to do depth test
	vector<vector<GzReal> > depthBuffer;
	//buffer size, image size
	GzInt width_,height_;
	//preset clear color
	GzColor clearColor_;
	//preset depth
	GzReal clearDepth_;

	GzFunctional status_;
};
//----------------------------------------------------------------------------

#endif
