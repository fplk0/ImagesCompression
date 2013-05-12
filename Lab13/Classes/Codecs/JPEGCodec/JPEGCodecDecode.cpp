#include "stdafx.h"

#include <Windows.h>
#include "dct.h"

#include "ColorSpaceRGB.h"
#include "ColorSpaceYCbCr.h"
#include "JPEGCodec.h"

void JPEGCodec::runDecode()
{
	initDCT();
	
	if (image != NULL)
	{
		image->release();
		image = NULL;
	}

	reader->openStream();
	restartInterval = 0;
	curBitPos = 0;
	curByteRead = 0;

	memset(quantizationTables, 0, sizeof(quantizationTables));

	while (!reader->atEnd(true))
	{
		byte nextByte;

		reader->readByte(nextByte);
		if (nextByte != 0xFF || reader->atEnd(true))
			break;

		reader->readByte(nextByte);

		int sectionSize;
		switch (nextByte)
		{
		case JPEGSectionSOI:
			break;
		case JPEGSectionDHT:
			this->_decodeDHT();
			break;
		case JPEGSectionDQT:
			this->_decodeDQT();
			break;
		case JPEGSectionSOF0:
			this->_decodeSOF0();
			break;
		case JPEGSectionSOF2:
			//TODO: some error handling here
			break;
		case JPEGSectionCOM:
			this->_decodeComment();
			break;
		case JPEGSectionSOS:
			this->_decodeSOS();
			break;
		default: //Some unknown section found; read it's size and skip it
			{
				unsigned short currentSectionSize;
				reader->readShort(currentSectionSize);
				reader->skipBytes(currentSectionSize - 2);
			}
			break;
		}
	}
}

void JPEGCodec::_decodeSOF0()
{
	unsigned short sectionLength;
	reader->readShort(sectionLength);

	unsigned short _width, _height;
	byte _precision;
	byte _componentsCount;

	reader->readByte(_precision);
	precison = _precision;

	reader->readShort(_width);
	reader->readShort(_height);
	width = _width;
	height = _height;

	reader->readByte(_componentsCount);
	componentsCount = _componentsCount;

	//componentsInfo = new ComponentInfo[componentsCount];

	mcuSize = 0;
	mcuWidth = 0;
	mcuHeight = 0;

	for (int i = 0; i < componentsCount; i++)
	{
		byte cb;
		reader->readByte(cb);
		int id = cb-1;
		componentsInfo[i].id = cb;
		reader->readByte(cb);
		componentsInfo[id].horizontalSubsampling = cb >> 4;
		componentsInfo[id].verticalSubsampling = cb & 0xF;
		mcuWidth = max(mcuWidth, componentsInfo[id].horizontalSubsampling);
		mcuHeight = max(mcuHeight, componentsInfo[id].verticalSubsampling);

		mcuSize += componentsInfo[id].subBlocksPerBlock();

		reader->readByte(cb);
		componentsInfo[id].quantizationTableID = cb;
	}

	mcusPerRow = (width + mcuWidth * 8 - 1) / (mcuWidth * 8);
	mcuSize *= 64;
}

void JPEGCodec::_decodeComment()
{
	unsigned short commentLength;
	reader->readShort(commentLength);
	commentLength -= 2;

	char *commentBuffer = new char[commentLength + 2];
	reader->readBytes((byte*)commentBuffer, commentLength);

	wchar_t *wcharBuffer = new wchar_t[commentLength + 2];
	int charCount = MultiByteToWideChar(CP_UTF8, 0, commentBuffer, commentLength, wcharBuffer, commentLength+2);
	wcharBuffer[charCount] = 0;

	comment = wstring(wcharBuffer);

	delete [] wcharBuffer;
	delete [] commentBuffer;
	//MultiByteToWideChar
}

