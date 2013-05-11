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

	int _flushStreamIfNeeded(bool flushBits = 1, bool forceFlush = 0);
	virtual size_t _writeToStream(byte *buf, size_t byteCnt) = 0;

public:

	virtual SFStreamWriter* init();
	int flushStream(bool flushBits = 1);

	int writeBytes(byte *bytes, size_t size, bool reverse = 0);
	int writeByte(byte bt);
	int writeShort(unsigned short sh);
	int writeInt(unsigned int val);
	int writeLL(unsigned long long val);
	int writeBits(int num, size_t bitsCt);

	virtual void closeStream();

	virtual ~SFStreamWriter(void);
};

