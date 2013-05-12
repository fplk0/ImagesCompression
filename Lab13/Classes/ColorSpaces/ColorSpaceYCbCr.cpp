#include "stdafx.h"
#include "Atomic.h"

#include "ColorSpaceYCbCr.h"

DEFINE_OBJECT_IMPL(self);

inline void _YCbCrFromRGB(int &r, int &g, int &b, int &y, int &cb, int &cr)
{
	y = (int) (0.299f * r + 0.587f * g + 0.114f * b);
	cb = (int) (128 - 0.168736f * r - 0.331264f * g + 0.5f * b);
	cr = (int) (128 + 0.5f * r - 0.418688f * g - 0.081312f * b);
}

inline void _RGBFromYCbCr(int &y, int &cb, int &cr, int &r, int &g, int &b)
{
	cr -= 128;
	cb -= 128;
	r = (int) (y + 1.402f * cr);
	g = (int) (y - 0.34414f * cb - 0.71414f * cr);
	b = (int) (y + 1.772f * cb);
	if (r > 255)
		r = 255;
	if (g > 255)
		g = 255;
	if (b > 255)
		b = 255;

	if (r < 0)
		r = 0;
	if (g < 0)
		g = 0;
	if (b < 0)
		b = 0;
}

ColorSpaceYCbCr* ColorSpaceYCbCr::singleton()
{
	static ColorSpaceYCbCr *singleInstance = NULL;
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

void ColorSpaceYCbCr::convertColorFromRGB(int *src, int *dst)
{
	_YCbCrFromRGB(src[0], src[1], src[2], dst[0], dst[1], dst[2]);
}

void ColorSpaceYCbCr::convertColorToRGB(int *src, int *dst)
{
	_RGBFromYCbCr(src[0], src[1], src[2], dst[0], dst[1], dst[2]);
}

void ColorSpaceYCbCr::convertImageFromRGB(int *src, int *dst, int pixelCount)
{
	int shiftSrc = 3;
	int shiftDst = componentsCount;

	for (int i = 0; i < pixelCount; i++)
	{
		_YCbCrFromRGB(src[0], src[1], src[2], dst[0], dst[1], dst[2]);
		src += shiftSrc;
		dst += shiftDst;
	}
}

void ColorSpaceYCbCr::convertImageToRGB(int *src, int *dst, int pixelCount)
{
	int shiftSrc = componentsCount * sizeof(*src);
	int shiftDst = 3;

	for (int i = 0; i < pixelCount; i++)
	{
		_RGBFromYCbCr(src[0], src[1], src[2], dst[0], dst[1], dst[2]);
		src += shiftSrc;
		dst += shiftDst;
	}
}

void ColorSpaceYCbCr::convertImageToRGB(int **src, int *dst, int pixelCount)
{
	int shiftDst = 3;
	for (int i = 0; i < pixelCount; i++)
	{
		_RGBFromYCbCr(src[0][i], src[1][i], src[2][i], dst[0], dst[1], dst[2]);
		dst += shiftDst;
	}
}

int ColorSpaceYCbCr::lowerBoundForComponent(int componentIndex)
{
	return 0;
}

int ColorSpaceYCbCr::upperBoundForComponent(int componentIndex)
{
	return 255;
}

int ColorSpaceYCbCr::neutralValueForComponent(int componentIndex)
{
	switch (componentIndex)
	{
	case 0:
		return 0;
	case 1: case 2:
		return 127;
	}
	return 0;
}

ColorSpaceYCbCr::~ColorSpaceYCbCr(void)
{
}
