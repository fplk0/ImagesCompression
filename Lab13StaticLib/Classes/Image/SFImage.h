#pragma once
#include "SFObject.h"
#include "ColorSpace.h"

#undef super
#undef self

#define super SFObject
#define self SFImage

class SFImage :
	public super
{
	DEFAULT_METACLASS_DECLARATION(self, super);

	int *pixels;

	ColorSpace *colorSpace;

	int componentsCount;
	int width;
	int height;
	int alignedWidth;

public:

	wstring comment;
	wstring getComment() { return comment; };
	void setComment(const wstring &value) { comment = value; };

	inline int getPixel(int x, int y, int component)
	{
		return pixels[component + (y * alignedWidth + x) * componentsCount];
	}

	inline void setPixel(int x, int y, int component, int value)
	{
		pixels[component + (y * alignedWidth + x) * componentsCount] = value;
	}

	int getWidth() { return width; };
	int getHeight() { return height; };
	ColorSpace* getColorSpace() { return colorSpace; };

	virtual SFImage* init();
	virtual SFImage* initWithSizeAndColorSpace(int width, int height, ColorSpace *cs);
	SFImage* imageByConvertingToColorSpace(ColorSpace *newCs);
	void convertToColorSpace(ColorSpace *newCs);
	virtual SFImage* copy();
	virtual void free();

	virtual ~SFImage(void);
};