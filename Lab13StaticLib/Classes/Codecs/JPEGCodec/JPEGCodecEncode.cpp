#include "stdafx.h"

#include "ColorSpaceYCbCr.h"
#include "dct.h"
#include <algorithm>

#include <Windows.h>
#include <ctime>

#include "JPEGCodec.h"

int JPEGCodec::defaultQuantizationTable[2][64] = 
{
	{ 
		16, 11, 10, 16, 24, 40, 51, 61, 
		12, 12, 14, 19, 26, 58, 60, 55, 
		14, 13, 16, 24, 40, 57, 69, 56, 
		14, 17, 22, 29, 51, 87, 80, 62, 
		18, 22, 37, 56, 68, 109, 103, 77, 
		24, 35, 55, 64, 81, 104, 113, 92, 
		49, 64, 78, 87, 103, 121, 120, 101, 
		72, 92, 95, 98, 112, 100, 103, 99
	}, 
	{
		17, 18, 24, 47, 99, 99, 99, 99, 
		18, 21, 26, 66, 99, 99, 99, 99, 
		24, 26, 56, 99, 99, 99, 99, 99, 
		47, 66, 99, 99, 99, 99, 99, 99, 
		99, 99, 99, 99, 99, 99, 99, 99, 
		99, 99, 99, 99, 99, 99, 99, 99, 
		99, 99, 99, 99, 99, 99, 99, 99, 
		99, 99, 99, 99, 99, 99, 99, 99 
	}
};

void JPEGCodec::setHorizontalSubsamplingForComponent(int horizontalSubsampling, int component)
{
	componentsInfo[component].horizontalSubsampling = horizontalSubsampling;
}

void JPEGCodec::setVerticalSubsamplingForComponent(int verticalSubsampling, int component)
{
	componentsInfo[component].verticalSubsampling = verticalSubsampling;
}

int JPEGCodec::getHorizontalSubsamplingForComponent(int component)
{
	return componentsInfo[component].horizontalSubsampling;
}

int JPEGCodec::getVerticalSubsamplingForComponent(int component)
{
	return componentsInfo[component].verticalSubsampling;
}

void JPEGCodec::runEncode()
{
	initDCT();
	_prepareQuantizationMatrix();
	componentsCount = 3;
	taskScheduler = SFTaskScheduler::singleInstance();

	width = image->getWidth();
	height = image->getHeight();

	int maxHorizontalSubsampling = 0, maxVerticalSubsampling = 0;
	mcuSize = 0;
	for (int i = 0; i < componentsCount; i++)
	{
		maxHorizontalSubsampling = max(maxHorizontalSubsampling, componentsInfo[i].horizontalSubsampling);
		maxVerticalSubsampling = max(maxVerticalSubsampling, componentsInfo[i].verticalSubsampling);
		componentsInfo[i].id = i + 1;
		mcuSize += componentsInfo[i].horizontalSubsampling * componentsInfo[i].verticalSubsampling;
	}

	mcuBlockSize = mcuSize;

	mcuSize *= 64;

	componentsInfo[0].quantizationTableID = 0;
	componentsInfo[1].quantizationTableID = 1;
	componentsInfo[2].quantizationTableID = 1;

	mcuBlockWidth = maxHorizontalSubsampling;
	mcuBlockHeight = maxVerticalSubsampling;
	mcuWidth = mcuBlockWidth * 8;
	mcuHeight = mcuBlockHeight * 8;


	mcusPerRow = (width + mcuBlockWidth * 8 - 1) / (mcuBlockWidth * 8);
	mcusPerCol = (height + mcuBlockHeight * 8 - 1) / (mcuBlockHeight * 8);

	int totalMCUs = mcusPerCol * mcusPerRow;

	encodedBlocks = new int[mcuSize * totalMCUs];

	int mcusPerBlock = 128;

	taskIndex = SFTaskScheduler::singleInstance()->nextTasksType();

	clock_t encodeStart = clock();

	for (int i = 0, iEnd = totalMCUs / mcusPerBlock; i < iEnd; i++)
	{
		_encodeSingleBlock(i * mcusPerBlock, mcusPerBlock);
	}

	if (totalMCUs % mcusPerBlock != 0)
	{
		_encodeSingleBlock(totalMCUs - totalMCUs % mcusPerBlock, totalMCUs % mcusPerBlock);
	}

	taskScheduler->waitForTaskFinish(taskIndex);

	printf("Blocks encode time: %lf\n", (clock() - encodeStart) / (double)CLOCKS_PER_SEC);

	_prepareHuffmanTables();

	writer->openStream();

	writer->writeByte(0xFF);
	writer->writeByte(JPEGSectionSOI);

	_encodeComment();
	_encodeSOF0();
	_encodeDQT();
	_encodeDHT();
	_encodeSOS();

	writer->closeStream();
	_cleanupEncode();
}

