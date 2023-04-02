#include "GzFrameBuffer.h"

//Put your implementation here------------------------------------------------
#include <climits>
void GzFrameBuffer::initFrameSize(GzInt width, GzInt height) {
	image.resize(width, height);
	depthBuffer=vector<vector<GzReal> >(width, vector<GzReal>(height, clearDepth));
}

GzImage GzFrameBuffer::toImage() {
	return image;
}

void GzFrameBuffer::clear(GzFunctional buffer) {
	if (buffer&GZ_COLOR_BUFFER) image.clear(clearColor);
	if (buffer&GZ_DEPTH_BUFFER)
		for (GzInt x=0; x!=depthBuffer.size(); x++) 
			fill(depthBuffer[x].begin(), depthBuffer[x].end(), clearDepth);
}

void GzFrameBuffer::setClearColor(const GzColor& color) {
	clearColor=color;
}

void GzFrameBuffer::setClearDepth(GzReal depth) {
	clearDepth=depth;
}

void GzFrameBuffer::drawPoint(const GzVertex& v, const GzColor& c, GzFunctional status) {
	GzInt x=(GzInt)v[X];
	GzInt y=image.sizeH()-(GzInt)v[Y]-1;
	if ((x<0)||(y<0)||(x>=image.sizeW())||(y>=image.sizeH())) return;
	if (status&GZ_DEPTH_TEST) {
		if (v[Z]>=depthBuffer[x][y]) {
			image.set(x, y, c);
			depthBuffer[x][y]=v[Z];
		}
	} else {
		image.set(x, y, c);
		depthBuffer[x][y]=v[Z];
	}
}

void GzFrameBuffer::drawTriangle(vector<GzVertex>& v, vector<GzColor>& c, GzFunctional status) {
	GzInt yMin, yMax;
	GzReal xMin, xMax, zMin, zMax;
	GzColor cMin, cMax;

	v.push_back(v[0]);
	c.push_back(c[0]);

	yMin = INT_MAX;
	yMax = -INT_MAX;

	for (GzInt i = 0; i < 3; i++) {
		yMin = min((GzInt)floor(v[i][Y]), yMin);
		yMax = max((GzInt)floor(v[i][Y] - 1e-3), yMax);
	}

	for (GzInt y = yMin; y <= yMax; y++) {
		xMin = INT_MAX;
		xMax = -INT_MAX;
		for (GzInt i = 0; i < 3; i++) {
			if ((GzInt)floor(v[i][Y]) == y) {
				if (v[i][X] < xMin) {
					xMin = v[i][X];
					zMin = v[i][Z];
					cMin = c[i];
				}
				if (v[i][X] > xMax) {
					xMax = v[i][X];
					zMax = v[i][Z];
					cMax = c[i];
				}
			}
			if ((y - v[i][Y]) * (y - v[i + 1][Y]) < 0) {
				GzReal x;
				realInterpolate(v[i][Y], v[i][X], v[i + 1][Y], v[i + 1][X], y, x);
				if (x < xMin) {
					xMin = x;
					realInterpolate(v[i][Y], v[i][Z], v[i + 1][Y], v[i + 1][Z], y, zMin);
					colorInterpolate(v[i][Y], c[i], v[i + 1][Y], c[i + 1], y, cMin);
				}
				if (x > xMax) {
					xMax = x;
					realInterpolate(v[i][Y], v[i][Z], v[i + 1][Y], v[i + 1][Z], y, zMax);
					colorInterpolate(v[i][Y], c[i], v[i + 1][Y], c[i + 1], y, cMax);
				}
			}
		}
		drawRasLine(y, xMin, zMin, cMin, xMax - 1e-3, zMax, cMax, status);
	}
}