void JPEGCodec::_decodeDQT()
{
	unsigned short tableLength;
	reader->readShort(tableLength);
	tableLength -= 4;

	byte sc; 
	reader->readByte(sc);
	int valueLen = sc >> 4;
	int tableID = sc & 0xF;

	int *buf = NULL;
	if (quantizationTables[tableID] != NULL)
	{
		buf = quantizationTables[tableID];
	}
	else
	{
		buf = new int[64];
		quantizationTables[tableID] = buf;
	}

	for (int i = 0; i < 64; i++)
	{
		if (valueLen == 0)
		{
			byte next;
			reader->readByte(next);
			buf[_zig[i]] = next;
		}
		else
		{
			unsigned short next;
			reader->readShort(next);
			buf[_zig[i]] = next;
		}
	}
}

void JPEGCodec::_decodeDHT()
{
	int sectionLength = reader->readShort();
	byte classID = reader->readByte();
	int tableClass = (classID >> 4) & 0xF;
	int tableID = classID & 0xF;

	HuffmanDecodeInfo *decodeInfo;
	if (tableClass == 0)
		decodeInfo = &dcHuffmanDecodeTables[tableID];
	else
		decodeInfo = &acHuffmanDecodeTables[tableID];

	int codesCount[16];
	int totalCodesCount = 0;

	for (int i = 0; i < 16; i++)
	{
		codesCount[i] = reader->readByte();
		decodeInfo->valPtr[i] = totalCodesCount;
		totalCodesCount += codesCount[i];
	}

	decodeInfo->values = new int[totalCodesCount];

	for (int i = 0; i < totalCodesCount; i++)
	{
		decodeInfo->values[i] = reader->readByte();
	}

	int lastPrevCode = 0;

	for (int i = 0; i < 16; i++)
	{
		if (codesCount[i] != 0)
		{
			decodeInfo->minCode[i] = lastPrevCode;
			decodeInfo->maxCode[i] = lastPrevCode + codesCount[i] - 1;
			lastPrevCode = decodeInfo->maxCode[i] + 1;
		}
		else
		{
			decodeInfo->maxCode[i] = -1;
		}
		lastPrevCode <<= 1;
	}
}

byte JPEGCodec::readBit(int &to)
{
	int &pos = curBitPos;
	if (pos == 0)
	{
		if (curByteRead == 0xFF)
		{
			reader->skipBytesInline(1);
		}
		curByteRead = reader->nextByteInline();
		reader->readBitsInline(to, 1);
		if (curByteRead == 0xFF)
		{
			byte nextByte = reader->nextByteInline();
			if (nextByte != 0)
				return nextByte;
		}
		pos = 7;
	}
	else
	{
		reader->readBitsInline(to, 1);
		pos--;
	}
	return 0;
}

void JPEGCodec::_readNum(int &to, int len)
{
	int bs;
	int fb = 1;
	int val = 0;

	for (int i = 0; i < len; i++)
	{
		readBit(bs);
		if (i == 0)
			fb = bs;
		if (bs)
			val += 1 << (len - i - 1);
	}

	if (fb == 0)
		val = - ((1 << len) - 1 - val);
	to = val;
}

int JPEGCodec::_scanDCCoef(int compNum, int prevVal)
{
	int c = 0;
	readBit(c);
	int huffLen = 1;
	int huffID = componentsInfo[compNum].quantizationTableID;

	HuffmanDecodeInfo &curHuff = dcHuffmanDecodeTables[huffID];

	while(c > curHuff.maxCode[huffLen -1])
	{
		int nextBit;
		readBit(nextBit);
		c = c*2 + nextBit;
		huffLen ++;
	}

	int curInd = curHuff.valPtr[huffLen-1] + c - curHuff.minCode[huffLen-1];
	int len = curHuff.values[curInd];

	int curCoef;
	_readNum(curCoef, len);
	decodeBlockInfo->writeNext(curCoef + prevVal);
	return curCoef + prevVal;

}

