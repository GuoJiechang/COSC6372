#include <fstream>
#include <iostream>
#include "Gz.h"


using namespace std;

Gz gz;
GzImage texture;

void draw(const char* fileName) {
	ifstream fi("./Tris.txt");
	int nTri;
	fi>>nTri;	//Number of triangles

	gz.clear(GZ_COLOR_BUFFER | GZ_DEPTH_BUFFER);	//Clear frame buffer with background color
													//Clear depth buffer with default depth

	GzVertex v;
	GzTexCoord tc;
	gz.begin(GZ_TRIANGLES);		//Draw triangles
	for (int i=0; i<nTri; i++)
		for (int j=0; j<3; j++) {
			fi>>v[X]>>v[Y]>>v[Z];
			gz.addVertex(v);
			fi>>tc[U]>>tc[V];
			gz.addTexCoord(tc);
		}
	gz.end();

	fi.close();

	gz.toImage().save(fileName);
}

int main() {
	texture.load("./Texture.bmp");
	//cout<<texture.sizeW()<<" " << texture.sizeH()<<endl;
	gz.initFrameSize(320, 240);
	
	gz.clearColor(GzColor(0, 0, 0));	//Background color: Black
	gz.enable(GZ_DEPTH_TEST);			//Use depth test
	gz.clearDepth(-100);				//Default depth: -100
	gz.disable(GZ_LIGHTING);			//No lighting
	gz.enable(GZ_TEXTURE);	
	gz.texture(texture);				//Use the texture

	
	gz.lookAt(	10, 10, -10,	//Eye position 
				0, 0, 0,	//Look at
				0, 1, 0);	//Up vector
	gz.orthographic(-8, 8,	//Left and Right
					-6, 6,	//Bottom and Top
					1, 10);	//zNear and zFar
	gz.translate(0, -1, 0);
	draw("TeaPot1.bmp");
	
	gz.lookAt(	15, 15, -15,	//Eye position 
				0, 0, 0,	//Look at
				0, 1, 0);	//Up vector
	gz.perspective(	40,				//Fovy
					(GzReal)4/3,	//Aspect
					1, 10);			//zNear and zFar
	gz.rotate(-20, 0, 1, 0);
	gz.translate(1, 5, 0);

	draw("TeaPot2.bmp");		
	return 0;
}
