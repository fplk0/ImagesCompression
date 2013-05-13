#include "stdafx.h"

#include <Windows.h>
#include "dct.h"

#include "ColorSpaceRGB.h"
#include "ColorSpaceYCbCr.h"

#include "SFFileStreamReader.h"

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
	curByte = 0;

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

	reader->readShort(_height);
	reader->readShort(_width);
	width = _width;
	height = _height;

	reader->readByte(_componentsCount);
	componentsCount = _componentsCount;

	//componentsInfo = new ComponentInfo[componentsCount];

	mcuSize = 0;
	mcuBlockWidth = 0;
	mcuBlockHeight = 0;

	int subs = 1;
	for (int i = 0; i < componentsCount; i++)
	{
		byte cb;
		reader->readByte(cb);
		if (cb == 0)
			subs = 0;
		int id = cb-subs;
		componentsInfo[i].id = cb;
		reader->readByte(cb);
		componentsInfo[id].horizontalSubsampling = cb >> 4;
		componentsInfo[id].verticalSubsampling = cb & 0xF;
		mcuBlockWidth = max(mcuBlockWidth, componentsInfo[id].horizontalSubsampling);
		mcuBlockHeight = max(mcuBlockHeight, componentsInfo[id].verticalSubsampling);

		mcuSize += componentsInfo[id].subBlocksPerBlock();

		reader->readByte(cb);
		componentsInfo[id].quantizationTableID = cb;
	}

	mcusPerRow = (width + mcuBlockWidth * 8 - 1) / (mcuBlockWidth * 8);
	mcusPerCol = (height + mcuBlockHeight * 8 - 1) / (mcuBlockHeight * 8);
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
	tableLength -= 2;

	while (tableLength > 0)
	{
		byte sc; 
		reader->readByte(sc);
		tableLength--;
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
		tableLength -= 64 * (valueLen ? 2 : 1);
	}
}