void JPEGCodec::_prepareQuantizationMatrix()
{
	quantizationTables[0] = new int[64];
	quantizationTables[1] = new int[64];

	double mulCoef = 1;
	if (encodeQuality > 100)
		encodeQuality = 100;
	if (encodeQuality < 1)
		encodeQuality = 1;

	if (encodeQuality >= 50)
	{
		mulCoef = (100 - encodeQuality) / 50.0;
	}
	else
		mulCoef = 50.0 / encodeQuality;

	for (int i = 0; i < 64; i++)
	{
		quantizationTables[0][i] = (int)(defaultQuantizationTable[0][i] * mulCoef);
		if (quantizationTables[0][i] < 1)
			quantizationTables[0][i] = 1;
		if (quantizationTables[0][i] > 255)
			quantizationTables[0][i] = 255;

		quantizationTables[1][i] = (int)(defaultQuantizationTable[1][i] * mulCoef);

		if (quantizationTables[1][i] < 1)
			quantizationTables[1][i] = 1;
		if (quantizationTables[1][i] > 255)
			quantizationTables[1][i] = 255;
	}
}

int JPEGCodec::lengthOfNum(int num)
{
	if (num < 0)
		num = -num;
	int i = 0, len = 0;
	while (num > i)
	{
		len++;
		i <<= 1;
		i++;
	}
	return len;
}

void JPEGCodec::_encodeComment()
{
	int commentLength = (int)image->comment.length();

	if (commentLength == 0)
		return;
	writer->writeByte(0xFF);
	writer->writeByte(JPEGSectionCOM);

	int fullCommentLength = commentLength * 3 + 5;
	char *commentBuffer = new char[fullCommentLength];

	int writtenBytes = WideCharToMultiByte(CP_UTF8, 0, image->comment.c_str(), -1, commentBuffer, fullCommentLength, NULL, NULL);
	if (commentBuffer[writtenBytes-1] == 0)
		writtenBytes--;

	int maxSize = (1 << 16) - 3;
	int size = min(writtenBytes, maxSize);

	writer->writeShort(size+2);
	writer->writeBytes((byte*)commentBuffer, (size_t)size);

	delete [] commentBuffer;
}

void JPEGCodec::_encodeSOF0()
{
	writer->writeByte(0xFF);
	writer->writeByte(JPEGSectionSOF0);

	int sectionLength = 17;

	writer->writeShort(sectionLength);
	writer->writeByte(8);
	writer->writeShort(height);
	writer->writeShort(width);
	writer->writeByte(3);

	for (int i = 0; i < 3; i++)
	{
		writer->writeByte(i+1);
		int subsInfo = componentsInfo[i].horizontalSubsampling << 4;
		subsInfo |= componentsInfo[i].verticalSubsampling;
		writer->writeByte(subsInfo);

		writer->writeByte(componentsInfo[i].quantizationTableID);
	}
}

