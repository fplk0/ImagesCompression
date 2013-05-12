#pragma once

#include "colorspace.h"

#undef super
#undef self

#define self ColorSpaceYCbCr
#define super ColorSpace

class ColorSpaceYCbCr :
	public ColorSpace
{
	DEFAULT_METACLASS_DECLARATION(self, super);

public:

	static ColorSpaceYCbCr* singleton();
	virtual ColorSpaceYCbCr* init();

    virtual wstring nameOfComponentAtIndex(int componentIndex);

    virtual inline void convertColorFromRGB(int *src, int *dst);

    virtual inline void convertColorToRGB(int *src, int *dst);

	virtual void convertImageFromRGB(int *src, int *dst, int pixelCount);
	virtual void convertImageToRGB(int *src, int *dst, int pixelCount);
	virtual void convertImageToRGB(int **components, int *dst, int pixelCount);

    virtual int lowerBoundForComponent(int componentIndex);

    virtual int upperBoundForComponent(int componentIndex);

    virtual int neutralValueForComponent(int componentIndex);

	virtual ~ColorSpaceYCbCr(void);
};

