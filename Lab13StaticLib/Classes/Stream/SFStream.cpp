#include "stdafx.h"
#include "SFStream.h"

DEFINE_OBJECT_IMPL(SFStream);

SFStream* SFStream::init()
{
	return this->initWithBufSize();
}

SFStream* SFStream::initWithBufSize(size_t _bufSize)
{
	SFStream *retVal = static_cast<SFStream*>(super::init());
	retVal->bufSize = _bufSize;
	retVal->buf = NULL;
	_opened = false;
	return retVal;
}

void SFStream::openStream()
{
	if (!_opened)
		_opened = true;
	else
		return;
	if (this->buf == NULL)
		this->buf = (byte*)malloc(bufSize);
	bufPos = 0;
	bitPos = 0;
}

void SFStream::closeStream()
{
	if (_opened)
		_opened = false;
	else
		return;
	::free(this->buf);
	this->buf = NULL;
}

void SFStream::free()
{
	if (_opened)
		this->closeStream();

	super::free();
}

SFStream::~SFStream(void)
{

}
