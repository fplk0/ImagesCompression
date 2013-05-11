#pragma once
#include "SFStreamReader.h"

#undef super
#undef self

#define super SFStreamReader
#define self SFFileStreamReader

class SFFileStreamReader :
	public super
{
	START_METACLASS_DECLARATION(self, super);
	DEFINE_METACLASS_DEFAULT_ALLOC(self);
	END_METACLASS_DECLARATION(self, super);

	FILE *file;
	wstring fileName;

	virtual size_t _scanNext(byte *buf, size_t maxSize);
	virtual bool _atEnd();
	virtual bool _rewind(long long byteCnt);

public:
	
	void setFileName(wstring _fileName);
	virtual SFFileStreamReader* initWithFileName(wstring _fileName = L"", size_t bufSize = 1 << 14);

	virtual bool rewind();

	virtual size_t streamSize();

	virtual void openStream();
	virtual void closeStream();

	virtual ~SFFileStreamReader(void);
};

