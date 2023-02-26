#include <fstream>
#include "Gz.h"

using namespace std;

Gz gz;

int main() {
	gz.initFrameSize(320, 240);
	
	gz.clearColor(GzColor(0, 0, 0));	//Background color: Black
	gz.enable(GZ_DEPTH_TEST);			//Use depth test
	gz.clearDepth(-100);				//Default depth: -100

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

	gz.toImage().save("TeaPot.bmp");

	return 0;
}
