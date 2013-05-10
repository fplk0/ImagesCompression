#pragma once
#include "genericencoder.h"

#undef super
#define super GenericEncoder

class LZWEncoder :
	public super
{
	START_METACLASS_DECLARATION(LZWEncoder, super);
	DEFINE_METACLASS_DEFAULT_ALLOC(LZWEncoder);
	END_METACLASS_DECLARATION(LZWEncoder, super);

public:
	
	static const int defaultCodeLength = 16;

	virtual LZWEncoder* initWithFileNames(const wstring &sourceFileName = L"", const wstring &destinationFileName = L"");

	int maxCodeLength;

	virtual void runEncode();
	virtual void runDecode();

	~LZWEncoder(void);
};

