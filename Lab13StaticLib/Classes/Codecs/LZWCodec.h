#pragma once
#include "GenericCodec.h"

#undef super
#define super GenericCodec

class LZWCodec :
	public super
{
	START_METACLASS_DECLARATION(LZWCodec, super);
	DEFINE_METACLASS_DEFAULT_ALLOC(LZWCodec);
	END_METACLASS_DECLARATION(LZWCodec, super);

public:
	
	static const int defaultCodeLength = 16;

	virtual LZWCodec* initWithStreams(SFStreamReader *sourceStream = NULL, SFStreamWriter *destStream = NULL);
	virtual LZWCodec* initWithFileNames(const wstring &sourceFileName = L"", const wstring &destinationFileName = L"");

	int maxCodeLength;

	virtual void runEncode();
	virtual void runDecode();

	~LZWCodec(void);
};