void JPEGCodec::_encodeDQT()
{
	for (int comp = 0; comp < 2; comp++)
	{
		writer->writeByte(0xFF);
		writer->writeByte(JPEGSectionDQT);

		unsigned short tableLength = 67;
		writer->writeShort(tableLength);

		writer->writeByte(comp);
		for (int i = 0; i < 64; i++)
		{
			writer->writeByte(quantizationTables[comp][_zig[i]]);
		}
	}
}

void JPEGCodec::_encodeDHT()
{
	for (int comp = 0; comp < 2; comp++)
	{
		for (int dcac = 0; dcac < 2; dcac++)
		{
			HuffmanEncodeInfo *huffPtr;
			if (dcac == 0)
				huffPtr = &dcHuffmanEncodeTables[comp];
			else
				huffPtr = &acHuffmanEncodeTables[comp];

			writer->writeByte(0xFF);
			writer->writeByte(JPEGSectionDHT);

			int huffLen = 3 + 16;

			int codesCount = 0;
			for (int i = 0; i < 16; i++)
				codesCount += huffPtr->lengthsCount[i];
			huffLen += codesCount;

			writer->writeShort(huffLen);

			int fullTableID = dcac << 4;
			fullTableID |= comp;

			writer->writeByte(fullTableID);

			for (int i = 0; i < 16; i++)
				writer->writeByte(huffPtr->lengthsCount[i]);

			int ptrInVals = 0;
			for (int i = 0; i < 16; i++)
			{
				for (int j = 0, jEnd = huffPtr->lengthsCount[i]; j < jEnd; j++)
					writer->writeByte(huffPtr->freqSortedVals[ptrInVals++]);
			}
		}
	}
}

void JPEGCodec::_prepareHuffmanTables()
{
	int dcFreqs[2][256], acFreqs[2][256];

	memset(dcFreqs, 0, sizeof(dcFreqs));
	memset(acFreqs, 0, sizeof(acFreqs));

	int totalMCUs = mcusPerCol * mcusPerRow;

	int posInBlock = 0;

	int dcPredictors[3];
	memset(dcPredictors, 0, sizeof(dcPredictors));

	for (int cmcu = 0; cmcu < totalMCUs; cmcu++)
	{
		for (int ci = 0; ci < 3; ci++)
		{
			int curInd = 0;
			if (ci > 0)
				curInd = 1;

			for (int i = 0, iEnd = componentsInfo[ci].subBlocksPerBlock(); i < iEnd; i++)
			{
				int nextDCVal = encodedBlocks[posInBlock];
				int dcValDiff = nextDCVal - dcPredictors[ci];
				dcPredictors[ci] = nextDCVal;

				int valToEncode = lengthOfNum(dcValDiff);
				dcFreqs[curInd][valToEncode]++;

				for (int j = 1; j < 64; j++)
				{
					int zeroCount = 0;
					for (; j < 64; j++, zeroCount++)
						if (encodedBlocks[posInBlock + j] != 0)
							break;
					int curByte = 0;
					if (j >= 64)
						acFreqs[curInd][curByte]++;
					else
					{
						while (zeroCount >= 16)
						{
							zeroCount -= 16;
							curByte = 0xF0;
							acFreqs[curInd][curByte]++;
						}
						curByte = zeroCount << 4;
						curByte |= lengthOfNum(encodedBlocks[posInBlock + j]);
						acFreqs[curInd][curByte]++;
					}
				}
				posInBlock += 64;
			}
		}
	}

	_buildHuffmanEncodeTable(&dcHuffmanEncodeTables[0], dcFreqs[0]);
	_buildHuffmanEncodeTable(&dcHuffmanEncodeTables[1], dcFreqs[1]);
	_buildHuffmanEncodeTable(&acHuffmanEncodeTables[0], acFreqs[0]);
	_buildHuffmanEncodeTable(&acHuffmanEncodeTables[1], acFreqs[1]);
}

