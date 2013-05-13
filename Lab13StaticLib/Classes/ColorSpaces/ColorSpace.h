#pragma once
#include "SFObject.h"
#include <string>

using namespace std;

#undef super
#undef self

#define super SFObject
#define self ColorSpace

class ColorSpace :
	public SFObject
{
	ABSTRACT_METACLASS_DECLARATION(self, super);

protected:
	int componentsCount;
	wstring name;

public:
	int getComponentsCount()
	{
		return componentsCount;
	}

protected:
    void setComponentsCount(int value)
	{
		componentsCount = value;
	}

public:
	wstring getName()
	{
		return name;
	}

protected:
    void setName(const wstring& value)
    {
        name = value;
    }

public:
    virtual wstring nameOfComponentAtIndex(int componentIndex) = 0;
    virtual inline void convertColorFromRGB(int *src, int *dst) = 0;
    virtual inline void convertColorToRGB(int *src, int *dst) = 0;

	virtual void convertImageFromRGB(int *src, int *dst, int pixelCount) = 0;
	virtual void convertImageToRGB(int *src, int *dst, int pixelCount) = 0;
	virtual void convertImageToRGB(int **components, int *dst, int pixelCount) = 0;

    virtual int lowerBoundForComponent(int componentIndex) = 0;
    virtual int upperBoundForComponent(int componentIndex) = 0;
    virtual int neutralValueForComponent(int componentIndex)
    {
        return 0;
    }

public:
	virtual ~ColorSpace(void);
};

