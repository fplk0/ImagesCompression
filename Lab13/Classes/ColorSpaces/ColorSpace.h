#pragma once
#include "object.h"
#include <string>

using namespace std;

#undef super
#undef self

#define super Object
#define self ColorSpace

class ColorSpace :
	public Object
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
    virtual inline void convertColorFromRGB(float *src, float *dst) = 0;
    virtual inline void convertColorToRGB(float *src, float *dst) = 0;

	virtual void convertImageFromRGB(float *src, float *dst, int pixelCount) = 0;
	virtual void convertImageToRGB(float *src, float *dst, int pixelCount) = 0;

    virtual float lowerBoundForComponent(int componentIndex) = 0;
    virtual float upperBoundForComponent(int componentIndex) = 0;
    virtual float neutralValueForComponent(int componentIndex)
    {
        return 0.0f;
    }

public:
	virtual ~ColorSpace(void);
};