void JPEGCodec::_writeBit(int bit)
{
	bit &= 1;
	curByte |= bit << (7 - curBitPos);
	curBitPos++;

	if (curBitPos == 8)
	{
		curBitPos = 0;
		writer->writeByte(curByte);
		if (curByte == 0xFF)
			writer->writeByte(0);
		curByte = 0;
	}
}

void JPEGCodec::_flushBits()
{
	if (curBitPos > 0)
		writer->writeByte(curByte);
}

void JPEGCodec::_writeNum(int num, int len, bool isSigned)
{
	if (len == 0)
		return;
	if (isSigned)
	{
		int fb = 1;
		if (num < 0)
		{
			fb = 0;
			num = (1 << len) + num - 1;
		}
		_writeBit(fb);
		len--;
	}
	len--;
	while (len >= 0)
	{
		int curBit = num >> len;
		curBit &= 1;
		_writeBit(curBit);
		len--;
	}
}

//#define __fprintf(file, str, ...) 
//#define __fprintf(file, str, ...)  fprintf(file, str, __VA_ARGS__)

FILE *dumpFile;

#ifdef JPEG_DEBUG

#define MY_FOPEN fopen
#define MY_FCLOSE fclose
#define condLog(cond, format, ...) \
	if ((cond)) \
{\
	fprintf(dumpFile, format, __VA_ARGS__); \
}

#else

#define MY_FOPEN NULL;
#define MY_FCLOSE
#define condLog(cond, format, ...) 

#endif

void JPEGCodec::_encodeSOS()
{
	dumpFile = MY_FOPEN("offsetDump.txt", "w");
	writer->writeByte(0xFF);
	writer->writeByte(JPEGSectionSOS);

	writer->writeShort(12);
	writer->writeByte(componentsCount);

	for (int i = 0; i < componentsCount; i++)
	{
		writer->writeByte(componentsInfo[i].id);
		int huffID = componentsInfo[i].quantizationTableID;
		huffID = (huffID << 4) + huffID;
		writer->writeByte(huffID);
	}

	writer->writeByte(0x00);
	writer->writeByte(0x3F);
	writer->writeByte(0x00);

	int totalMCUs = mcusPerCol * mcusPerRow;

	curBitPos = 0;
	curByte = 0;

	int posInBlock = 0;

	int dcPredictors[3];
	memset(dcPredictors, 0, sizeof(dcPredictors));

	int logStartVal = 240*21 + 214;

	for (int cmcu = 0; cmcu < totalMCUs; cmcu++)
	{
		bool needsLog = (cmcu >= logStartVal)  && (cmcu <= logStartVal + 20);
		if (needsLog)
			condLog(needsLog, "LogStart\n\n");
		for (int ci = 0; ci < 3; ci++)
		{
			int curInd = 0;
			if (ci > 0)
				curInd = 1;

			HuffmanEncodeInfo &dcHuff = dcHuffmanEncodeTables[curInd];
			HuffmanEncodeInfo &acHuff = acHuffmanEncodeTables[curInd];

			for (int i = 0, iEnd = componentsInfo[ci].subBlocksPerBlock(); i < iEnd; i++)
			{
				int nextDCVal = encodedBlocks[posInBlock];
				int dcValDiff = nextDCVal - dcPredictors[ci];
				dcPredictors[ci] = nextDCVal;

				int numLen = lengthOfNum(dcValDiff);
				
				int code = dcHuff.codes[numLen];
				int codeLen = dcHuff.lengths[numLen];

				condLog(needsLog, "%X.%d\n", writer->bufPos + writer->totalBytesWritten, curBitPos);

				_writeNum(code, codeLen, 0);
				_writeNum(dcValDiff, numLen, 1);

				for (int j = 1; j < 64; j++)
				{
					int zeroCount = 0;
					for (; j < 64; j++, zeroCount++)
						if (encodedBlocks[posInBlock + j] != 0)
							break;
					int curByte = 0;
					if (j >= 64)
					{
						code = acHuff.codes[curByte];
						codeLen = acHuff.lengths[curByte];

						condLog(needsLog, "%X.%d\n", writer->bufPos + writer->totalBytesWritten, curBitPos);
						_writeNum(code, codeLen, 0);
					}
					else
					{
						while (zeroCount >= 16)
						{
							zeroCount -= 16;
							curByte = 0xF0;
							code = acHuff.codes[curByte];
							codeLen = acHuff.lengths[curByte];

							condLog(needsLog, "%X.%d\n", writer->bufPos + writer->totalBytesWritten, curBitPos);
							_writeNum(code, codeLen, 0);
						}
						curByte = zeroCount << 4;

						numLen = lengthOfNum(encodedBlocks[posInBlock + j]);
						curByte |= numLen;

						code = acHuff.codes[curByte];
						codeLen = acHuff.lengths[curByte];

						condLog(needsLog, "%X.%d\n", writer->bufPos + writer->totalBytesWritten, curBitPos);
						_writeNum(code, codeLen, 0);
						_writeNum(encodedBlocks[posInBlock + j], numLen, 1);
					}
				}

				condLog(needsLog, "\n\n");

				posInBlock += 64;
			}
		}
	}

	_flushBits();

	MY_FCLOSE(dumpFile);

	writer->writeByte(0xFF);
	writer->writeByte(JPEGSectionEOI);
}

