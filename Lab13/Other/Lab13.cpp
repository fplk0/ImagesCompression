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

int _tmain(int argc, _TCHAR* argv[])
{
	//testDct();
	//return 0;

	//testColorSpace();
	//return 0;

	JPEGCodec *test = JPEGCodec::alloc()->init();

	//Sleep(4000);

	clock_t cl = clock();
	//SFFileStreamReader *reader = SFFileStreamReader::alloc()->initWithFileName(L"Resources/testJpeg.jpg");
	SFFileStreamReader *reader = SFFileStreamReader::alloc()->initWithFileName(L"Resources/maelstromBig.jpg");
	test->setSourceStream(reader);
	test->runDecode();

	printf("%lf\n", (clock() - cl) / (double)CLOCKS_PER_SEC);

	JPEGCodec *encoder = JPEGCodec::alloc()->init();
	SFFileStreamWriter *writer = SFFileStreamWriter::alloc()->initWithFileName(L"Resources/testJpegEncoded.jpg");
	encoder->setDestinationStream(writer);

	encoder->setImage(test->getImage());

	for (int i = 0; i < 3; i++)
	{
		encoder->setHorizontalSubsamplingForComponent(test->getHorizontalSubsamplingForComponent(i), i);
		encoder->setVerticalSubsamplingForComponent(test->getVerticalSubsamplingForComponent(i), i);
	}

	encoder->setEncodeQuality(50);

	encoder->runEncode();

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

