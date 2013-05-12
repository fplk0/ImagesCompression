#include "stdafx.h"
#include "SFMemoryStreamReader.h"

DEFINE_OBJECT_IMPL(self);

SFMemoryStreamReader* SFMemoryStreamReader::initWithData(SFData *_data)
{
	this->init();

	data = _data;
	if (data != NULL)
		data->retain();
	posInData = 0;

	return this;
}

bool SFMemoryStreamReader::_rewind(long long bytesCnt)
{
	posInData -= bytesCnt;
	return true;
}

size_t SFMemoryStreamReader::_scanNext(byte *buf, size_t maxSize)
{
	size_t bytesToRead = min(maxSize, data->getLength() - posInData);
	memcpy(buf, data->getData() + posInData, bytesToRead);
	posInData += bytesToRead;
	return bytesToRead;
}

bool SFMemoryStreamReader::_atEnd()
{
	return posInData == data->getLength();
}

bool SFMemoryStreamReader::rewind()
{
	posInData = 0;
	super::rewind();
	return true;
}

void SFMemoryStreamReader::openStream()
{
	posInData = 0;
	super::openStream();
}

void SFMemoryStreamReader::free()
{
	if (data != NULL)
		data->release();
	data = NULL;

	super::free();
}

size_t SFMemoryStreamReader::streamSize()
{
	return data->getLength();
}

SFMemoryStreamReader::~SFMemoryStreamReader(void)
{
}