void JPEGCodec::_buildHuffmanEncodeTable(HuffmanEncodeInfo *huffmanPtr, int *frequencies)
{
	typedef pair<int, int> pii;
	pii sortedCodes[257];
	for (int i = 0; i < 256; i++)
	{
		sortedCodes[i].first = frequencies[i] * 2;
		sortedCodes[i].second = i;
	}
	sortedCodes[256].first = 1;
	sortedCodes[256].second = 256;

	struct CoinsList
	{
		int currentValue;
		int currentElem;
		int nextIndex;
		int lastIndex;
		CoinsList()
		{
			nextIndex = -1;
		}
	} coinsList[257*16];

	sort(sortedCodes, sortedCodes + 257);

	int prevRoundList[2][257];
	int prevRoundLength[2] = {0, 0};
	int cur = 0;
	int next = 1;
	for (int i = 0; i < 16; i++)
	{
		int startIdx = 257*i;
		for (int j = 0; j < 257; j++)
		{
			coinsList[startIdx + j].currentElem = sortedCodes[j].second;
			coinsList[startIdx + j].currentValue = sortedCodes[j].first;
			coinsList[startIdx + j].lastIndex = startIdx + j;
		}

		int curPtr = 0, prevPtr = 0;
		int total = prevRoundLength[cur] + 257;
		int pickedCnt = 0;
		int pickedInds[2];

		prevRoundLength[next] = 0;

		while (total > 0)
		{
			int smallestCurVal = (int)1e9;
			if (curPtr < 257)
				smallestCurVal = sortedCodes[curPtr].first;
			int smallestPrevVal = (int)1e9;
			int prevRoundInd = 0;
			if (prevPtr < prevRoundLength[cur])
			{
				prevRoundInd = prevRoundList[cur][prevPtr];
				smallestPrevVal = coinsList[prevRoundInd].currentValue;
			}

			if (smallestCurVal < smallestPrevVal)
			{
				pickedInds[pickedCnt++] = startIdx + curPtr;
				curPtr++;
			}
			else
			{
				pickedInds[pickedCnt++] = prevRoundInd;
				prevPtr++;
			}

			total--;
			if (pickedCnt == 2)
			{
				int &cLen = prevRoundLength[next];
				prevRoundList[next][cLen++] = pickedInds[0];

				coinsList[pickedInds[0]].currentValue += coinsList[pickedInds[1]].currentValue;
				coinsList[coinsList[pickedInds[0]].lastIndex].nextIndex = pickedInds[1];
				coinsList[pickedInds[0]].lastIndex = coinsList[pickedInds[1]].lastIndex;
				pickedCnt = 0;
			}
		}
		
		swap(cur, next);
	}

	int selectedCount[257];
	memset(selectedCount, 0, sizeof(selectedCount));

	for (int i = 0; i < 256; i++)
	{
		int curInd = prevRoundList[cur][i];
		while (curInd != -1)
		{
			CoinsList &cl = coinsList[curInd];
			selectedCount[cl.currentElem]++;
			curInd = cl.nextIndex;
		}
	}

	int curH = 0;
	int maxCode = 0;

	for (int i = 0; i < 16; i++)
		huffmanPtr->lengthsCount[i] = 0;

	reverse(sortedCodes, sortedCodes + 257);
	for (int i = 0; i < 256; i++)
	{
		int nextH = selectedCount[sortedCodes[i].second];
		if (sortedCodes[i].first <= 1)
			break;

		huffmanPtr->lengthsCount[nextH-1]++;
		huffmanPtr->freqSortedVals[i] = sortedCodes[i].second;
		while (curH < nextH)
		{
			curH++;
			maxCode *= 2;
		}
		huffmanPtr->lengths[sortedCodes[i].second] = nextH;
		huffmanPtr->codes[sortedCodes[i].second] = maxCode;
		maxCode++;
	}
}

