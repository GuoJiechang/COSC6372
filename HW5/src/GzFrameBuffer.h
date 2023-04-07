#ifndef __GZ_FRAME_BUFFER_H_
#define __GZ_FRAME_BUFFER_H_

#include "GzCommon.h"
#include "GzImage.h"
#include "GzVector.h"
#include "GzMatrix.h"
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


	void shadeModel(const GzInt model);						//Set the current shade model (curShadeModel)
	void addLight(const GzVector& v, const GzColor& c);		//Add a light source at position p with color c
	void material(GzReal _kA, GzReal _kD, GzReal _kS, GzReal _s);		//Specify the meterial of the object, includes:
																			//	_kA: The ambient coefficients
																			//	_kD: The diffuse coefficients
																			//	_kS: The specular coefficients
																			//	_s: The spec power
	void loadLightTrans(GzMatrix& mat);

	void drawPointWLight(const GzVertex& v, const GzColor& c, const GzVector& n, GzFunctional status);
	void drawTriangleWLight(vector<GzVertex>& v, vector<GzColor>& c, vector<GzVector>& n, GzFunctional status);
	void drawTriangleWTexture(vector<GzVertex>& v, vector<GzVertex>& vView, vector<GzTexCoord>& tc,GzFunctional status);

	void texture(const GzImage& t);

	//set true to do perspective correction for texture mapping
	void IsPerspective(bool b);
private:
	bool isPerspective = false;
	//Shading 
	GzInt curShadeModel;
	vector<GzVector> lightDir;
	vector<GzColor> lightColor;
	GzReal kA, kD, kS, s;
	GzMatrix lightTrans;

	//Put any variables and private functions for your implementation here
	GzImage image;
	vector<vector<GzReal> > depthBuffer;
	GzColor clearColor;
	GzReal clearDepth;

	void realInterpolate(GzReal key1, GzReal val1, GzReal key2, GzReal val2, GzReal key, GzReal& val);
	void colorInterpolate(GzReal key1, GzColor& val1, GzReal key2, GzColor& val2, GzReal key, GzColor& val);
	void normalInterpolate(GzReal key1, GzVector& val1, GzReal key2, GzVector& val2, GzReal key, GzVector& val);
	void texcoordInterpolate(GzReal key1, GzTexCoord& val1, GzReal key2, GzTexCoord& val2, GzReal key, GzReal z1,GzReal z2, GzTexCoord& val);

	void drawRasLine(GzInt y, GzReal xMin, GzReal zMin, GzColor& cMin, GzReal xMax, GzReal zMax, GzColor& cMax, GzFunctional status);
	void drawRasLineWLight(GzInt y, GzReal xMin, GzReal zMin, GzColor& cMin, GzVector& nMin, GzReal xMax, GzReal zMax, GzColor& cMax, GzVector& nMax, GzFunctional status);
	void drawRasLineWTexture(GzInt y, GzReal xMin, GzReal zMin, GzReal zViewMin, GzTexCoord& tcMin, GzReal xMax, GzReal zMax, GzReal zViewMax, GzTexCoord& tcMax, GzFunctional status);

	GzColor colorWLight(GzColor c, GzVector n);

	GzImage texture1; //the texture
	//Design function
	GzColor sampleTexture(GzTexCoord texCoord);

};
//----------------------------------------------------------------------------

#endif
