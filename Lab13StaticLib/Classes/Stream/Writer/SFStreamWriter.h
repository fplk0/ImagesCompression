#pragma once
#include "SFStream.h"

#undef super
#undef self

#define super SFStream
#define self SFStreamWriter

class SFStreamWriter :
	public super
{
	START_METACLASS_DECLARATION(self, super);
	END_METACLASS_DECLARATION(self, super);

	inline bool _shouldWriteNext() { return (bufPos + ((bitPos == 0) ? 0 : 1)) >= bufSize; };
	int _flushStreamIfNeeded(bool flushBits = 1, bool forceFlush = 0);
	virtual size_t _writeToStream(byte *buf, size_t byteCnt) = 0;

public:
	int totalBytesWritten;

	virtual SFStreamWriter* init();
	int flushStream(bool flushBits = 1);

	inline int writeBytes(byte *bytes, size_t size, bool reverse = 0);
	inline int writeByte(byte bt);
	inline int writeShort(unsigned short sh);
	inline int writeInt(unsigned int val);
	inline int writeLL(unsigned long long val);
	inline int writeBits(int num, size_t bitsCt);

	virtual void openStream();
	virtual void closeStream();

	virtual ~SFStreamWriter(void);
};

int SFStreamWriter::writeBytes(byte *bytes, size_t size, bool reverse)
{
	if (_shouldWriteNext())
		this->_flushStreamIfNeeded(1, 0);

	if (!reverse)
	{
		for (size_t pos = 0; pos < size; pos++)
		{
			if (_shouldWriteNext())
				this->_flushStreamIfNeeded();
			buf[bufPos++] = bytes[pos];
		}
	}
	else
	{
		for (size_t pos = size - 1; ; pos--)
		{
			if (_shouldWriteNext())
				this->_flushStreamIfNeeded();
			buf[bufPos++] = bytes[pos];

			if (pos == 0)
				break;
		}
	}
	return 0;
}

int SFStreamWriter::writeByte(byte bt)
{
	if (_shouldWriteNext())
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
		if (_shouldWriteNext())
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