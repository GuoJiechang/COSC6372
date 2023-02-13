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
};
//----------------------------------------------------------------------------

#endif
