#pragma once

#include "Object.h"
#include "SFStreamReader.h"
#include "SFStreamWriter.h"

#undef super
#undef self

#define super Object

class GenericEncoder : public Object
{
	START_METACLASS_DECLARATION(GenericEncoder, super);
	DEFINE_METACLASS_ABSTRACT_ALLOC(GenericEncoder);
	END_METACLASS_DECLARATION(GenericEncoder, super);

protected:
	SFStreamReader *reader;
	SFStreamWriter *writer;

	virtual void free();
public:
	virtual GenericEncoder* initWithFileNames(const wstring &sourceFileName = L"", const wstring &destinationFileName = L"");
	virtual GenericEncoder* initWithStreams(SFStreamReader *source, SFStreamWriter *destination);

	void setSourceFileName(const wstring &_sourceFileName);
	void setDestinationFileName(const wstring &_destinationFileName);

	void setSourceStream(SFStreamReader *source);
	void setDestinationStream(SFStreamWriter *dest);

	virtual void runEncode() = 0;
	virtual void runDecode() = 0;
};
