// Lab13.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "LZWCodec.h"
#include "HuffmanCodec.h"
#include "RLECodec.h"

#include "SFMemoryStreamReader.h"
#include "SFMemoryStreamWriter.h"

#include "SFFileStreamReader.h"
#include "SFFileStreamWriter.h"

#include "ColorSpaceRGB.h"
#include "ColorSpaceYCbCr.h"

#include "SFImage.h"
#include "JPEGCodec.h"

#include "dct.h"

#include <Windows.h>

#include <vector>
#include <ctime>

using namespace std;

void testMemory()
{
	char testData[] = "8888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888888812345112233445556";
	int testLen = strlen(testData);
	SFData *td = SFData::alloc()->initWithData((byte*)testData, testLen, 0);

	GenericCodec::GenericCodecMetaClass *meta;// = &GenericCodec::metaObject;
	//meta = &RLECodec::metaObject;
	//meta = &LZWCodec::metaObject;
	meta = &HuffmanCodec::metaObject;

	GenericCodec *testCodec = meta->alloc();
	//testCodec->initWithFileNames(L"Resources\\testInput.txt", L"Resources\\testEncoded.txt");
	//testCodec->initWithFileNames(L"Resources\\RLETest.txt", L"Resources\\testEncoded.txt");
	//testCodec->initWithFileNames(L"Resources\\YuraTest.txt", L"Resources\\testEncoded.txt");
	//testCodec->initWithFileNames(L"D:/GoT.mkv", L"D:/Got_Encoded");
	//wstring fName(L"D:\\Films\\Game of Thrones\\S3\\Game.of.Thrones.S03E04.720p.rus.LostFilm.TV.mkv");
	//testCodec->initWithFileNames(fName, L"D:/Got_Encoded");

	SFMemoryStreamReader *sr = SFMemoryStreamReader::alloc()->initWithData(td);
	SFMemoryStreamWriter *sw = SFMemoryStreamWriter::alloc()->initDynamic();
	testCodec->initWithStreams(sr, sw);

	clock_t encodeStart = clock();

	testCodec->runEncode();

	printf("Encode time: %lf\n", (clock() - encodeStart) / (double)CLOCKS_PER_SEC);

	testCodec->release();

	SFData *encodedData = sw->getData();
	encodedData->retain();
	sw->release();
	sr->release();
	
	GenericCodec *testDecoder = static_cast<GenericCodec*>(meta->alloc());
	//testDecoder->initWithFileNames(L"Resources\\testEncoded.txt", L"Resources\\testDecoded.txt");
	//testDecoder->initWithFileNames(L"D:/Got_Encoded", L"D:/GoT_decoded.mkv");

	sr = SFMemoryStreamReader::alloc()->initWithData(encodedData);

	sw = SFMemoryStreamWriter::alloc()->initDynamic();

	testDecoder->initWithStreams(sr, sw);

	clock_t decodeStart = clock();

	testDecoder->runDecode();

	printf("Decode time: %lf\n", (clock() - decodeStart) / (double)CLOCKS_PER_SEC);
	
	testDecoder->release();

	sr->release();
	SFData *decodedData = sw->getData();

	char *testOutput = new char[decodedData->getLength() + 5];
	testOutput[decodedData->getLength()] = 0;

	memcpy(testOutput, decodedData->getData(), decodedData->getLength());
	encodedData->release();
	encodedData = NULL;
	decodedData->release();
	decodedData = NULL;
}

void testFiles()
{
		//GenericCodec *testCodec = LZWCodec::alloc();
	//GenericCodec *testCodec = HuffmanCodec::alloc();
	GenericCodec *testCodec = RLECodec::alloc();

	//testCodec->initWithFileNames(L"Resources\\testInput.txt", L"Resources\\testEncoded.txt");
	//testCodec->initWithFileNames(L"Resources\\RLETest.txt", L"Resources\\testEncoded.txt");
	//testCodec->initWithFileNames(L"Resources\\YuraTest.txt", L"Resources\\testEncoded.txt");
	//testCodec->initWithFileNames(L"D:/GoT.mkv", L"D:/Got_Encoded");
	//wstring fName(L"D:\\Films\\Game of Thrones\\S3\\Game.of.Thrones.S03E04.720p.rus.LostFilm.TV.mkv");
	//testCodec->initWithFileNames(fName, L"D:/Got_Encoded");

	clock_t encodeStart = clock();

	testCodec->runEncode();

	printf("Encode time: %lf\n", (clock() - encodeStart) / (double)CLOCKS_PER_SEC);

	testCodec->release();

	//GenericCodec *testDecoder = LZWCodec::alloc();
	/*GenericCodec *testDecoder = HuffmanCodec::alloc();*/
	GenericCodec *testDecoder = RLECodec::alloc();
	testDecoder->initWithFileNames(L"Resources\\testEncoded.txt", L"Resources\\testDecoded.txt");
	//testDecoder->initWithFileNames(L"D:/Got_Encoded", L"D:/GoT_decoded.mkv");
	clock_t decodeStart = clock();

	testDecoder->runDecode();

	printf("Decode time: %lf\n", (clock() - decodeStart) / (double)CLOCKS_PER_SEC);
	
	testDecoder->release();
}

