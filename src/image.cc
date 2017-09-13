#include "stdafx.h"
#include "image.h"
#include "mdcode.h"

int gene_medianCut(ImageLib::Image& img, 
	Color* palette, int reqColors, int splitMode)
{
	int nColors = medianCut(img, palette, 
		reqColors, splitMode, ImageLib::btPosterizeLine, 3);
	ImageLib::unPosterizeLine(PB(palette), PB(palette), 3, nColors);
	return nColors;
}

float borderColorVal(ImageLib::ImageObj& img, int x, int y)
{
	int halfX = img.width>>1;
	int halfY = img.height>>1;
	float dx = float(abs(x-halfX))/halfX;
	float dy = float(abs(y-halfY))/halfY;
	float tmp = max(dx, dy);
	return tmp*tmp*tmp;
}

int borderColor(ImageLib::ImageObj& img)
{
	// get palette scores
	double* scores = xCalloc(256);
	SCOPE_EXIT(free(scores));
	for(int x = 0; x < img.width; x++)
	for(int y = 0; y < img.height; y++) {
		scores[*img.get8(x, y)] += 
		borderColorVal(img, x, y); }

	// select best palette entry
	double bestVal = 0; int bestPal = 0;
	for(int i = 0; i < img.palSize; i++) {
		if(bestVal < scores[i]) { bestVal = 
			scores[i]; bestPal = i; }}
	return bestPal;
}

int gene_conv16(ImageLib::ImageObj& img)
{
	Color pal[16];
	int nPal = gene_medianCut(img, 
		pal, 16, SPLITMODE_MODE);
	ImageLib::nearest_conv(img, pal, nPal);	
	return borderColor(img);
}


ImageInfo::ImageInfo() {}

int ImageInfo::load(cch* nm)
{
	IFRET(img.Load(nm));
	bkgndColor = gene_conv16(img);
	ypos = 0; 
	
	
	
	
	
	// scale image
	ysize = img.height;
	if((img.width > 320)&&(ysize > 224)) { ysize = 224;
		if(width() < 320) { ysize = lrintf(float(
		320 * img.height) / img.width); }}
	
	name.init(getName(nm).xdup());
	return 0;
}

int ImageInfo::width() {
	return lrintf(float(img.width
		* ysize) / img.height); }

RECT ImageInfo::calcRect()
{
	int width = this->width(); 
	return ImageLib::Rect::RectXYWH((320-width)/2,
		((224-ysize)/2)+ypos, width, ysize);
}

ImageInfo::calcRect2_t ImageInfo::calcRect2()
{
	calcRect2_t r; r.rc = calcRect();
	r.offsetX = max(r.rc.left,0);
	r.width = ALIGN(min(r.rc.right,320)-r.offsetX,7);
	r.offsetY = max(r.rc.top, 0);
	r.height = ALIGN(min(r.rc.bottom,224)-r.offsetY,7);
	OffsetRect(&r.rc, -r.offsetX, -r.offsetY);
	return r;
}

int ImageInfo::getSize() { auto r = calcRect2();
	return 32 + 4 + r.width*r.height/2; }

void ImageInfo::write(FILE* fp)
{
	// create output image
	auto r = calcRect2();
	ImageLib::ImageObj tmp; 
	ImageLib::ResizeResample(tmp, img, 
		bkgndColor, r.width, r.height, r.rc, 0);
	
	// remap palette
	memcpyX(tmp.palette, img.palette, 16);
	std::swap(tmp.palette[bkgndColor], tmp.palette[0]);
	for(byte& ch : Range(tmp.bColors, tmp.nPixels())) {
	if(ch == 0) ch = bkgndColor; ei(ch == bkgndColor) ch = 0; }
	
	// write image header
	ImageHeadr head;
	head.scrollX = bswap16(r.offsetX);
	head.scrollY = bswap16(-r.offsetY);
	head.width = bswap16(r.width/8); 
	head.height = bswap16(r.height/8);
	for(int i = 0; i < 16; i++) { auto c = tmp.palette[i];
		head.pal[i] = ((c.r>>5)<<9)|((c.g>>5)<<13)|((c.b>>5)<<1); }
	head.nTile = bswap16((r.width * r.height)/64);
	xfwrite(head, fp);
	
	// write image tiles
	byte tile[32];
	for(int y = 0; y < r.height; y += 8)
	for(int x = 0; x < r.width; x += 8) {
		byte* src = tmp.get8(x, y);
	for(int z = 0; z < 32; z += 4) {
		tile[z+0] = (src[0] << 4) | src[1];
		tile[z+1] = (src[2] << 4) | src[3];
		tile[z+2] = (src[4] << 4) | src[5];
		tile[z+3] = (src[6] << 4) | src[7];
		src += r.width;
	}	xfwrite(tile, fp); 
	}
}

void ImageInfo::draw(HDC hdc, int x, int y)
{
	ImageLib::ImageObj ret;	
	auto rc = calcRect();
	ImageLib::ResizeResample(ret, img, 
		bkgndColor, 320, 224, rc, 0);
	ret.bitBlt(hdc, x, y);
}
