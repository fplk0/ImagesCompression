#include "stdafx.h"

#include "ColorSpaceYCbCr.h"

#include "JPEGCodec.h"

#include "dct.h"

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

void JPEGCodec::runEncode()
{
	componentsCount = 3;

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

	componentsInfo[0].quantizationTableID = 1;
	componentsInfo[1].quantizationTableID = 2;
	componentsInfo[2].quantizationTableID = 2;

	mcuBlockWidth = maxHorizontalSubsampling;
	mcuWidth = mcuBlockWidth * 8;
	mcuHeight = mcuBlockHeight * 8;


	mcusPerRow = (width + mcuBlockWidth * 8 - 1) / (mcuBlockWidth * 8);
	mcusPerCol = (height + mcuBlockHeight * 8 - 1) / (mcuBlockHeight * 8);

	int totalMCUs = mcusPerCol * mcusPerRow;

	encodedBlocks = new int[mcuSize * totalMCUs];

	int mcusPerBlock = 16;

	for (int i = 0, iEnd = totalMCUs / mcusPerBlock; i < iEnd; i++)
	{
		_encodeSingleBlock(i * mcusPerBlock, mcusPerBlock);
	}

	if (totalMCUs % mcusPerBlock != 0)
	{
		_encodeSingleBlock(totalMCUs - totalMCUs % mcusPerBlock, totalMCUs % mcusPerBlock);
	}


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
		quantizationTables[0][i] = (int)defaultQuantizationTable[0][i] * mulCoef;
		quantizationTables[1][i] = (int)defaultQuantizationTable[1][i] * mulCoef;
	}
}

void JPEGCodec::_encodeSingleBlock(int startBlockIndex, int blocksCount)
{
	int mcuYPixel = startBlockIndex / mcusPerRow;
	int mcuXPixel = startBlockIndex % mcusPerRow;
	
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
					int val = mcuConvertedBuf[posInBuf++];
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

		cs->convertImageFromRGB(mcuBuf, mcuConvertedBuf, mcuSize);
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
					shift = cv;
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
				int *block = mcuStartPtr + posInMCUBuf;

				float srcBuf[8], dstBuf[8];
				float intermBuf[64];

				for (int i = 0; i < 8; i++)
				{
					for (int j = 0; j < 8; j++)
					{
						srcBuf[j] = (float)block[j*8+i];
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
						block[j*8+i] = dstBuf[i];
				}

				for (int k = 0; k < 64; k++)
				{
					block[k] /= quantizationTable[k];
				}

				posInMCUBuf += 64;
			}
		}
	}
}