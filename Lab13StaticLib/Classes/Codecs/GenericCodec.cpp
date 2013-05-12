#include "stdafx.h"

#include "SFFileStreamReader.h"
#include "SFFileStreamWriter.h"
#include "GenericCodec.h"

DEFINE_OBJECT_IMPL(GenericCodec);

GenericCodec* GenericCodec::initWithFileNames(const wstring &_sourceFileName, const wstring &_destinationFileName)
{
	GenericCodec *rv = this->initWithStreams(NULL, NULL);
	if (rv != NULL)
	{
		rv->reader = NULL;
		rv->writer = NULL;

		rv->setSourceFileName(_sourceFileName);
		rv->setDestinationFileName(_destinationFileName);
	}

	return rv;
}

GenericCodec* GenericCodec::initWithStreams(SFStreamReader *source, SFStreamWriter *destination)
{
	GenericCodec *rv =  static_cast<GenericCodec*>(super::init());
	if (rv != NULL)
	{
		rv->reader = NULL;
		rv->writer = NULL;
		rv->setSourceStream(source);
		rv->setDestinationStream(destination);
	}

	return rv;
}

void GenericCodec::setSourceFileName(const wstring &_sourceFileName)
{
	if (reader != NULL)
	{
		reader->closeStream();
		reader->release();
	}

	reader = SFFileStreamReader::alloc()->initWithFileName(_sourceFileName);
}

void GenericCodec::setDestinationFileName(const wstring &_destinationFileName)
{
	if (writer != NULL)
	{
		writer->closeStream();
		writer->release();
	}

	writer = SFFileStreamWriter::alloc()->initWithFileName(_destinationFileName);
}

void GenericCodec::setSourceStream(SFStreamReader *source)
{
	if (this->reader != NULL)
		this->reader->release();
	this->reader = source;
	if (source != NULL)
		this->reader->retain();
}

void GenericCodec::setDestinationStream(SFStreamWriter *dest)
{
	if (this->writer != NULL)
		this->writer->release();
	this->writer = dest;
	if (dest != NULL)
		this->writer->retain();
}

void GenericCodec::free()
{
	reader->release();
	writer->release();

	reader = NULL;
	writer = NULL;

	super::free();
}