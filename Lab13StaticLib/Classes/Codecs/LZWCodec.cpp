#include "stdafx.h"
#include "LZWCodec.h"

#include <vector>
using namespace std;

DEFINE_OBJECT_IMPL(LZWCodec);

LZWCodec* LZWCodec::initWithFileNames(const wstring &sourceFileName, const wstring &destinationFileName)
{
	LZWCodec *rv = static_cast<LZWCodec*>(super::initWithFileNames(sourceFileName, destinationFileName));
	rv->maxCodeLength = defaultCodeLength;
	return rv;
}

void LZWCodec::runEncode()
{
	reader->openStream();
	writer->openStream();

	size_t codeLen = 1 << (maxCodeLength + 8);
	int *codes = new int[codeLen];
	memset(codes, -1, sizeof(int) * codeLen);

	int maxCode = 1 << maxCodeLength;
	
	int *writtenCodes = new int[maxCode];
	int wc = 0;

	byte nextByte;
	reader->readByte(nextByte);
	int curCode = nextByte;

	int lastCode = 256;
	int bitLength = 8;

	while (!reader->atEnd(false))
	{
		reader->readByte(nextByte);
		int entry = (curCode << 8) | nextByte;
		if (codes[entry] == -1)
		{
			writer->writeBits(curCode, bitLength);
			if (lastCode < maxCode)
			{
				if ((lastCode & (-lastCode)) == lastCode)
				{
					bitLength++;
				}
				writtenCodes[wc++] = entry;
				codes[entry] = lastCode++;
			}
			else if (lastCode == maxCode)
			{
				for (int i = 0; i < wc; i++)
				{
					codes[writtenCodes[i]] = -1;
				}

				wc = 0;
				bitLength = 8;
				lastCode = 256;
			}
			curCode = nextByte;
		}
		else
		{
			curCode = codes[entry];
		}
	}

	writer->writeBits(curCode, bitLength);

	reader->closeStream();
	writer->closeStream();

	delete [] writtenCodes;
	delete [] codes;
}

void LZWCodec::runDecode()
{
	reader->openStream();
	writer->openStream();

	size_t codeLen = 1 << maxCodeLength;
	int *codes = new int[codeLen];
	memset(codes, -1, sizeof(int) * codeLen);

	int maxCode = 1 << maxCodeLength;

	int bitLength = 8;

	for (int i = 0; i < 256; i++)
	{
		codes[i] = (maxCode << 8) | i;
	}

	if (reader->atEnd(false))
	{
		delete [] codes;
		return;
	}

	int codesCount = 256;
	int curCode;

	vector<byte> codesStack;

	while (!reader->atEnd(false))
	{
		size_t bitsRead = reader->readBits(curCode, bitLength);

		if (bitsRead != bitLength)
			break;
		int tmp = curCode;
		while (tmp != maxCode)
		{
			byte cb = codes[tmp] & 0xFF;
			int pc = codes[tmp] >> 8;
			codesStack.push_back(cb);
			tmp = pc;
		}
		
		if (codesCount - 1 > 255)
			codes[codesCount-1] |= codesStack.back();

		if (codesCount < maxCode)
		{
			codes[codesCount] = curCode << 8;
		}

		if ((codesCount & (-codesCount)) == codesCount && codesCount < maxCode)
		{
			bitLength++;
		}
		codesCount++;

		codesStack[0] = codes[curCode] & 0xFF;

		if(codesCount-1 == maxCode)
		{
			bitLength = 8;
			codesCount = 256;
		}

		while (!codesStack.empty())
		{
			writer->writeByte(codesStack.back());
			codesStack.pop_back();
		}
	}

	reader->closeStream();
	writer->closeStream();

	delete [] codes;
}

LZWCodec::~LZWCodec(void)
{
}
