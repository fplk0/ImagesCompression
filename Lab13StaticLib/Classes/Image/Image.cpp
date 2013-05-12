#include "stdafx.h"
#include "ColorSpaceRGB.h"

#include "Image.h"

DEFINE_OBJECT_IMPL(Image);

Image* Image::init()
{
	throw new exception("You should initialize this class by using initWithSizeAndColorSpace");
	return NULL;
}

Image* Image::initWithSizeAndColorSpace(int _width, int _height, ColorSpace *cs)
{
	super::init();
	width = alignedWidth = _width;
	height = _height;
	colorSpace = cs;
	colorSpace->retain();
	componentsCount = cs->getComponentsCount();

	if (width * height != 0)
		pixels = new int[alignedWidth * height * componentsCount];

	return this;
}

Image* Image::imageByConvertingToColorSpace(ColorSpace *newCs)
{
	if (newCs->metaClass() == colorSpace->metaClass())
		return this->copy();
	Image *rv = Image::alloc()->initWithSizeAndColorSpace(this->width, this->height, newCs);
	
	if (colorSpace->metaClass() == ColorSpaceRGB::metaClassStatic())
	{
		newCs->convertImageFromRGB(this->pixels, rv->pixels, alignedWidth * height);
	}
	else if (newCs->metaClass() == ColorSpaceRGB::metaClassStatic())
	{
		colorSpace->convertImageToRGB(this->pixels, rv->pixels, alignedWidth * height);
	}
	else
	{
		int *rgbBuf = new int[alignedWidth * height * ColorSpaceRGB::singleton()->getComponentsCount()];
		colorSpace->convertImageToRGB(this->pixels, rgbBuf, alignedWidth * height);
		newCs->convertImageFromRGB(rgbBuf, rv->pixels, alignedWidth * height);
		delete [] rgbBuf;
	}

	return rv;
}

void Image::convertToColorSpace(ColorSpace *newCs)
{
	if (newCs->metaClass() == colorSpace->metaClass())
		return;

	int *rgbBuf = this->pixels, *dstBuf = this->pixels;

	int pixelsCount = alignedWidth * height;

	if (ColorSpaceRGB::singleton()->getComponentsCount() != colorSpace->getComponentsCount())
	{
		rgbBuf = new int[ColorSpaceRGB::singleton()->getComponentsCount() * pixelsCount];
	}

	if (newCs->getComponentsCount() != colorSpace->getComponentsCount())
	{
		dstBuf = new int[newCs->getComponentsCount() * pixelsCount];
	}

	if (colorSpace->metaClass() == ColorSpaceRGB::metaClassStatic())
	{
		newCs->convertImageFromRGB(this->pixels, dstBuf, pixelsCount);
	}
	else if (newCs->metaClass() == ColorSpaceRGB::metaClassStatic())
	{
		colorSpace->convertImageToRGB(this->pixels, dstBuf, pixelsCount);
	}
	else
	{
		colorSpace->convertImageToRGB(this->pixels, rgbBuf, pixelsCount);
		newCs->convertImageFromRGB(rgbBuf, dstBuf, pixelsCount);
	}

	this->colorSpace->release();
	this->colorSpace = newCs;
	this->colorSpace->retain();

	if (rgbBuf != dstBuf)
		delete [] rgbBuf;
	if (this->pixels != dstBuf)
	{
		delete [] this->pixels;
		this->pixels = dstBuf;
	}
}

Image* Image::copy()
{
	Image *rv = static_cast<Image*>(Object::copy());
	
	rv->width = width;
	rv->height = height;
	rv->colorSpace = colorSpace;
	rv->colorSpace->retain();

	rv->componentsCount = componentsCount;

	rv->pixels = new int[alignedWidth * height * componentsCount];

	memcpy(rv->pixels, pixels, alignedWidth * height * componentsCount * sizeof(float));

	return rv;
}

void Image::free()
{
	if (colorSpace != NULL)
		colorSpace->release();
	colorSpace = NULL;

	delete [] pixels;

	super::free();
}

Image::~Image(void)
{
}
