#include "stdafx.h"
#include "SFData.h"

#include <cstdlib>

DEFINE_OBJECT_IMPL(self)

SFData* SFData::init()
{
	return this->initWithSize();
}

SFData* SFData::initWithData(byte *src, size_t size, bool copy)
{
	SFData *retVal = static_cast<SFData*>(super::init());

	if (retVal != NULL)
	{
		retVal->_isAllocated = copy;

		if (copy)
		{
			retVal->data = NULL;
			retVal->resize(size);
			memcpy(retVal->data, src, size);
		}
		else
		{
			retVal->len = size;
			retVal->data = src;
		}
	}

	return retVal;
}

SFData* SFData::initWithSize(size_t size)
{
	SFData *retVal = static_cast<SFData*>(super::init());

	if (retVal != NULL)
	{
		retVal->_isAllocated = true;
		retVal->data = NULL;
		retVal->resize(size);
	}

	return retVal;
}

void SFData::resize(size_t newSize)
{
	if (_isAllocated)
	{
		data = static_cast<byte*>(realloc(data, newSize));
		len = newSize;
	}
}

void SFData::free()
{
	if (_isAllocated)
		::free(data);
	data = NULL;

	super::free();
}

SFData::~SFData(void)
{
}
