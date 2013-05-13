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

	long long totalBytesRead;
	size_t bytesRead;
	int readNext(bool skipBits = 1, bool forceRead = 0);

	inline bool _shouldReadNext() { return (bufPos + ((bitPos == 0) ? 0 : 1)) >= bytesRead; };

	virtual size_t _scanNext(byte *buf, size_t maxSize) { return 0; };
	virtual bool _atEnd() { return true; };
	virtual bool _rewind(long long rewCount) { return true; };

	void _shiftLeft(size_t byteCnt);

public:

	virtual bool rewind();
	virtual void openStream();

	bool rewind(long long byteCount);

	inline bool atEnd(bool skipBits = 0);
	inline void skipBytes(size_t skipCount);

	inline byte nextByte();

	inline void padLastByteBits() { if (bitPos != 0) { bitPos = 0; bufPos++; } };
	inline size_t readBytes(byte *bytes, size_t size);
	inline size_t readBytesRev(byte *bytes, size_t size);
	inline size_t readByte(byte &bt);
	inline size_t readShort(unsigned short &sh);
	inline size_t readInt(unsigned int &sh);
	inline size_t readLL(unsigned long long &ll);
	inline byte readByte();
	inline unsigned short readShort();
	inline unsigned int readInt();
	inline unsigned long long readLL();
	inline size_t readBits(int &num, size_t bitsCt);

	// Versions with agressive inlining
	// I know that it looks kinda ugly, but I dunno what to do with it :/
	
	SF_FORCE_INLINE bool atEndInline(bool skipBits = 0);
	SF_FORCE_INLINE void skipBytesInline(size_t skipCount);

	SF_FORCE_INLINE byte nextByteInline();

	SF_FORCE_INLINE size_t readBytesInline(byte *bytes, size_t size);
	SF_FORCE_INLINE size_t readBytesRevInline(byte *bytes, size_t size);
	SF_FORCE_INLINE size_t readByteInline(byte &bt);
	SF_FORCE_INLINE size_t readShortInline(unsigned short &sh);
	SF_FORCE_INLINE size_t readIntInline(unsigned int &sh);
	SF_FORCE_INLINE size_t readLLInline(unsigned long long &ll);

	SF_FORCE_INLINE byte readByteInline();
	SF_FORCE_INLINE unsigned short readShortInline();
	SF_FORCE_INLINE unsigned int readIntInline();
	SF_FORCE_INLINE unsigned long long readLLInline();
	
	SF_FORCE_INLINE size_t readBitsInline(int &num, size_t bitsCt);

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

byte SFStreamReader::nextByte()
{
	size_t curPos = bufPos + ((bitPos == 0) ? 0 : 1);
	if (curPos >= bytesRead)
	{
		if (_atEnd())
			return 0;
		if (bytesRead < bufSize)
		{
			bytesRead += _scanNext(buf + bytesRead, bufSize - bytesRead);
		}
		else
		{
			_shiftLeft(max(bufSize / 16, 1u));
		}
		curPos = bufPos + ((bitPos == 0) ? 0 : 1);
	}

	return buf[curPos];
}

size_t SFStreamReader::readBytes(byte *bytes, size_t size)
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

size_t SFStreamReader::readBytesRev(byte *bytes, size_t size)
{
	size_t pos = size - 1;
	for (; ; pos--)
	{
		if (_shouldReadNext() && this->readNext(1) < 0)
			break;
		bytes[pos] = buf[bufPos++];
		if (pos == 0)
			break;
	}
	return size - pos - 1;
}

size_t SFStreamReader::readByte(byte &bt)
{
	return this->readBytesRevInline(&bt, 1);
}

size_t SFStreamReader::readShort(unsigned short &sh)
{
	return this->readBytesRevInline((byte*)&sh, 2);
}

size_t SFStreamReader::readInt(unsigned int &sh)
{
	return this->readBytesRevInline((byte*)&sh, 4);
}

size_t SFStreamReader::readLL(unsigned long long &ll)
{
	return this->readBytesRevInline((byte*)&ll, 8);
}

byte SFStreamReader::readByte()
{
	byte retVal;
	readByteInline(retVal);
	return retVal;
}

unsigned short SFStreamReader::readShort()
{
	unsigned short retVal;
	readShortInline(retVal);
	return retVal;
}

unsigned int SFStreamReader::readInt()
{
	unsigned int retVal;
	readIntInline(retVal);
	return retVal;
}

unsigned long long SFStreamReader::readLL()
{
	unsigned long long retVal;
	readLLInline(retVal);
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


// INLINE PART IS HERE



bool SFStreamReader::atEndInline(bool skipBits)
{
	int cp = bufPos;
	if (skipBits && bitPos > 0)
		cp++;
	if (cp == bytesRead && this->_atEnd())
		return 1;
	return 0;
}

void SFStreamReader::skipBytesInline(size_t skipCount)
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

byte SFStreamReader::nextByteInline()
{
	size_t curPos = bufPos + ((bitPos == 0) ? 0 : 1);
	if (curPos >= bytesRead)
	{
		if (_atEnd())
			return 0;
		if (bytesRead < bufSize)
		{
			bytesRead += _scanNext(buf + bytesRead, bufSize - bytesRead);
		}
		else
		{
			_shiftLeft(max(bufSize / 16, 1u));
		}
		curPos = bufPos + ((bitPos == 0) ? 0 : 1);
	}

	return buf[curPos];
}

size_t SFStreamReader::readBytesInline(byte *bytes, size_t size)
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

size_t SFStreamReader::readBytesRevInline(byte *bytes, size_t size)
{
	size_t pos = size - 1;
	for (; ; pos--)
	{
		if (_shouldReadNext() && this->readNext(1) < 0)
			break;
		bytes[pos] = buf[bufPos++];
		if (pos == 0)
			break;
	}
	return size - pos - 1;
}

size_t SFStreamReader::readByteInline(byte &bt)
{
	return this->readBytesRevInline(&bt, 1);
}

size_t SFStreamReader::readShortInline(unsigned short &sh)
{
	return this->readBytesRevInline((byte*)&sh, 2);
}

size_t SFStreamReader::readIntInline(unsigned int &sh)
{
	return this->readBytesRevInline((byte*)&sh, 4);
}

size_t SFStreamReader::readLLInline(unsigned long long &ll)
{
	return this->readBytesRevInline((byte*)&ll, 8);
}

byte SFStreamReader::readByteInline()
{
	byte retVal;
	readByteInline(retVal);
	return retVal;
}

unsigned short SFStreamReader::readShortInline()
{
	unsigned short retVal;
	readShortInline(retVal);
	return retVal;
}

unsigned int SFStreamReader::readIntInline()
{
	unsigned int retVal;
	readIntInline(retVal);
	return retVal;
}

unsigned long long SFStreamReader::readLLInline()
{
	unsigned long long retVal;
	readLLInline(retVal);
	return retVal;

}

size_t SFStreamReader::readBitsInline(int &num, size_t bitsCt)
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