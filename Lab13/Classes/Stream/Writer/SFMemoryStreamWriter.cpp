#include "stdafx.h"
#include "SFMemoryStreamWriter.h"

DEFINE_OBJECT_IMPL(self);

size_t SFMemoryStreamWriter::_writeToStream(byte *buf, size_t byteCnt)
{
	if (byteCnt == 0)
		return 0;
	if (posInData + byteCnt > data->getLength())
	{
		if (_dynamic)
		{
			size_t nextSize = max(data->getLength() * 2, posInData + byteCnt);
			data->resize(nextSize);
		}
		else
			return 0;
	}

	memcpy(data->getData() + posInData, buf, byteCnt);
	posInData += byteCnt;
	return byteCnt;
}

SFMemoryStreamWriter* SFMemoryStreamWriter::initDynamic(size_t startSize)
{
	super::init();
	_dynamic = true;
	data = SFData::alloc()->initWithSize(startSize);
	posInData = 0;

	return this;
}

SFMemoryStreamWriter* SFMemoryStreamWriter::initWithData(SFData *outData)
{
	super::init();
	_dynamic = false;
	data = outData;
	data->retain();
	posInData = 0;

	return this;
}

SFData* SFMemoryStreamWriter::getData()
{
	return data;
}

void SFMemoryStreamWriter::openStream()
{
	posInData = 0;
	super::openStream();
}

void SFMemoryStreamWriter::closeStream()
{
	super::closeStream();
	data->resize(posInData);
}

void SFMemoryStreamWriter::free()
{
	data->release();
	super::free();
}

SFMemoryStreamWriter::~SFMemoryStreamWriter(void)
{
}
