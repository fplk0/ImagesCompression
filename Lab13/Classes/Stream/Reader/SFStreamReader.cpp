#include "stdafx.h"
#include "SFStreamReader.h"

DEFINE_OBJECT_IMPL(SFStreamReader);

void SFStreamReader::_shiftLeft(size_t cnt)
{
	memcpy(buf, buf + bytesRead - cnt, cnt);
	bufPos = cnt - (bytesRead - bufPos);
	bytesRead = cnt;
	size_t moreRead = _scanNext(buf + bytesRead, bufSize - bytesRead);
	bytesRead += moreRead;
	totalBytesRead += moreRead;
}

bool SFStreamReader::rewind()
{
	bytesRead = 0;
	bufPos = 0;
	bitPos = 0;
	return true;
}

bool SFStreamReader::rewind(long long byteCount)
{
	if (bufPos >= byteCount)
	{
		bufPos -= byteCount;
		return true;
	}
	else //Current position of "caret" in derived class is pos+bytesRead, so we want to rewind it by bytesRead and a bit more than that, to make sure further rewinds will be more optimal
	{
		long long rewNeeded = bytesRead;
		byteCount -= bufPos;
		long long tbr = totalBytesRead - bytesRead;
		if (byteCount > tbr)
			return false;
		
		rewNeeded += byteCount;
		tbr -= byteCount;
		//Okay, now we'll optimize things a bit
		size_t moreRew = bufSize / 2;
		if (moreRew > tbr)
			moreRew = tbr;
		rewNeeded += moreRew;
		tbr -= moreRew;
		if (!this->_rewind(rewNeeded)) //Call to rewind
		{
			return false; //Well, we can't do anything about it
		}
		bufPos = bytesRead; //Invalidate bufPos
		readNext();
		skipBytes(moreRew); //skip extra-rewinded bytes
	}
}

void SFStreamReader::openStream()
{
	super::openStream();
	bytesRead = 0;
	totalBytesRead = 0;
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
		totalBytesRead += bytesRead;
		bufPos = 0;
		if (bytesRead <= 0)
			return -1;
	}
	return 0;
}

SFStreamReader::~SFStreamReader(void)
{
}
