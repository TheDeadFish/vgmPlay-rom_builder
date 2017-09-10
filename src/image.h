#ifndef _IMAGE_H_
#define _IMAGE_H_

struct ImageInfo
{
	xstr name;
	ImageLib::ImageObj img;
	int bkgndColor;
	int ypos; int ysize;
	
	
	ImageInfo();
	int load(cch*);
	

	
	int getSize();
	
	byte* build(ImageLib::Image& img, byte* out);
	
	void write(FILE* fp);
	
	
	
	int fileOffset;

	
	int width();
	
	struct calcRect2_t { RECT rc; 
		int offsetX, offsetY;
		int width, height; };
	calcRect2_t calcRect2();
	RECT calcRect();
};

#endif
