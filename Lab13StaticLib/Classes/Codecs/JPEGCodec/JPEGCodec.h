#pragma once

#include "GenericCodec.h"
#include <map>
#include "SFImage.h"
#include "PlatformSpecific.h"

#undef super
#undef self
#define super GenericCodec
#define self JPEGCodec

class JPEGCodec :
	public GenericCodec
{
	DEFAULT_METACLASS_DECLARATION(self, super);

	static int _zig[64], _dezig[64];
	static void _fillZig();

	SFImage *image;

	int width, height;

	int precison;
	int componentsCount;

	struct ComponentInfo
	{
		int id;
		int horizontalSubsampling;
		int verticalSubsampling;
		int quantizationTableID;
		inline int subBlocksPerBlock() { return horizontalSubsampling * verticalSubsampling; };
		ComponentInfo()
		{
			horizontalSubsampling = verticalSubsampling = 1;
		};
	};

	ComponentInfo componentsInfo[16];

	float **pixels;
	int *quantizationTables[16];
	byte curByte;
	byte curFlag;
	int curBitPos;

	int singleDecodeBlockSize;
	int mcusPerRow, mcusPerCol;
	int mcuBlockWidth, mcuBlockHeight;
	int mcuWidth, mcuHeight;
	int mcuSize, mcuBlockSize;

	struct CurrentDecodeBlockInfo
	{
		int *block;
		int pos;
		int posInSmallBlock;
		int startMCUIndex;
		inline void nextBlock() { posInSmallBlock = 0; pos += 64; };
		inline void writeNext(int val) 
		{
			block[pos + _zig[posInSmallBlock++]] = val; 
		};

		CurrentDecodeBlockInfo(int blockSize, int _startMCUIndex)
		{
			startMCUIndex = _startMCUIndex;
			block = new int[blockSize];
			posInSmallBlock = 0;
			pos = 0;
		};
		~CurrentDecodeBlockInfo()
		{
			delete [] block;
		}
	} *decodeBlockInfo;

	int restartInterval;

	struct HuffmanDecodeInfo
	{
		int minCode[16];
		int maxCode[16];
		int valPtr[16];
		int *values;

		HuffmanDecodeInfo() { values = NULL; };
		~HuffmanDecodeInfo() { delete [] values; };
	};

	int *encodedBlocks;

	SF_FORCE_INLINE void _readNum(int &to, int len);
	SF_FORCE_INLINE byte readBit(int &to); //If it finds a marker, it returns it's value, 0 otherwise

	SF_FORCE_INLINE void _writeNum(int &from, int len);
	SF_FORCE_INLINE void writeBit(int &from);

	HuffmanDecodeInfo dcHuffmanDecodeTables[16], acHuffmanDecodeTables[16];

	wstring comment;

	virtual void free();

	enum JPEGSection
	{
		JPEGSectionSOI = 0xD8, 
		JPEGSectionSOF0 = 0xC0, //Baseline DCT
		JPEGSectionSOF2 = 0xC2, //Progressive DCT
		JPEGSectionDHT = 0xC4, 
		JPEGSectionDQT = 0xDB, 
		JPEGSectionSOS = 0xDA, 
		JPEGSectionCOM = 0xFE, 
		JPEGSectionEOI = 0xD9, 
	};

public:
	
	virtual JPEGCodec* init();

	SFImage* getImage() { return image; };
	void setImage(SFImage *img) {  }

//Encoding
public:
	virtual void runEncode();
private:
	void _decodeComment();
	void _decodeSOF0();
	void _decodeDQT();
	void _decodeDHT();
	void _decodeSOS();
	void _dumpQuantizationTables();
	void _decodeSingleBlock(CurrentDecodeBlockInfo *blockAddr);

	SF_FORCE_INLINE int _scanDCCoef(int compNum, int prevVal);
	SF_FORCE_INLINE int _scanACCoef(int compNum);
	SF_FORCE_INLINE void _decodeMCU(int *dcPredictors);

	void _cleanupDecode();

//Decoding
public:
	void setHorizontalSubsamplingForComponent(int horizontalSubsampling, int component);
	virtual void runDecode();
	void _encodeSingleBlock(int startBlockIndex, int blocksCount);
private:


protected:
	virtual ~JPEGCodec(void);
};

