#pragma once
#include "sfstreamwriter.h"

#undef super
#undef self

#define super SFStreamWriter
#define self SFFileStreamWriter

class SFFileStreamWriter :
	public SFStreamWriter
{
	START_METACLASS_DECLARATION(self, super);
	DEFINE_METACLASS_DEFAULT_ALLOC(self);
	END_METACLASS_DECLARATION(self, super);

	FILE *file;
	wstring fileName;

	virtual size_t _writeToStream(byte *buf, size_t byteCnt);

public:

	void setFileName(wstring _fileName);
	virtual SFFileStreamWriter* init();
	virtual SFFileStreamWriter* initWithFileName(wstring _fileName = L"", size_t bufSize = 1 << 14);

	virtual void openStream();
	virtual void closeStream();

	virtual ~SFFileStreamWriter(void);
};

