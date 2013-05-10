#include "stdafx.h"
#include "SFStreamReader.h"

DEFINE_OBJECT_IMPL(SFStreamReader);

bool SFStreamReader::rewind()
{
	bytesRead = 0;
	bufPos = 0;
	bitPos = 0;
	return true;
}

void SFStreamReader::openStream()
{
	super::openStream();
	bytesRead = 0;
}

int SFStreamReader::readNext(bool skipBits, bool forceRead)
{
	if (skipBits)
	{
		if (bitPos  > 0)
		{
			bitPos = 0;
			bufPos++;
		}
	}

	if (bufPos >= bytesRead || forceRead)
	{
		bytesRead = _scanNext(buf, bufSize);
		bufPos = 0;
		if (bytesRead <= 0)
			return -1;
	}
	return 0;
}

bool SFStreamReader::atEnd(bool skipBits)
{
	int cp = bufPos;
	if (skipBits && bitPos > 0)
		cp++;
	if (cp == bytesRead && this->_atEnd())
		return 1;
	return 0;
}

size_t SFStreamReader::readBytes(byte *bytes, size_t size)
{
	size_t pos = 0;
	for (pos = 0; pos < size; pos++)
	{
		if (this->readNext(1) < 0)
			break;
		bytes[pos] = buf[bufPos++];
	}
	return pos;
}

size_t SFStreamReader::readByte(byte &bt)
{
	return this->readBytes(&bt, 1);
}

size_t SFStreamReader::readShort(unsigned short &sh)
{
	return this->readBytes((byte*)&sh, 2);
}

size_t SFStreamReader::readInt(unsigned int &sh)
{
	return this->readBytes((byte*)&sh, 4);
}

size_t SFStreamReader::readLL(unsigned long long &ll)
{
	return this->readBytes((byte*)&ll, 8);
}

size_t SFStreamReader::readBits(int &num, size_t bitsCt)
{
	size_t readBits = 0;
	num = 0;
	while(bitsCt > 0)
	{
		if (readNext(0) < 0)
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

SFStreamReader::~SFStreamReader(void)
{
}
