#pragma once
#include "SFStream.h"

#include "PlatformSpecific.h"

#undef super
#undef self

#define super SFStream

class SFStreamReader :
	public super
{
	START_METACLASS_DECLARATION(SFStreamReader, super);
	DEFINE_METACLASS_ABSTRACT_ALLOC(SFStreamReader);
	END_METACLASS_DECLARATION(SFStreamReader, super);

	size_t bytesRead;
	int readNext(bool skipBits = 1, bool forceRead = 0);

	inline bool _shouldReadNext() { return (bufPos + (bool)bitPos) >= bytesRead; };

	virtual size_t _scanNext(byte *buf, size_t maxSize) { return 0; };
	virtual bool _atEnd() { return true; };

public:

	virtual bool rewind();
	virtual void openStream();

	SF_FORCE_INLINE bool atEnd(bool skipBits = 0);
	SF_FORCE_INLINE void skipBytes(size_t skipCount);
	SF_FORCE_INLINE size_t readBytes(byte *bytes, size_t size, bool reverse = 0);
	SF_FORCE_INLINE size_t readByte(byte &bt);
	SF_FORCE_INLINE size_t readShort(unsigned short &sh);
	SF_FORCE_INLINE size_t readInt(unsigned int &sh);
	SF_FORCE_INLINE byte readByte();
	SF_FORCE_INLINE unsigned short readShort();
	SF_FORCE_INLINE unsigned int readInt();
	SF_FORCE_INLINE unsigned long long readLL();
	SF_FORCE_INLINE size_t readLL(unsigned long long &ll);
	SF_FORCE_INLINE size_t readBits(int &num, size_t bitsCt);

	virtual size_t streamSize() = 0;

	virtual ~SFStreamReader(void);
};

bool SFStreamReader::atEnd(bool skipBits)
{
	int cp = bufPos;
	if (skipBits && bitPos > 0)
		cp++;
	if (cp == bytesRead && this->_atEnd())
		return 1;
	return 0;
}

void SFStreamReader::skipBytes(size_t skipCount)
{
	while (true)
	{
		size_t inc = min(skipCount, bytesRead - bufPos);
		bufPos += inc;
		skipCount -= inc;
		if (skipCount == 0)
			break;
		readNext();
	}
}

size_t SFStreamReader::readBytes(byte *bytes, size_t size, bool reverse)
{
	if (!reverse)
	{
		size_t pos = 0;
		for (pos = 0; pos < size; pos++)
		{
			if (_shouldReadNext() && this->readNext(1) < 0)
				break;
			bytes[pos] = buf[bufPos++];
		}
		return pos;
	}
	else
	{
		size_t pos = size - 1;
		for (; pos >= 0; pos--)
		{
			if (_shouldReadNext() && this->readNext(1) < 0)
				break;
			bytes[pos] = buf[bufPos++];
		}
		return size - pos - 1;
	}
}

size_t SFStreamReader::readByte(byte &bt)
{
	return this->readBytes(&bt, 1);
}

size_t SFStreamReader::readShort(unsigned short &sh)
{
	return this->readBytes((byte*)&sh, 2, true);
}

size_t SFStreamReader::readInt(unsigned int &sh)
{
	return this->readBytes((byte*)&sh, 4, true);
}

size_t SFStreamReader::readLL(unsigned long long &ll)
{
	return this->readBytes((byte*)&ll, 8, true);
}

byte SFStreamReader::readByte()
{
	byte retVal;
	readByte(retVal);
	return retVal;
}

unsigned short SFStreamReader::readShort()
{
	unsigned short retVal;
	readShort(retVal);
	return retVal;
}

unsigned int SFStreamReader::readInt()
{
	unsigned int retVal;
	readInt(retVal);
	return retVal;
}

unsigned long long SFStreamReader::readLL()
{
	unsigned long long retVal;
	readLL(retVal);
	return retVal;

}

size_t SFStreamReader::readBits(int &num, size_t bitsCt)
{
	size_t readBits = 0;
	num = 0;
	while(bitsCt > 0)
	{
		if (_shouldReadNext() && readNext(0) < 0)
			break;
		size_t bitsLeft = 8 - bitPos;
		int bitsToRead = min(bitsLeft, bitsCt);
		int mask = (1 << bitsToRead) - 1;
		int sh1 = bitsLeft - bitsToRead;
		int tmp = (buf[bufPos] >> sh1) & mask;
		tmp <<= bitsCt - bitsToRead;
		num |= tmp;
		readBits += bitsToRead;
		bitPos += bitsToRead;

		if (bitPos >= 8)
		{
			bitPos = 0;
			bufPos++;
		}
		bitsCt -= bitsToRead;
	}

	return readBits;
}