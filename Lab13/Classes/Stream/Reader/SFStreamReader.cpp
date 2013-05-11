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

SFStreamReader::~SFStreamReader(void)
{
}
