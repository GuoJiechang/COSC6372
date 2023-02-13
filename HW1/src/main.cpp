#include <fstream>
#include "Gz.h"

using namespace std;

Gz gz;

void fillRec(int x1, int y1, int x2, int y2, double r, double g, double b, int z) {
	gz.begin(GZ_POINTS);	//Delimit the vertices of point
	for (int x=x1; x<=x2; x++)
		for (int y=y1; y<=y2; y++) {
			gz.addVertex(GzVertex(x, y, z));	//Specify a vertex
			gz.addColor(GzColor(r, g, b));		//Specify a color
		}
	gz.end();				//End of limit
}

void draw() {
	ifstream fi("rects.txt");
	int n;
	fi>>n;
	for (int i=0; i<n; i++) {
		int x1, y1, x2, y2, z;
		double r, g, b;
		fi>>x1>>y1>>x2>>y2>>r>>g>>b>>z;
		fillRec(x1, y1, x2, y2, r, g, b, z);
	}
	fi.close();
}

int main() {
	gz.initFrameSize(640, 480);
	
	gz.clearColor(GzColor(0, 0, 0));	//Background color: Black
	gz.clear(GZ_COLOR_BUFFER);			//Clear frame buffer with background color
	gz.disable(GZ_DEPTH_TEST);			//No depth test
	draw();
	gz.toImage().save("NoDepthBuf.bmp");

	gz.clearColor(GzColor(1, 1, 1));	//Background color: White
	gz.enable(GZ_DEPTH_TEST);			//Use depth test
	gz.clearDepth(-20);					//Default depth: -20
	gz.clear(GZ_COLOR_BUFFER | GZ_DEPTH_BUFFER);	//Clear frame buffer with background color
													//Clear depth buffer with default depth
	draw();
	gz.toImage().save("WithDepthBuf.bmp");

	return 0;
}