void JPEGCodec::_decodeDHT()
{
	int sectionLength = reader->readShort();
	sectionLength -= 2;

	while (sectionLength > 0)
	{
		byte classID = reader->readByte();
		sectionLength--;
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
		sectionLength -= 16;

		decodeInfo->values = new int[totalCodesCount];

		for (int i = 0; i < totalCodesCount; i++)
		{
			decodeInfo->values[i] = reader->readByte();
			sectionLength--;
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
}

byte JPEGCodec::readBit(int &to)
{
	int &pos = curBitPos;
	if (pos == 0)
	{
		if (curByte == 0xFF)
		{
			if (reader->nextByteInline() == 0)
				reader->skipBytesInline(1);
		}
		curByte = reader->nextByteInline();
		reader->readBitsInline(to, 1);
		if (curByte == 0xFF)
		{
			byte nextByte = reader->nextByteInline();
			if (nextByte != 0)
			{
				//return nextByte;
			}
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
}

void JPEGCodec::_dumpQuantizationTables()
{
	FILE *quantTablesFile = fopen("D:/QuantTablesDump.txt", "a");
	fwprintf(quantTablesFile, L"%s\n", ((SFFileStreamReader*)reader)->getFileName());
	for (int ci = 0; ci < componentsCount; ci++)
	{
		fwprintf(quantTablesFile, L"%d\n", ci);
		for (int i = 0, ctr = 0; i < 64; i++)
		{
			fwprintf(quantTablesFile, L"%d ", quantizationTables[componentsInfo[ci].quantizationTableID][i]);
			ctr++;
			if (ctr == 8)
			{
				ctr = 0;
				fwprintf(quantTablesFile, L"\n");
			}
		}
		fwprintf(quantTablesFile, L"\n\n");
	}
	fwprintf(quantTablesFile, L"\n\n");

	fclose(quantTablesFile);
}

void JPEGCodec::_decodeSOS()
{
	this->_dumpQuantizationTables();
	image = SFImage::alloc()->initWithSizeAndColorSpace(width, height, ColorSpaceRGB::singleton());

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

	int bitPos = 0;

	if (restartInterval != 0)
		singleDecodeBlockSize = restartInterval;
	else
		singleDecodeBlockSize = 1;

	int curMCU = 0;
	decodeBlockInfo = new CurrentDecodeBlockInfo(singleDecodeBlockSize * mcuSize, curMCU);
	int irst = restartInterval;

	int *dcPredictors = new int[componentsCount];
	memset(dcPredictors, 0, sizeof(int) * componentsCount);

	while (true)
	{
		if (curMCU == 40*240+156)
			printf("zz");
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

	const int maxMCUSize = 16;

	int _mcuBuf[3 * maxMCUSize * 64];
	int _mcuConvertedBuf[3 * maxMCUSize * 64];

	int *mcuBuf = _mcuBuf;
	int *mcuConvertedBuf = _mcuConvertedBuf;

	int mcuBlocksSize = mcuBlockHeight * mcuBlockWidth;

	if (mcuBlocksSize > maxMCUSize)
	{
		mcuBuf = new int[3 * mcuBlocksSize * 64];
		mcuConvertedBuf = new int[3 * mcuBlocksSize * 64];
	}

	memset(mcuBuf, 0, 3 * mcuBlocksSize * 64 * sizeof(int));

	if (componentsCount < 3)
	{
		for (int i = 0; i < 64; i++)
			zeroBlock[i] = ColorSpaceYCbCr::singleton()->neutralValueForComponent(1);
	}

	int rowsPerBlock[16], colsPerBlock[16];
	for (int ci = 0; ci < componentsCount; ci++)
	{
		rowsPerBlock[ci] = mcuBlockHeight / componentsInfo[ci].verticalSubsampling;
		colsPerBlock[ci] = mcuBlockWidth / componentsInfo[ci].horizontalSubsampling;
	}

	while (currentBlockStart < blockAddr->pos)
	{
		int smallBlockStart = currentBlockStart;
		int mcuStart = currentBlockStart;

		//Dequantization & IDCT start

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

		//Dequantization & IDCT finish

		int fullMcuIdx = blockAddr->startMCUIndex + mcuIdx;
		int mcuRow = fullMcuIdx / mcusPerRow;
		int mcuCol = fullMcuIdx % mcusPerRow;

		int mcuWidthInPixels = mcuBlockWidth * 8;
		int mcuHeightInPixels = mcuBlockHeight * 8;

		int *currentSubBlocksStart[16];

		ColorSpaceYCbCr *cs = ColorSpaceYCbCr::singleton();

		//De-subsampling start

		for (int ci = 0; ci < componentsCount; ci++)
		{
			int idx = 0;

			for (int i = 0; i < componentsInfo[ci].verticalSubsampling; i++)
				for (int j = 0; j < componentsInfo[ci].horizontalSubsampling; j++)
				{
					int *curBlockStart = subblocksStart[ci] + blockAddr->block + 64 * idx;
					int blockPtr = 0;
					idx++;
					for (int y = 0; y < 8; y++)
					{
						for (int x = 0; x < 8; x++)
						{
							int curVal = curBlockStart[blockPtr];
							
							int tY = (i * 8 + y) * rowsPerBlock[ci];
							int tX = (j * 8 + x) * colsPerBlock[ci];

							for (int _y = 0; _y < rowsPerBlock[ci]; _y++)
							{
								for (int _x = 0; _x < colsPerBlock[ci]; _x++)
								{
									int fY = tY + _y;
									int fX = tX + _x;
									int fullInd = (mcuWidthInPixels * fY + fX) * 3 + ci;
									mcuBuf[fullInd] = curVal;
								}
							}



							blockPtr++;
						}
					}
				}
		}

		//De-subsampling finish

		cs->convertImageToRGB(mcuBuf, mcuConvertedBuf, mcuHeightInPixels * mcuWidthInPixels);

		int mcuXPixel = mcuCol * mcuBlockWidth * 8;
		int mcuYPixel = mcuRow * mcuBlockHeight * 8;
		int posInBuf = 0;

		for (int i = 0; i < mcuHeightInPixels; i++)
		{
			for (int j = 0; j < mcuWidthInPixels; j++)
			{
				for (int ci = 0; ci < 3; ci++)
				{
					int val = mcuConvertedBuf[posInBuf++];
					if (i + mcuYPixel >= image->getHeight())
						continue;
					if (j + mcuXPixel >= image->getWidth())
						continue;
					image->setPixel(j + mcuXPixel, i + mcuYPixel, ci, val);
				}
			}
		}

		mcuIdx++;
		currentBlockStart += mcuSize;
	}

	if (mcuBlocksSize > maxMCUSize)
	{
		delete [] mcuBuf;
		delete [] mcuConvertedBuf;
	}
}

void JPEGCodec::_cleanupDecode()
{

}