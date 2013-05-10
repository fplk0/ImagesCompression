#include "stdafx.h"

#include "SFFileStreamReader.h"
#include "SFFileStreamWriter.h"
#include "GenericEncoder.h"

DEFINE_OBJECT_IMPL(GenericEncoder);

GenericEncoder* GenericEncoder::initWithFileNames(const wstring &_sourceFileName, const wstring &_destinationFileName)
{
	GenericEncoder *rv = this->initWithStreams(NULL, NULL);
	if (rv != NULL)
	{
		rv->reader = NULL;
		rv->writer = NULL;

		rv->setSourceFileName(_sourceFileName);
		rv->setDestinationFileName(_destinationFileName);
	}

	return rv;
}

GenericEncoder* GenericEncoder::initWithStreams(SFStreamReader *source, SFStreamWriter *destination)
{
	GenericEncoder *rv =  static_cast<GenericEncoder*>(super::init());
	if (rv != NULL)
	{
		rv->reader = NULL;
		rv->writer = NULL;
		rv->setSourceStream(source);
		rv->setDestinationStream(destination);
	}

	return rv;
}

void GenericEncoder::setSourceFileName(const wstring &_sourceFileName)
{
	if (reader != NULL)
	{
		reader->closeStream();
		reader->release();
	}

	reader = SFFileStreamReader::alloc()->initWithFileName(_sourceFileName);
}

void GenericEncoder::setDestinationFileName(const wstring &_destinationFileName)
{
	if (writer != NULL)
	{
		writer->closeStream();
		writer->release();
	}

	writer = SFFileStreamWriter::alloc()->initWithFileName(_destinationFileName);
}

void GenericEncoder::setSourceStream(SFStreamReader *source)
{
	if (this->reader != NULL)
		this->reader->release();
	this->reader = source;
	if (source != NULL)
		this->reader->retain();
}

void GenericEncoder::setDestinationStream(SFStreamWriter *dest)
{
	if (this->writer != NULL)
		this->writer->release();
	this->writer = dest;
	if (dest != NULL)
		this->writer->retain();
}

void GenericEncoder::free()
{
	reader->release();
	writer->release();

	reader = NULL;
	writer = NULL;

	super::free();
}