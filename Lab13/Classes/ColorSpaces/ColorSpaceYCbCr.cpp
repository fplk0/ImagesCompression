#include "stdafx.h"
#include "Atomic.h"

#include "ColorSpaceYCbCr.h"

DEFINE_OBJECT_IMPL(self);

inline void _YCbCrFromRGB(float &r, float &g, float &b, float &y, float &cb, float &cr)
{
	y = 0.299f * r + 0.587f * g + 0.114f * b;
	cb = 0.5f - 0.168736f * r - 0.331264f * g + 0.5f * b;
	cr = 0.5f + 0.5f * r - 0.418688f * g - 0.081312f * b;
}

inline void _RGBFromYCbCr(float &y, float &cb, float &cr, float &r, float &g, float &b)
{
	r = y + 1.402f * (cr - 0.5f);
	g = y - 0.34414f * (cb - 0.5f) - 0.71414f * (cr - 0.5f);
	b = y + 1.772f * (cb - 0.5f);
}

ColorSpaceYCbCr* ColorSpaceYCbCr::singleton()
{
	static ColorSpaceYCbCr *singleInstance;
	SFPerformOnceBegin
		singleInstance = ColorSpaceYCbCr::alloc()->init();
	SFPerformOnceEnd
	return singleInstance;
}

ColorSpaceYCbCr* ColorSpaceYCbCr::init()
{
	super::init();

	componentsCount = 3;
	return this;
}

wstring ColorSpaceYCbCr::nameOfComponentAtIndex(int componentIndex)
{
	switch (componentIndex)
	{
	case 0:
		return L"Y";
	case 1:
		return L"Cb";
	case 2:
		return L"Cr";
	}
	return L"";
}

void ColorSpaceYCbCr::convertColorFromRGB(float *src, float *dst)
{
	_YCbCrFromRGB(src[0], src[1], src[2], dst[0], dst[1], dst[2]);
}

void ColorSpaceYCbCr::convertColorToRGB(float *src, float *dst)
{
	_RGBFromYCbCr(src[0], src[1], src[2], dst[0], dst[1], dst[2]);
}

void ColorSpaceYCbCr::convertImageFromRGB(float *src, float *dst, int pixelCount)
{
	int shiftSrc = 3 * sizeof(*src);
	int shiftDst = componentsCount * sizeof(*dst);

	for (int i = 0; i < pixelCount; i++)
	{
		_YCbCrFromRGB(src[0], src[1], src[2], dst[0], dst[1], dst[2]);
		src += shiftSrc;
		dst += shiftDst;
	}
}

void ColorSpaceYCbCr::convertImageToRGB(float *src, float *dst, int pixelCount)
{
	int shiftSrc = componentsCount * sizeof(*src);
	int shiftDst = 3 * sizeof(*dst);

	for (int i = 0; i < pixelCount; i++)
	{
		_YCbCrFromRGB(src[0], src[1], src[2], dst[0], dst[1], dst[2]);
		src += shiftSrc;
		dst += shiftDst;
	}
}

float ColorSpaceYCbCr::lowerBoundForComponent(int componentIndex)
{
	return 0.0;
}

float ColorSpaceYCbCr::upperBoundForComponent(int componentIndex)
{
	return 1.0;
}

float ColorSpaceYCbCr::neutralValueForComponent(int componentIndex)
{
	switch (componentIndex)
	{
	case 0:
		return 0.0;
	case 1: case 2:
		return 0.5;
	}
	return 0.0;
}

ColorSpaceYCbCr::~ColorSpaceYCbCr(void)
{
}
