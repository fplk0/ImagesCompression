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


int SFStreamWriter::writeBytes(byte *bytes, size_t size, bool reverse)
{
	this->_flushStreamIfNeeded(1, 0);

	if (!reverse)
	{
		for (size_t pos = 0; pos < size; pos++)
		{
			if (bufPos == bufSize)
				this->_flushStreamIfNeeded();
			buf[bufPos++] = bytes[pos];
		}
	}
	else
	{
		for (size_t pos = size - 1; pos >= 0; pos--)
		{
			if (bufPos == bufSize)
				this->_flushStreamIfNeeded();
			buf[bufPos++] = bytes[pos];
		}
	}
	return 0;
}

int SFStreamWriter::writeByte(byte bt)
{
	this->_flushStreamIfNeeded();
	buf[bufPos++] = bt;
	return 0;
}

int SFStreamWriter::writeShort(unsigned short sh)
{
	this->writeBytes((byte*)&sh, 2, true);
	return 0;
}

int SFStreamWriter::writeInt(unsigned int val)
{
	this->writeBytes((byte*)&val, 4, true);
	return 0;
}

int SFStreamWriter::writeLL(unsigned long long val)
{
	this->writeBytes((byte*)&val, 8, true);
	return 0;
}

int SFStreamWriter::writeBits(int num, size_t bitsCt)
{
	while(bitsCt > 0)
	{
		this->_flushStreamIfNeeded(false);

		if (bitPos == 0)
			buf[bufPos] = 0;

		size_t bitsLeft = 8 - bitPos;
		int bitsToWrite = min(bitsLeft, bitsCt);
		int shift = bitsCt - bitsToWrite;
		int mask = (1 << bitsToWrite) - 1;
		int leftShift = bitsLeft - bitsToWrite;
		int tmp = (num >> shift) & mask;
		tmp <<= leftShift;

		buf[bufPos] |= tmp;
		bitPos += bitsToWrite;
		bitsCt -= bitsToWrite;

		if (bitPos == 8)
		{
			bitPos = 0;
			bufPos++;
		}
	}
	return 0;
}

void SFStreamWriter::closeStream()
{
	this->_flushStreamIfNeeded(true, true);
	super::closeStream();
}

SFStreamWriter::~SFStreamWriter()
{

}