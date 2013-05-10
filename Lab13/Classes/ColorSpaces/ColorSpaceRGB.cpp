#include "stdafx.h"
#include "ColorSpaceRGB.h"
#include "Atomic.h"

DEFINE_OBJECT_IMPL(self);

ColorSpaceRGB* ColorSpaceRGB::singleton()
{
	static ColorSpaceRGB *singleInstance;
	SFPerformOnceBegin
		Sleep(10000);
		printf("2");
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

void ColorSpaceRGB::convertImageFromRGB(float *src, float *dst, int pixelCount)
{
	if (src == dst)
		return;
	memcpy(dst, src, pixelCount * componentsCount * sizeof(float));
}

void ColorSpaceRGB::convertImageToRGB(float *src, float *dst, int pixelCount)
{
	if (src == dst)
		return;
	memcpy(dst, src, pixelCount * componentsCount * sizeof(float));
}

ColorSpaceRGB::~ColorSpaceRGB(void)
{
}