DWORD __stdcall threadTest(void *param)
{
	ColorSpaceRGB::singleton();

	printf("1\n");

	//*((int*)param) = 1;

	return 0;
}


void testColorSpace()
{
	int vals[3] = {255, 231, 255};
	int vals2[3] = {138, 60, 19}, vals3[3];
	ColorSpaceYCbCr *sp = ColorSpaceYCbCr::singleton();
	sp->convertColorFromRGB(vals, vals2);
	sp->convertColorToRGB(vals2, vals);
}

void testDct()
{
	float src[8], dst[8], src2[8];
	for (int i = 0; i < 8; i++)
		src[i] = rand() % 10;

	initDCT();

	perform1DDCT(src, dst);
	perform1DIDCT(dst, src2);

}

void testCodec(GenericCodec *codec, FILE *dumpFile, wstring outputPath)
{
	vector<wstring> paths;

	paths.push_back(L"hamlet.txt");
	paths.push_back(L"bodyBuild.txt");
	paths.push_back(L"druidTree.bmp");
	paths.push_back(L"word.bmp");
	paths.push_back(L"maelstromBig.bmp");

	for (int i = 0; i < paths.size(); i++)
	{
		wstring curPath = paths[i];
		wstring fullPath = wstring(L"Resources/").append(curPath);
		wstring pathInOutput = wstring(outputPath).append(curPath);
		
		wstring pathInOutputEncoded = wstring(pathInOutput).append(L".encoded");

		SFFileStreamReader *reader = SFFileStreamReader::alloc()->initWithFileName(fullPath);
		SFFileStreamWriter *writer = SFFileStreamWriter::alloc()->initWithFileName(pathInOutputEncoded);

		reader->openStream();
		int sourceSize = reader->streamSize();

		codec->setSourceStream(reader);
		codec->setDestinationStream(writer);

		clock_t cl = clock();

		codec->runEncode();

		double encodeTime = (clock() - cl) / (double)CLOCKS_PER_SEC;

		reader->release();
		writer->release();

		reader = SFFileStreamReader::alloc()->initWithFileName(pathInOutputEncoded);
		writer = SFFileStreamWriter::alloc()->initWithFileName(pathInOutput);

		reader->openStream();

		int encodedSize = reader->streamSize();

		codec->setSourceStream(reader);
		codec->setDestinationStream(writer);

		cl = clock();

		codec->runDecode();

		double decodeTime = (clock() - cl) / (double)CLOCKS_PER_SEC;

		reader->release();
		writer->release();

		double compRat = encodedSize / (double)sourceSize;
		compRat = 1 / compRat;

		fwprintf(dumpFile, L"%20s %lf %lf %lf %lf\n", curPath.c_str(), encodeTime, decodeTime, encodeTime / decodeTime, compRat);
	}
}

void testSimpleAlgos()
{
	FILE *dumpFile;// = fopen("Resources/dump.txt", "r");
	_wfopen_s(&dumpFile, L"Resources/dump.txt", L"w");

	fwprintf(dumpFile, L"FileName -- Encode time -- Decode time -- Symmetry -- Comp ratio");

	wstring path;

	GenericCodec *codec;

	codec = HuffmanCodec::alloc()->initWithStreams(NULL, NULL);
	path = L"Resources/huffman/";

	fwprintf(dumpFile, L"\n\nHuffman\n");

	testCodec(codec, dumpFile, path);

	codec->release();


	codec = LZWCodec::alloc()->initWithStreams(NULL, NULL);
	path = L"Resources/lzw/";

	fwprintf(dumpFile, L"\n\nLZW\n");

	testCodec(codec, dumpFile, path);

	codec->release();


	codec = RLECodec::alloc()->initWithStreams(NULL, NULL);
	path = L"Resources/rle/";

	fwprintf(dumpFile, L"\n\nRLE\n");

	testCodec(codec, dumpFile, path);

	codec->release();


	fclose(dumpFile);
}

