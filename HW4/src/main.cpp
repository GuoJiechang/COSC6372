#include <fstream>
#include "Gz.h"

using namespace std;

Gz gz;

void draw(const char* fileName) {
	ifstream fi("./Tris.txt");
	int nTri;
	fi>>nTri;	//Number of triangles

	gz.clear(GZ_COLOR_BUFFER | GZ_DEPTH_BUFFER);	//Clear frame buffer with background color
													//Clear depth buffer with default depth

	GzVertex v;
	GzVector n;
	GzColor c;
	gz.begin(GZ_TRIANGLES);		//Draw triangles
	for (int i=0; i<nTri; i++)
		for (int j=0; j<3; j++) {
			fi>>v[X]>>v[Y]>>v[Z];
			gz.addVertex(v);
			fi>>n[X]>>n[Y]>>n[Z];
			gz.addNormal(n);
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
	gz.enable(GZ_LIGHTING);

	gz.addLight(GzVector(0, -1, 0), GzColor(1, 0.1, 0.1));	//Red light direction (0, -1, 0)
	gz.addLight(GzVector(-1, 0, 0), GzColor(0.1, 1, 0.1));	//Green light direction (-1, 0, 0)
	gz.addLight(GzVector(0, 0, -1), GzColor(0.1, 0.1, 1));	//Blue light direction (0, 0, -1)
	gz.material(0.1, 0.3, 0.6, 3);		//Ambient, diffuse, specular, spec power

	
	
	gz.shadeModel(GZ_GOURAUD);
	
	gz.lookAt(	10, 10, 10,	//Eye position 
				0, 0, 0,	//Look at
				0, 1, 0);	//Up vector
	gz.orthographic(-2.4, 2.4,	//Left and Right
					-1.8, 1.8,	//Bottom and Top
					1, 10);	//zNear and zFar
	gz.translate(0, -1, 0);
	draw("GouraudTeaPot1.bmp");
	
	gz.lookAt(	3, 3, 3,	//Eye position 
				0, 0, 0,	//Look at
				0, 1, 0);	//Up vector
	gz.perspective(	50,				//Fovy
					(GzReal)4/3,	//Aspect
					1, 10);			//zNear and zFar
	gz.rotate(45, 1, 0, 0);
	gz.translate(0, -1, 0);

	draw("GouraudTeaPot2.bmp");

	
	
	gz.shadeModel(GZ_PHONG);

	gz.lookAt(	10, 10, 10,	//Eye position 
				0, 0, 0,	//Look at
				0, 1, 0);	//Up vector
	gz.orthographic(-2.4, 2.4,	//Left and Right
					-1.8, 1.8,	//Bottom and Top
					1, 10);	//zNear and zFar
	gz.translate(0, -1, 0);
	draw("PhongTeaPot1.bmp");
	
	gz.lookAt(	3, 3, 3,	//Eye position 
				0, 0, 0,	//Look at
				0, 1, 0);	//Up vector
	gz.perspective(	50,				//Fovy
					(GzReal)4/3,	//Aspect
					1, 10);			//zNear and zFar
	gz.rotate(45, 1, 0, 0);
	gz.translate(0, -1, 0);

	draw("PhongTeaPot2.bmp");
		
	return 0;
}
