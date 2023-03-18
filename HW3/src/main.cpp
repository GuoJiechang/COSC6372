//This file was updated on September 26, 2010 due to the different convention of Z-buffer.
//Also, the correspoding results file were updated (TeaPot1.bmp -> TeaPot6.bmp).
//Please check the file HW3.pdf again for the description of the update and some hints.
//NOTE: There is another update in function 
//			void Gz::multMatrix(GzMatrix mat)
// of the file Gz.cpp.

#include <fstream>
#include "Gz.h"

using namespace std;

Gz gz;

void draw(const char* fileName) {
	ifstream fi("Tris.txt");
	int nTri;
	fi>>nTri;	//Number of triangles

	gz.clear(GZ_COLOR_BUFFER | GZ_DEPTH_BUFFER);	//Clear frame buffer with background color
													//Clear depth buffer with default depth

	GzVertex v;
	GzColor c;
	gz.begin(GZ_TRIANGLES);		//Draw triangles
	for (int i=0; i<nTri; i++)
		for (int j=0; j<3; j++) {
			fi>>v[X]>>v[Y]>>v[Z];
			gz.addVertex(v);
			fi>>c[R]>>c[G]>>c[B]>>c[A];
			gz.addColor(c);
		}
	gz.end();

	fi.close();

	gz.toImage().save(fileName);
}

int main() {
	gz.initFrameSize(320, 240);
	
	gz.clearColor(GzColor(0, 0, 0));	//Background color: Black
	gz.enable(GZ_DEPTH_TEST);			//Use depth test
	gz.clearDepth(-100);				//Default depth: -100

	gz.lookAt(	0, 0, 10,	//Eye position 
				0, 0, 0,	//Look at
				0, 1, 0);	//Up vector
	gz.orthographic(-8, 8,	//Left and Right
					-6, 6,	//Bottom and Top
					1, 10);	//zNear and zFar
	draw("TeaPot1.bmp");

	gz.lookAt(	0, 10, 0,	//Eye position 
				0, 0, 0,	//Look at
				1, 0, 2);	//Up vector
	gz.orthographic(-4, 4,	//Left and Right
					-3, 3,	//Bottom and Top
					1, 10);	//zNear and zFar
	draw("TeaPot2.bmp");

	gz.lookAt(	10, 10, 10,	//Eye position 
				0, 0, 0,	//Look at
				0, 1, 0);	//Up vector
	gz.orthographic(-4, 4,	//Left and Right
					-3, 3,	//Bottom and Top
					1, 10);	//zNear and zFar
	gz.translate(0, -1, 0);
	draw("TeaPot3.bmp");



	gz.lookAt(	0, 0, 20,	//Eye position 
				0, 0, 0,	//Look at
				1, 1, 0);	//Up vector
	gz.perspective(	20,				//Fovy
					(GzReal)4/3,	//Aspect
					1, 10);			//zNear and zFar
	gz.translate(0, -1, 0);
	draw("TeaPot4.bmp");

	gz.lookAt(	10, 10, 10,	//Eye position 
				0, 0, 0,	//Look at
				0, 1, 0);	//Up vector
	gz.perspective(	30,				//Fovy
					(GzReal)4/3,	//Aspect
					1, 10);			//zNear and zFar
	gz.translate(0, -1, 0);
	draw("TeaPot5.bmp");

	gz.lookAt(	3, 3, 3,	//Eye position 
				0, 0, 0,	//Look at
				0, 1, 0);	//Up vector
	gz.perspective(	70,				//Fovy
					(GzReal)4/3,	//Aspect
					1, 10);			//zNear and zFar
	gz.rotate(45, 1, 0, 0);
	gz.translate(0, -1, 0);
	draw("TeaPot6.bmp");
		
	return 0;
}