int JPEGCodec::_scanACCoef(int compNum)
{
	int huffID = componentsInfo[compNum].quantizationTableID;

	HuffmanDecodeInfo &curHuff = acHuffmanDecodeTables[huffID];

	for (int i = 0; i < 63; i++)
	{
		int c = 0;
		readBit(c);
		int huffLen = 1;

		while(c > curHuff.maxCode[huffLen -1])
		{
			int nextBit;
			readBit(nextBit);
			c = c*2 + nextBit;
			huffLen++;
		}

		int curInd = curHuff.valPtr[huffLen-1] + c - curHuff.minCode[huffLen-1];
		int curHuffByte = curHuff.values[curInd];
		int zeroCount = (curHuffByte >> 4) & 0xF;
		if (curHuffByte == 0)
			zeroCount = 63 - i;

		for (int j = 0; j < zeroCount; j++, i++)
			decodeBlockInfo->writeNext(0);

		if (curHuffByte == 0)
			break;

		int len = curHuffByte & 0xF;
		int curACCoef;
		_readNum(curACCoef, len);
		decodeBlockInfo->writeNext(curACCoef);
	}
	return 0;
}

void JPEGCodec::_decodeMCU(int *dcPredictors)
{
	for (int i = 0; i < componentsCount; i++)
	{
		for (int j = 0, jEnd = componentsInfo[i].subBlocksPerBlock(); j < jEnd; j++)
		{
			dcPredictors[i] = _scanDCCoef(i, dcPredictors[i]);
			_scanACCoef(i);
			decodeBlockInfo->nextBlock();
		}
	}

	//curBitPos = 0;
	//reader->padLastByteBits();
}

void JPEGCodec::_decodeSOS()
{
	image = Image::alloc()->initWithSizeAndColorSpace(width, height, ColorSpaceRGB::singleton());

	int headerLength = reader->readShort();

	int scanComponentsCount = reader->readByte();

	int componentNumbers[16];
	int huffmanTablesDC[16];
	int huffmanTablesAC[16];

	for (int i = 0; i < scanComponentsCount; i++)
	{
		componentNumbers[i] = reader->readByte();
		byte huffmanIDS = reader->readByte();
		byte huffmanIDDC = huffmanIDS >> 4;
		byte huffmanIDAC = huffmanIDS & 0xF;
		huffmanTablesDC[i] = huffmanIDDC;
		huffmanTablesAC[i] = huffmanIDDC | 0x10;
	}

	int scannedBytes = 3 + scanComponentsCount * 2;

	reader->skipBytes(headerLength - scannedBytes); // Skip some unneeded bytes, if we have some

	byte curByte = reader->nextByte();
	int bitPos = 0;

	if (restartInterval != 0)
		singleDecodeBlockSize = restartInterval;
	else
		singleDecodeBlockSize = 4;

	int curMCU = 0;
	decodeBlockInfo = new CurrentDecodeBlockInfo(singleDecodeBlockSize * mcuSize, curMCU);
	int irst = restartInterval;

	int *dcPredictors = new int[componentsCount];
	memset(dcPredictors, 0, sizeof(int) * componentsCount);

	while (true)
	{
		this->_decodeMCU(dcPredictors);
		curMCU++;
		if (restartInterval)
		{
			irst--;
			if (irst == 0)
			{
				irst = restartInterval;
				memset(dcPredictors, 0, sizeof(int) * componentsCount);
				reader->skipBytesInline(2);
			}
		}

		if ((curMCU & (singleDecodeBlockSize - 1)) == 0)
		{
			_decodeSingleBlock(decodeBlockInfo);
			decodeBlockInfo = new CurrentDecodeBlockInfo(singleDecodeBlockSize * mcuSize, curMCU);
		}

		byte nb = reader->nextByteInline();
		if (nb == 0xFF)
		{
			reader->skipBytesInline(1);
			nb = reader->nextByteInline();
			if (nb == 0xD9)
			{
				reader->padLastByteBits();
				reader->skipBytes(1);
				printf("%d\n", curMCU);
				break;
			}
			else
				reader->rewind(1);
		}
	}

	_decodeSingleBlock(decodeBlockInfo);
}

static int zeroBlock[64] = { 0 };

