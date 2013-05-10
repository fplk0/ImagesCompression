#pragma once
#include "SFStream.h"

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

	virtual size_t _scanNext(byte *buf, size_t maxSize) { return 0; };
	virtual bool _atEnd() { return true; };

public:

	virtual bool rewind();
	virtual void openStream();

	bool atEnd(bool skipBits = 0);
	size_t readBytes(byte *bytes, size_t size);
	size_t readByte(byte &bt);
	size_t readShort(unsigned short &sh);
	size_t readInt(unsigned int &sh);
	size_t readLL(unsigned long long &ll);
	size_t readBits(int &num, size_t bitsCt);

	virtual size_t streamSize() = 0;

	virtual ~SFStreamReader(void);
};

