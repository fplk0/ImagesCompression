#pragma once

#include "SFObject.h"
#include "SFStreamReader.h"
#include "SFStreamWriter.h"

#undef super
#undef self

#define super SFObject

class GenericCodec : public SFObject
{
	START_METACLASS_DECLARATION(GenericCodec, super);
	DEFINE_METACLASS_ABSTRACT_ALLOC(GenericCodec);
	END_METACLASS_DECLARATION(GenericCodec, super);

protected:
	SFStreamReader *reader;
	SFStreamWriter *writer;

	virtual void free();
public:
	virtual GenericCodec* initWithFileNames(const wstring &sourceFileName = L"", const wstring &destinationFileName = L"");
	virtual GenericCodec* initWithStreams(SFStreamReader *source, SFStreamWriter *destination);

	void setSourceFileName(const wstring &_sourceFileName);
	void setDestinationFileName(const wstring &_destinationFileName);

	void setSourceStream(SFStreamReader *source);
	void setDestinationStream(SFStreamWriter *dest);

	virtual void runEncode() = 0;
	virtual void runDecode() = 0;
};