void JPEGCodec::_decodeSingleBlock(CurrentDecodeBlockInfo *blockAddr)
{
	int currentBlockStart = 0;
	int mcuIdx = 0;
	int subblocksStart[16];

	if (componentsCount < 3)
	{
		for (int i = 0; i < 64; i++)
			zeroBlock[i] = ColorSpaceYCbCr::singleton()->neutralValueForComponent(1);
	}

	while (currentBlockStart < blockAddr->pos)
	{
		int smallBlockStart = currentBlockStart;
		int mcuStart = currentBlockStart;

		for (int compIdx = 0; compIdx < componentsCount; compIdx++)
		{
			ComponentInfo &curCompInfo = componentsInfo[compIdx];
			int *quantizationTable = quantizationTables[curCompInfo.quantizationTableID];
			subblocksStart[compIdx] = smallBlockStart;

			for (int subblockIdx = 0, subblockIdxEnd = curCompInfo.subBlocksPerBlock(); subblockIdx < subblockIdxEnd; subblockIdx++)
			{
				int *block = blockAddr->block + smallBlockStart;
				for (int k = 0; k < 64; k++)
				{
					block[k] *= quantizationTable[k];
				}

				float srcBuf[8], dstBuf[8];
				float intermBuf[64];

				for (int i = 0; i < 8; i++)
				{
					for (int j = 0; j < 8; j++)
					{
						srcBuf[j] = (float)block[j*8+i];
					}
					perform1DIDCT(srcBuf, dstBuf);
					for (int j = 0; j < 8; j++)
					{
						intermBuf[j*8+i] = dstBuf[j];
					}
				}

				for (int j = 0; j < 8; j++)
				{
					for (int i = 0; i < 8; i++)
					{
						srcBuf[i] = intermBuf[j*8+i];
					}
					perform1DIDCT(srcBuf, dstBuf);
					for (int i = 0; i < 8; i++)
						block[j*8+i] = dstBuf[i];
				}

				smallBlockStart += 64;
			}
		}

		int fullMcuIdx = blockAddr->startMCUIndex + mcuIdx;
		int mcuRow = fullMcuIdx / mcusPerRow;
		int mcuCol = fullMcuIdx % mcusPerRow;

		int *currentSubBlocksStart[16];

		ColorSpaceYCbCr *cs = ColorSpaceYCbCr::singleton();

		for (int compIdx = 0; compIdx < cs->getComponentsCount(); compIdx++)
			currentSubBlocksStart[compIdx] = zeroBlock;

		int rowsPerBlock[16], colsPerBlock[16];

		for (int i = 0; i < componentsCount; i++)
		{
			rowsPerBlock[i] = mcuHeight / componentsInfo[i].verticalSubsampling;
			colsPerBlock[i] = mcuWidth / componentsInfo[i].horizontalSubsampling;
		}

		int mcuXPixel = mcuCol * mcuWidth * 8;
		int mcuYPixel = mcuRow * mcuHeight * 8;

		for (int i = 0; i < mcuHeight; i++)
		{
			for (int j = 0; j < mcuWidth; j++)
			{
				for (int ci = 0; ci < componentsCount; ci++)
				{
					int blockNum = (i / rowsPerBlock[ci]) * componentsInfo[ci].horizontalSubsampling + j / colsPerBlock[ci];
					currentSubBlocksStart[ci] = subblocksStart[ci] + blockAddr->block + blockNum * 64;
				}

				int rgbBuf[64 * 3];

				cs->convertImageToRGB(currentSubBlocksStart, rgbBuf, 64);

				int posInBuf = 0;
				int blockXPixel = mcuXPixel + j * 8;
				int blockYPixel = mcuYPixel + i * 8;

				for (int y = 0; y < 8; y++)
				{
					for (int x = 0; x < 8; x++)
					{
						for (int k = 0; k < 3; k++)
						{
							image->setPixel(x + blockXPixel, y + blockYPixel, k, rgbBuf[posInBuf++]);
						}
					}
				}
			}
		}

		mcuIdx++;
		currentBlockStart += mcuSize;
	}
}

void JPEGCodec::_cleanupDecode()
{

}