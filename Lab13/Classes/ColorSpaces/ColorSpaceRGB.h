#pragma once
#include "colorspace.h"

#undef super
#undef self

#define super ColorSpace
#define self ColorSpaceRGB

class ColorSpaceRGB :
	public ColorSpace
{
	DEFAULT_METACLASS_DECLARATION(self, super);

public:

	static ColorSpaceRGB* singleton();
	virtual ColorSpaceRGB* init();

    virtual wstring nameOfComponentAtIndex(int componentIndex);

    virtual inline void convertColorFromRGB(float *src, float *dst)
	{
		memcpy(dst, src, componentsCount * sizeof(float));
	}

    virtual inline void convertColorToRGB(float *src, float *dst)
	{
		memcpy(dst, src, componentsCount * sizeof(float));
	}

	virtual void convertImageFromRGB(float *src, float *dst, int pixelCount);
	virtual void convertImageToRGB(float *src, float *dst, int pixelCount);

    virtual float lowerBoundForComponent(int componentIndex)
	{
		return 0.0;
	}

    virtual float upperBoundForComponent(int componentIndex)
	{
		return 1.0;
	}

    virtual float neutralValueForComponent(int componentIndex)
	{
		return 0.0;
	}

	virtual ~ColorSpaceRGB(void);
};
