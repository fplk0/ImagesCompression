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

    virtual inline void convertColorFromRGB(int *src, int *dst)
	{
		memcpy(dst, src, componentsCount * sizeof(int));
	}

    virtual inline void convertColorToRGB(int *src, int *dst)
	{
		memcpy(dst, src, componentsCount * sizeof(int));
	}

	virtual void convertImageFromRGB(int *src, int *dst, int pixelCount);
	virtual void convertImageToRGB(int *src, int *dst, int pixelCount);
	virtual void convertImageToRGB(int **components, int *dst, int pixelCount);

    virtual int lowerBoundForComponent(int componentIndex)
	{
		return 0;
	}

    virtual int upperBoundForComponent(int componentIndex)
	{
		return 255;
	}

    virtual int neutralValueForComponent(int componentIndex)
	{
		return 255;
	}

	virtual ~ColorSpaceRGB(void);
};