int _tmain(int argc, _TCHAR* argv[])
{
	testSimpleAlgos();
	return 0;
	//testDct();
	//return 0;

	//testColorSpace();
	//return 0;

	JPEGCodec *test;

	//Sleep(4000);

	clock_t cl;
	SFFileStreamReader *reader;
	SFImage *img = NULL;

	clock_t totalEncode = 0, totalDecode = 0;

	for (int i = 0, iMax = 100; i < iMax; i++)
	{
		//SFFileStreamReader *reader = SFFileStreamReader::alloc()->initWithFileName(L"Resources/testJpeg.jpg");
		test = JPEGCodec::alloc()->init();
		reader = SFFileStreamReader::alloc()->initWithFileName(L"Resources/maelstromBig.jpg");
		test->setSourceStream(reader);
		cl = clock();
		test->runDecode();

		if (i == iMax - 1)
		{
			img = test->getImage();
			img->retain();
		}
		test->release();
		reader->release();
		totalDecode += clock() - cl;
		printf("%lf\n", (clock() - cl) / (double)CLOCKS_PER_SEC);
	}

	for (int i = 0, iMax = 100; i < iMax; i++)
	{
		JPEGCodec *encoder = JPEGCodec::alloc()->init();
		//SFFileStreamWriter *writer = SFFileStreamWriter::alloc()->initWithFileName(L"Resources/testJpegEncoded.jpg");
		SFMemoryStreamWriter *writer = SFMemoryStreamWriter::alloc()->initDynamic();
		encoder->setDestinationStream(writer);

		encoder->setImage(img);

		for (int i = 0; i < 3; i++)
		{
			encoder->setHorizontalSubsamplingForComponent(test->getHorizontalSubsamplingForComponent(i), i);
			encoder->setVerticalSubsamplingForComponent(test->getVerticalSubsamplingForComponent(i), i);
		}

		encoder->setHorizontalSubsamplingForComponent(2, 0);
		encoder->setVerticalSubsamplingForComponent(2, 0);

		encoder->setHorizontalSubsamplingForComponent(1, 1);
		encoder->setVerticalSubsamplingForComponent(1, 1);

		encoder->setHorizontalSubsamplingForComponent(1, 2);
		encoder->setVerticalSubsamplingForComponent(1, 2);

		encoder->setEncodeQuality(90);

		cl = clock();

		encoder->runEncode();

		encoder->release();
		writer->release();

		totalEncode += clock() - cl;

		printf("%lf\n", (clock() - cl) / (double)CLOCKS_PER_SEC);
	}

	printf("Total encode: %lf\n", totalEncode / (double)CLOCKS_PER_SEC);
	printf("Total decode: %lf\n", totalDecode / (double)CLOCKS_PER_SEC);

	return 0;
	int testInt = reader->readIntInline();
	reader->readBitsInline(testInt, 8);
	printf("%d\n", testInt);
	//printf("%s\n%s\n", str2(a), str(a));

	//const wstring *tt = test->metaClass()->className();
	//int t = test->metaClass()->className()->length();
	//wprintf(L"%s %d\n", tt->c_str(), t);
	//printf("%s %d\n", tt->c_str(), t);
	//test->retain();
	//test->release();
	//testMemory();

	//static int a = 0;
	//int *ptrA = &a;

	//CreateThread(NULL, 0, threadTest, (void*)&a, 0, 0);

	//while(*ptrA == 0);
	//return 0;

	CreateThread(NULL, 0, threadTest, NULL, 0, 0);
	CreateThread(NULL, 0, threadTest, NULL, 0, 0);
	CreateThread(NULL, 0, threadTest, NULL, 0, 0);
	ColorSpace *rgbSingle1 = ColorSpaceRGB::singleton();
	ColorSpace *rgbSingle2 = ColorSpaceRGB::singleton();
	if (rgbSingle1 != rgbSingle2)
	{

	}

	SFImage *test1 = SFImage::alloc()->initWithSizeAndColorSpace(1, 1, ColorSpaceRGB::singleton());
	test1->setPixel(0, 0, 0, 0.1);
	test1->setPixel(0, 0, 1, 0.2);
	test1->setPixel(0, 0, 2, 0.3);
	SFImage *test2 = test1->copy();

	test1->convertToColorSpace(ColorSpaceYCbCr::singleton());

	test1->release();
	test2->release();

	Sleep(40000);

	//LPTHREAD_START_ROUTINE

	return 0;
}

