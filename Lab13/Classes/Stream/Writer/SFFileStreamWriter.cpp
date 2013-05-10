#include "stdafx.h"
#include "SFFileStreamWriter.h"

DEFINE_OBJECT_IMPL(self);

SFFileStreamWriter* SFFileStreamWriter::init()
{
	return static_cast<SFFileStreamWriter*>(this->initWithBufSize(bufSize));
}

SFFileStreamWriter* SFFileStreamWriter::initWithFileName(wstring _fileName, size_t bufSize)
{
	SFFileStreamWriter *retVal = this->init();
	retVal->fileName = _fileName;
	retVal->file = NULL;
	return retVal;
}

size_t SFFileStreamWriter::_writeToStream(byte *buf, size_t byteCnt)
{
	size_t writtenBytes = fwrite(buf, sizeof(byte), bufPos, file);
	return writtenBytes;
}

void SFFileStreamWriter::setFileName(wstring _fileName)
{
	this->closeStream();
	fileName = _fileName;
}

void SFFileStreamWriter::openStream()
{
	super::openStream();
	
	_wfopen_s(&file, fileName.c_str(), L"wb");
}

void SFFileStreamWriter::closeStream()
{
	if (file == NULL)
		return;
	super::closeStream();

	if (file != NULL)
		fclose(file);
}

SFFileStreamWriter::~SFFileStreamWriter(void)
{
}
