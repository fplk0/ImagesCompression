#pragma once
#include "sfstreamreader.h"
#include "SFData.h"

#undef super
#undef self

#define super SFStreamReader
#define self SFMemoryStreamReader

class SFMemoryStreamReader :
	public super
{
	DEFAULT_METACLASS_DECLARATION(self, super);

	SFData *data;
	size_t posInData;

	virtual size_t _scanNext(byte *buf, size_t maxSize);
	virtual bool _atEnd();

public:
	virtual SFMemoryStreamReader* initWithData(SFData *_data);

	virtual bool rewind();
	virtual void openStream();

	virtual void free();

	virtual size_t streamSize();

	virtual ~SFMemoryStreamReader(void);
};