void JPEGCodec::_encodeSingleBlock(int startBlockIndex, int blocksCount)
{
	JPEGEncodeTask *encodeTask = (JPEGEncodeTask*)JPEGEncodeTask::alloc()->init();
	encodeTask->startBlockIndex = startBlockIndex;
	encodeTask->blocksCount = blocksCount;
	encodeTask->codec = this;
	this->retain();
	taskScheduler->addTask(encodeTask, taskIndex);
	encodeTask->release();

	//_encodeSingleBlockThreaded(startBlockIndex, blocksCount);
}

void JPEGCodec::_encodeSingleBlockThreaded(int startBlockIndex, int blocksCount)
{
	int mcuYPixel = startBlockIndex / mcusPerRow;
	mcuYPixel *= mcuHeight;
	int mcuXPixel = startBlockIndex % mcusPerRow;
	mcuXPixel *= mcuWidth;

	const int maxMCUSize = 16;

	int _mcuBuf[3 * maxMCUSize * 64];
	int _mcuConvertedBuf[3 * maxMCUSize * 64];

	int *mcuBuf = _mcuBuf;
	int *mcuConvertedBuf = _mcuConvertedBuf;

	int mcuBlockSize = mcuBlockHeight * mcuBlockWidth;

	if (mcuBlockSize > maxMCUSize)
	{
		mcuBuf = new int[3 * mcuBlockSize * 64];
		mcuConvertedBuf = new int[3 * mcuBlockSize * 64];
	}

	ColorSpaceYCbCr *cs = ColorSpaceYCbCr::singleton();

	int rowsPerBlock[16], colsPerBlock[16];
	for (int ci = 0; ci < componentsCount; ci++)
	{
		rowsPerBlock[ci] = mcuBlockHeight / componentsInfo[ci].verticalSubsampling;
		colsPerBlock[ci] = mcuBlockWidth / componentsInfo[ci].horizontalSubsampling;
	}

	for (int bnum = 0; bnum < blocksCount; bnum++)
	{
		int curBlockIndex = startBlockIndex + bnum;

		int *mcuStartPtr = encodedBlocks + curBlockIndex * mcuSize;

		int posInBuf = 0;

		//Copying current MCU pixels to buffer

		for (int i = 0; i < mcuHeight; i++)
		{
			for (int j = 0; j < mcuWidth; j++)
			{
				for (int ci = 0; ci < 3; ci++)
				{
					int curY = i + mcuYPixel;
					int curX = j + mcuXPixel;
					if (curY >= image->getHeight())
						curY = image->getHeight() - 1;
					if (curX >= image->getWidth())
						curX = image->getWidth() - 1;
					mcuBuf[posInBuf++] = image->getPixel(curX, curY, ci);
				}
			}
		}

		cs->convertImageFromRGB(mcuBuf, mcuConvertedBuf, mcuHeight * mcuWidth);

		int posInMCUBuf = 0;

		//Subsampling

		for (int ci = 0; ci < componentsCount; ci++)
		{
			int avgCnt = rowsPerBlock[ci] * colsPerBlock[ci];
			int avgCnt2 = avgCnt >> 1;
			int shift = -1;
			for (int i = 0; i < 31; i++)
			{
				int cv = 1 << i;
				if (cv == avgCnt)
				{
					shift = i;
					break;
				}
				else if (cv > avgCnt)
					break;
			}

			for (int i = 0; i < componentsInfo[ci].verticalSubsampling; i++)
			{
				for (int j = 0; j < componentsInfo[ci].horizontalSubsampling; j++)
				{
					for (int y = 0; y < 8; y++)
					{
						for (int x = 0; x < 8; x++)
						{
							int curVal = 0;
							
							int tY = (i * 8 + y) * rowsPerBlock[ci];
							int tX = (j * 8 + x) * colsPerBlock[ci];

							for (int _y = 0; _y < rowsPerBlock[ci]; _y++)
							{
								for (int _x = 0; _x < colsPerBlock[ci]; _x++)
								{
									int fY = tY + _y;
									int fX = tX + _x;
									int fullInd = (mcuWidth * fY + fX) * 3 + ci;
									curVal += mcuConvertedBuf[fullInd];
								}
							}

							curVal += avgCnt2;
							if (shift != -1)
								curVal >>= shift;
							else
								curVal /= avgCnt;
							mcuStartPtr[posInMCUBuf++] = curVal;
						}
					}
				}
			}
		}

		//Subsampling finish

		//DCT and quantization

		posInMCUBuf = 0;

		for (int compIdx = 0; compIdx < componentsCount; compIdx++)
		{
			ComponentInfo &curCompInfo = componentsInfo[compIdx];
			int *quantizationTable = quantizationTables[curCompInfo.quantizationTableID];

			for (int subblockIdx = 0, subblockIdxEnd = curCompInfo.subBlocksPerBlock(); subblockIdx < subblockIdxEnd; subblockIdx++)
			{
				int *block2 = mcuStartPtr + posInMCUBuf;

				int block[64];

				float srcBuf[8], dstBuf[8];
				float intermBuf[64];

				for (int i = 0; i < 8; i++)
				{
					for (int j = 0; j < 8; j++)
					{
						srcBuf[j] = (float)block2[j*8+i];
					}
					perform1DDCT(srcBuf, dstBuf);
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
					perform1DDCT(srcBuf, dstBuf);
					for (int i = 0; i < 8; i++)
						block[j*8+i] = (int)dstBuf[i];
				}

				for (int k = 0; k < 64; k++)
				{
					block[k] = (block[k] + quantizationTable[k] / 2) / quantizationTable[k];
				}

				for (int k = 0; k < 64; k++)
					block2[_dezig[k]] = block[k];

				posInMCUBuf += 64;
			}
		}

		mcuXPixel += mcuWidth;
		if (mcuXPixel >= width)
		{
			mcuXPixel = 0;
			mcuYPixel += mcuHeight;
		}
	}

	if (mcuBlockSize > maxMCUSize)
	{
		delete [] mcuBuf;
		delete [] mcuConvertedBuf;
	}
}

void JPEGCodec::_cleanupEncode()
{
	delete [] encodedBlocks;
	delete [] quantizationTables[0];
	delete [] quantizationTables[1];
}