#include "GzImage.h"
#include "CBitmap.h"

GzImage::GzImage() {
	h=w=0;
}

GzImage::GzImage(GzInt width, GzInt height) {
	resize(width, height);
}

GzImage::GzImage(const char* fileName) {
	load(fileName);
}

GzBool GzImage::load(const char* fileName) {
	CBitmap bmp;
	if (!bmp.Load((char*)fileName)) return false;
	RGBA* data=(RGBA*)(bmp.GetBits());

	resize(bmp.GetWidth(), bmp.GetHeight());
	GzReal depth=255;
	for (GzInt x=0; x<w; x++) 
		for (GzInt y=0; y<h; y++) {
			GzInt idx=(h-y-1)*w+x;
			pixel[x][y][R]=data[idx].Red/depth;
			pixel[x][y][G]=data[idx].Green/depth;
			pixel[x][y][B]=data[idx].Blue/depth;
			pixel[x][y][A]=data[idx].Alpha/depth;
		}
	
	return true;
}

GzBool GzImage::save(const char* fileName) {
	unsigned int mask=255;
	
	unsigned char* data=new unsigned char[h*w*4];
	for (GzInt x=0; x<w; x++) 
		for (GzInt y=0; y<h; y++) {
			GzInt idx=(h-y-1)*w+x;
			data[(idx)*4]=max(min(int(pixel[x][y][R]*mask), 255), 0);
			data[(idx)*4+1]=max(min(int(pixel[x][y][G]*mask), 255), 0);
			data[(idx)*4+2]=max(min(int(pixel[x][y][B]*mask), 255), 0);
			data[(idx)*4+3]=max(min(int(pixel[x][y][A]*mask), 255), 0);
		}
	
	CBitmap bmp;
	bmp.SetBits((void*)data, w, h, mask, mask<<8, mask<<16, mask<<24);
	bmp.Save((char*)fileName, 24);
	
	delete[] data;
	return true;
}
	
GzInt GzImage::sizeW() {
	return w;
}

GzInt GzImage::sizeH() {
	return h;
}

GzBool GzImage::resize(GzInt width, GzInt height) {
	if ((width<0)||(height<0)) return false;
	h=height;
	w=width;
	pixel.resize(w);
	clear(GzColor(0, 0, 0));
	return true;
}
	
GzColor GzImage::get(GzInt x, GzInt y) {
	if ((x<0)||(y<0)||(x>=w)||(y>=h)) return GzColor();
	return pixel[x][y];
}

GzBool GzImage::set(GzInt x, GzInt y, const GzColor& c) {
	if ((x<0)||(y<0)||(x>=w)||(y>=h)) return false;
	pixel[x][y]=c;
	return true;
}

void GzImage::clear(const GzColor& c) {
	fill(pixel.begin(), pixel.end(), vector<GzColor>(h, c));
}
