#include "stdafx.h"
#include "SFStreamWriter.h"

#include <assert.h>

DEFINE_OBJECT_IMPL(self);

SFStreamWriter* SFStreamWriter::init()
{
	super::init();
	return this;
}

int SFStreamWriter::_flushStreamIfNeeded(bool flushBits, bool forceFlush)
{
	if (flushBits)
	{
		if (bitPos > 0)
		{
			bitPos = 0;
			bufPos++;
		}
	}
	if (bufPos == bufSize || forceFlush)
	{
		size_t writtenBytes = _writeToStream(buf, bufPos);
		totalBytesWritten += writtenBytes;
		//assert(writtenBytes == bufPos);
		if (forceFlush && !flushBits && bitPos != 0)
		{
			buf[0] = buf[bufPos];
		}
		bufPos = 0;
	}
	return 0;
}

int SFStreamWriter::flushStream(bool flushBits)
{
	return this->_flushStreamIfNeeded(flushBits, true);
}

void SFStreamWriter::openStream()
{
	super::openStream();
	totalBytesWritten = 0;
}

void SFStreamWriter::closeStream()
{
	this->_flushStreamIfNeeded(true, true);
	super::closeStream();
}

SFStreamWriter::~SFStreamWriter()
{

}