void GzFrameBuffer::drawRasLine(GzInt y, GzReal xMin, GzReal zMin, GzColor& cMin, GzReal xMax, GzReal zMax, GzColor& cMax, GzFunctional status) {
	if ((y < 0) || (y >= image.sizeH())) return;
	if ((GzInt)floor(xMin) == (GzInt)floor(xMax)) {
		if (zMin > zMax) drawPoint(GzVertex(floor(xMin), y, zMin), cMin, status);
		else drawPoint(GzVertex(floor(xMin), y, zMax), cMax, status);
	}
	else {
		GzReal z;
		GzColor c;
		y = image.sizeH() - y - 1;
		int w = image.sizeW();
		if (status & GZ_DEPTH_TEST) {
			for (int x = max(0, (GzInt)floor(xMin)); x <= min(w - 1, (GzInt)floor(xMax)); x++) {
				realInterpolate(xMin, zMin, xMax, zMax, x, z);
				if (z >= depthBuffer[x][y]) {
					colorInterpolate(xMin, cMin, xMax, cMax, x, c);
					image.set(x, y, c);
					depthBuffer[x][y] = z;
				}
			}
		}
		else {
			for (int x = max(0, (GzInt)floor(xMin)); x <= min(w - 1, (GzInt)floor(xMax)); x++) {
				realInterpolate(xMin, zMin, xMax, zMax, x, z);
				colorInterpolate(xMin, cMin, xMax, cMax, x, c);
				image.set(x, y, c);
				depthBuffer[x][y] = z;
			}
		}
	}
}

void GzFrameBuffer::drawTriangle(vector<GzVertex> &v, vector<GzVector>& p, vector<GzVector> &n, vector<GzColor> &c, GzFunctional status)
{
	//Do light color shading on vertex
	if(curShadeModel == GZ_GOURAUD && n.size() == v.size() && (status&GZ_LIGHTING))
	{
		//for each vertex, calculate the light color according to the normal
		for(int i = 0; i < n.size(); i++)
		{
			GzVector lightColor = calAllLight(p[i], n[i]);
			
			c[i][0] = c[i][0] * lightColor[0];
			c[i][1] = c[i][1] * lightColor[1];
			c[i][2] = c[i][2] * lightColor[2];
		}

		drawTriangle(v,c,status);
	}//Do light color shading on fragment
	else if(curShadeModel == GZ_PHONG && n.size() == v.size() && (status&GZ_LIGHTING))
	{
		GzInt yMin, yMax;
		GzReal xMin, xMax, zMin, zMax;
		GzColor cMin, cMax;
		GzVector nMin, nMax;
		GzVector pMin, pMax;

		v.push_back(v[0]);
		c.push_back(c[0]);
		n.push_back(n[0]);
		p.push_back(p[0]);

		yMin=INT_MAX;
		yMax=-INT_MAX;

		for (GzInt i=0; i<3; i++) {
			yMin=min((GzInt)floor(v[i][Y]), yMin);
			yMax=max((GzInt)floor(v[i][Y]-1e-3), yMax);
		}
			
		for (GzInt y=yMin; y<=yMax; y++) {
			xMin=INT_MAX;
			xMax=-INT_MAX;
			for (GzInt i=0; i<3; i++) {
				if ((GzInt)floor(v[i][Y])==y) {
					if (v[i][X]<xMin) {
						xMin=v[i][X];
						zMin=v[i][Z];
						nMin=n[i];
						cMin=c[i];
						pMin=p[i];
					}
					if (v[i][X]>xMax) {
						xMax=v[i][X];
						zMax=v[i][Z];
						nMax=n[i];
						cMax=c[i];
						pMax=p[i];
					}
				}
				if ((y-v[i][Y])*(y-v[i+1][Y])<0) {
					GzReal x;
					realInterpolate(v[i][Y], v[i][X], v[i+1][Y], v[i+1][X], y, x);
					if (x<xMin) {
						xMin=x;
						realInterpolate(v[i][Y], v[i][Z], v[i+1][Y], v[i+1][Z], y, zMin);
						colorInterpolate(v[i][Y], c[i], v[i+1][Y], c[i+1], y, cMin);
						vectorInterpolate(v[i][Y], n[i], v[i+1][Y], n[i+1], y, nMin);
						vectorInterpolate(v[i][Y], p[i], v[i+1][Y], p[i+1], y, pMin);
					}
					if (x>xMax) {
						xMax=x;
						realInterpolate(v[i][Y], v[i][Z], v[i+1][Y], v[i+1][Z], y, zMax);
						colorInterpolate(v[i][Y], c[i], v[i+1][Y], c[i+1], y, cMax);
						vectorInterpolate(v[i][Y], n[i], v[i+1][Y], n[i+1], y, nMax);
						vectorInterpolate(v[i][Y], p[i], v[i+1][Y], p[i+1], y, pMax);
					}
				}
			}
			drawRasLine(y, xMin, zMin, nMin, pMin, cMin, xMax-1e-3, zMax, nMax, pMax, cMax, status);
		}

	}
	
}

