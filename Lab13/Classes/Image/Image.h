#pragma once
#include "Object.h"
#include "ColorSpace.h"

#undef super
#undef self

#define super Object
#define self Image

class Image :
	public super
{
	DEFAULT_METACLASS_DECLARATION(self, super);

	float *pixels;
	
	wstring comment;

	ColorSpace *colorSpace;

	int componentsCount;
	int width;
	int height;
	int alignedWidth;

public:

	wstring getComment() { return comment; };
	void setComment(const wstring &value) { comment = value; };

	inline float getPixel(int x, int y, int component)
	{
		return pixels[component + (y * alignedWidth + x) * componentsCount];
	}

	inline void setPixel(int x, int y, int component, float value)
	{
		pixels[component + (y * alignedWidth + x) * componentsCount] = value;
	}

	int getWidth() { return width; };
	int getHeight() { return height; };
	ColorSpace* getColorSpace() { return colorSpace; };

	virtual Image* init();
	virtual Image* initWithSizeAndColorSpace(int width, int height, ColorSpace *cs);
	Image* imageByConvertingToColorSpace(ColorSpace *newCs);
	void convertToColorSpace(ColorSpace *newCs);
	virtual Image* copy();
	virtual void free();

	virtual ~Image(void);
};