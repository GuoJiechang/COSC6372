#ifndef __GZ_FRAME_BUFFER_H_
#define __GZ_FRAME_BUFFER_H_

#include "GzCommon.h"
#include "GzImage.h"
#include <vector>
#include <cmath>
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
	void drawTriangle(vector<GzVertex>& v, vector<GzColor>& c, GzFunctional status);
	GzReal clearDepth;

private:
	//Put any variables and private functions for your implementation here
	GzImage image;
	vector<vector<GzReal> > depthBuffer;
	GzColor clearColor;

	void realInterpolate(GzReal key1, GzReal val1, GzReal key2, GzReal val2, GzReal key, GzReal& val);
	void colorInterpolate(GzReal key1, GzColor& val1, GzReal key2, GzColor& val2, GzReal key, GzColor& val);
	void drawRasLine(GzInt y, GzReal xMin, GzReal zMin, GzColor& cMin, GzReal xMax, GzReal zMax, GzColor& cMax, GzFunctional status);
};
//----------------------------------------------------------------------------

#endif