void GzFrameBuffer::drawRasLine(GzInt y, GzReal xMin, GzReal zMin, GzVector& nMin, GzVector& pMin, GzColor& cMin, GzReal xMax, GzReal zMax, GzVector& nMax, GzVector& pMax, GzColor& cMax, GzFunctional status) {
	if ((y<0)||(y>=image.sizeH())) return;
	if ((GzInt)floor(xMin)==(GzInt)floor(xMax)) {
		GzVertex vert;
		GzColor col;
		GzVector norm;
		GzVector fragPos;

		if (zMin>zMax)
		{ 
			vert = GzVertex(floor(xMin), y, zMin);
			col = cMin;
			norm = nMin;
			fragPos = pMin;
		}
		else 
		{
			vert = GzVertex(floor(xMin), y, zMax);
			col = cMax;
			norm = nMax;
			fragPos = pMax;
		}

		GzVector lightColor = calAllLight(fragPos,norm);
		
		col[0] = col[0] * lightColor[0];
		col[1] = col[1] * lightColor[1];
		col[2] = col[2] * lightColor[2];

		drawPoint(vert, col, status);
	} 
	else 
	{
		GzReal z;
		GzColor c;
		y=image.sizeH()-y-1;
		int w=image.sizeW();
		if (status&GZ_DEPTH_TEST) {
			for (int x=max(0, (GzInt)floor(xMin)); x<=min(w-1, (GzInt)floor(xMax)); x++) {
				realInterpolate(xMin, zMin, xMax, zMax, x, z);
				if (z>=depthBuffer[x][y]) {
					colorInterpolate(xMin, cMin, xMax, cMax, x, c);
					GzVector n;
					vectorInterpolate(xMin, nMin, xMax, nMax, x, n);
					GzVector fragPos;
					vectorInterpolate(xMin, pMin, xMax, pMax, x, fragPos);

					GzVector lightColor = calAllLight(fragPos,n);
					
					c[0] = c[0] * lightColor[0];
					c[1] = c[1] * lightColor[1];
					c[2] = c[2] * lightColor[2];
					image.set(x, y, c);
					depthBuffer[x][y]=z;
				}
			}
		} else {
			for (int x=max(0, (GzInt)floor(xMin)); x<=min(w-1, (GzInt)floor(xMax)); x++) {
				realInterpolate(xMin, zMin, xMax, zMax, x, z);
				colorInterpolate(xMin, cMin, xMax, cMax, x, c);
				GzVector n;
				vectorInterpolate(xMin, nMin, xMax, nMax, x, n);
				GzVector fragPos;
				vectorInterpolate(xMin, pMin, xMax, pMax, x, fragPos);

				GzVector lightColor = calAllLight(fragPos,n);
				
				c[0] = c[0] * lightColor[0];
				c[1] = c[1] * lightColor[1];
				c[2] = c[2] * lightColor[2];
				image.set(x, y, c);
				depthBuffer[x][y]=z;
			}
		}
	}
}

