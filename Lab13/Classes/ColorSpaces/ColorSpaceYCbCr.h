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

    virtual inline void convertColorFromRGB(float *src, float *dst);

    virtual inline void convertColorToRGB(float *src, float *dst);

	virtual void convertImageFromRGB(float *src, float *dst, int pixelCount);
	virtual void convertImageToRGB(float *src, float *dst, int pixelCount);

    virtual float lowerBoundForComponent(int componentIndex);

    virtual float upperBoundForComponent(int componentIndex);

    virtual float neutralValueForComponent(int componentIndex);

	virtual ~ColorSpaceYCbCr(void);
};

