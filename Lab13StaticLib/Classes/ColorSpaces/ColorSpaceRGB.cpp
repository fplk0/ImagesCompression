#include "stdafx.h"
#include "ColorSpaceRGB.h"
#include "Atomic.h"

DEFINE_OBJECT_IMPL(self);

ColorSpaceRGB* ColorSpaceRGB::singleton()
{
	static ColorSpaceRGB *singleInstance;
	SFPerformOnceBegin
		singleInstance = ColorSpaceRGB::alloc()->init();
	SFPerformOnceEnd
	return singleInstance;
}

ColorSpaceRGB* ColorSpaceRGB::init()
{
	super::init();

	componentsCount = 3;

	return this;
}

wstring ColorSpaceRGB::nameOfComponentAtIndex(int componentIndex)
{
	switch(componentIndex)
	{
	case 0:
		return L"Red";
	case 1:
		return L"Green";
	case 2:
		return L"Blue";
	}

	return L"";
}

void ColorSpaceRGB::convertImageFromRGB(int *src, int *dst, int pixelCount)
{
	if (src == dst)
		return;
	memcpy(dst, src, pixelCount * componentsCount * sizeof(int));
}

void ColorSpaceRGB::convertImageToRGB(int *src, int *dst, int pixelCount)
{
	if (src == dst)
		return;
	memcpy(dst, src, pixelCount * componentsCount * sizeof(int));
}

void ColorSpaceRGB::convertImageToRGB(int **components, int *dst, int pixelCount)
{
	for (int i = 0; i < pixelCount; i++)
	{
		dst[3*i] = components[0][i];
		dst[3*i+1] = components[1][i];
		dst[3*i+2] = components[2][i];
	}
}

ColorSpaceRGB::~ColorSpaceRGB(void)
{
}