void GzFrameBuffer::realInterpolate(GzReal key1, GzReal val1, GzReal key2, GzReal val2, GzReal key, GzReal& val) {
	val=val1+(val2-val1)*(key-key1)/(key2-key1);
}

void GzFrameBuffer::colorInterpolate(GzReal key1, GzColor& val1, GzReal key2, GzColor& val2, GzReal key, GzColor& val) {
	GzReal k=(key-key1)/(key2-key1);
	for (GzInt i=0; i<4; i++) val[i]=val1[i]+(val2[i]-val1[i])*k;
}

void GzFrameBuffer::vectorInterpolate(GzReal key1, GzVector &val1, GzReal key2, GzVector &val2, GzReal key, GzVector &val)
{
	GzReal k=(key-key1)/(key2-key1);
	for (GzInt i=0; i<3; i++) val[i]=val1[i]+(val2[i]-val1[i])*k;
}

void GzFrameBuffer::shadeModel(const GzInt model) {
	curShadeModel=model;
}

void GzFrameBuffer::material(GzReal _kA, GzReal _kD, GzReal _kS, GzReal _s) {
	kA=_kA;
	kD=_kD;
	kS=_kS;
	s=_s;
}

void GzFrameBuffer::loadLightTrans(GzMatrix mat)
{
	//clear previous transform on light directions
	lights.clear();
	lights = orignLights;

	GzMatrix transMat3 = Zeros(3);
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			transMat3[i][j] = mat[i][j];
		}
	}
	//transform light direction to view space
	for(int i = 0; i < lights.size(); i++)
	{
		GzVector dir = lights[i].first;
		dir.normalize();
		GzMatrix res = Zeros(3);
		res = transMat3.inverse3x3();
		res = res.transpose();
		GzMatrix dirMat;
		dirMat.resize(3,1);
		dirMat[0][0] = dir[0];
		dirMat[1][0] = dir[1];
		dirMat[2][0] = dir[2];
		GzMatrix resMat = res * dirMat;

		GzVector nRes;
		nRes[0] = resMat[0][0];
		nRes[1] = resMat[1][0];
		nRes[2] = resMat[2][0];	

		lights[i].first = nRes;
	}
}

void GzFrameBuffer::addLight(const GzVector& v, const GzColor& c) {
	orignLights.push_back(pair<GzVector,GzColor>(v,c));
}

GzVector GzFrameBuffer::calDirLight(GzVector p, GzVector n, GzVector dir, GzVector ltc)
{
	//ambient
	GzVector ambient;
	ambient = kA*ltc;

	GzVector zeroV;
	// diffuse
	GzVector norm = n;
	norm.normalize();
	GzVector lightDir = zeroV - dir;
	lightDir.normalize();
	float diff = max(dot(norm, lightDir), 0.0);
	GzVector diffuse = kD * diff * ltc;

	// specular
	GzVector viewDir;//we calculate the light in view space, so the viewPos-fragPos is equal to -fragPos
	viewDir = zeroV - p;
	viewDir.normalize();

	GzVector reflectDir = reflect(zeroV-lightDir, norm); 
	reflectDir.normalize();

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), s);
	GzVector specular = kS * spec * ltc;  

	return ambient + diffuse + specular;
}

GzVector GzFrameBuffer::calAllLight(GzVector p, GzVector n)
{
	GzVector lightColor;

	//for each light source
	for(int j = 0; j < lights.size(); j++)
	{
		GzVector ltc;
		ltc[0] = lights[j].second[0];
		ltc[1] = lights[j].second[1];
		ltc[2] = lights[j].second[2];
	
		lightColor = lightColor + calDirLight(p,n,lights[j].first,ltc);
	}
    return lightColor;
}

GzReal GzFrameBuffer::dot(GzVector x, GzVector y)
{
	return x[0]*y[0] + x[1]*y[1] + x[2]*y[2];
}

GzVector GzFrameBuffer::reflect(GzVector I, GzVector N)
{
	return I - 2.0 * dot(N,I)*N;
}
