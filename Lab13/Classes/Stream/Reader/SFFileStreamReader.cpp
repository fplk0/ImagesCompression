#include "stdafx.h"
#include "SFFileStreamReader.h"

DEFINE_OBJECT_IMPL(self);

SFFileStreamReader* SFFileStreamReader::initWithFileName(wstring _fileName, size_t bufSize)
{
	SFFileStreamReader *retVal = static_cast<SFFileStreamReader*>(this->initWithBufSize(bufSize));
	retVal->fileName = _fileName;
	retVal->file = NULL;
	return retVal;
}

void SFFileStreamReader::setFileName(wstring _fileName)
{
	this->closeStream();
	fileName = _fileName;
}

bool SFFileStreamReader::_rewind(long long bytesCnt)
{
	fseek(file, -(long)bytesCnt, SEEK_CUR);
	return true;
}

size_t SFFileStreamReader::_scanNext(byte *buf, size_t maxSize)
{
	size_t retVal = fread(buf, sizeof(byte), bufSize, file);
	return retVal;
}

bool SFFileStreamReader::_atEnd()
{
	return feof(file) != 0;
}

bool SFFileStreamReader::rewind()
{
	::rewind(file);
	super::rewind();
	return true;
}

size_t SFFileStreamReader::streamSize()
{
	if (file == NULL)
		return 0;
	size_t curPos = ftell(file);
	fseek(file, 0, SEEK_END);
	size_t endPos = ftell(file);
	fseek(file, curPos, SEEK_SET);
	return endPos;
}

void SFFileStreamReader::openStream()
{
	super::openStream();
	
	_wfopen_s(&file, fileName.c_str(), L"rb");
}

void SFFileStreamReader::closeStream()
{
	super::closeStream();
	if (file != NULL)
		fclose(file);
}

SFFileStreamReader::~SFFileStreamReader(void)
{
}
