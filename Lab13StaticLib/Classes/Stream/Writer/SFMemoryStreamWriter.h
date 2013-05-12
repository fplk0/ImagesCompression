#pragma once
#include "SFData.h"
#include "sfstreamwriter.h"

#undef super
#undef self

#define super SFStreamWriter
#define self SFMemoryStreamWriter

class SFMemoryStreamWriter :
	public super
{
	DEFAULT_METACLASS_DECLARATION(self, super);

	SFData *data;
	bool _dynamic;

	size_t posInData;
	virtual size_t _writeToStream(byte *buf, size_t byteCnt);

public:

	virtual SFMemoryStreamWriter* initDynamic(size_t startSize = 1024);
	virtual SFMemoryStreamWriter* initWithData(SFData *outData);

	SFData* getData();

	virtual void openStream();
	virtual void closeStream();

	virtual void free();

	virtual ~SFMemoryStreamWriter(void);
};

