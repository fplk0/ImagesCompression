// CLRWrapper.h

#pragma once

#include "JPEGCodec.h"
#include "SFFileStreamReader.h"
#include "SFFileStreamWriter.h"
#include "SFMemoryStreamWriter.h"

#include "SFImage.h"
#include "ColorSpaceRGB.h"

using namespace System;

namespace CLRWrapper 
{
	public ref class JPEGSettings
	{
	public:
		cli::array<int>^ horizontalSubsampling;
		cli::array<int>^ verticalSubsampling;
		int quality;

		JPEGSettings()
		{
			horizontalSubsampling = gcnew cli::array<int>(3) { 1, 2, 2 };
			verticalSubsampling = gcnew cli::array<int>(3) { 1, 2, 2};
			quality = 90;
		}
	};

	public ref class Lab1Wrapper
	{
		// TODO: Add your methods for this class here.
		static cli::array<float, 3>^ convertImageToPixels(SFImage *img)
		{
			cli::array<float, 3> ^pixels = gcnew cli::array<float, 3>(img->getHeight(), img->getWidth(), 3);

			for (int y = 0; y < img->getHeight(); y++)
			{
				for (int x = 0; x < img->getWidth(); x++)
					for (int comp = 0; comp < 3; comp++)
					{
						int val = img->getPixel(x, y, comp);
						pixels[y, x, comp] = val / 255.0f;
					}
			}

			return pixels;
		}

		static SFImage* convertPixelsToImage(cli::array<float, 3>^ pixels)
		{
			SFImage *retVal = SFImage::alloc()->initWithSizeAndColorSpace(pixels->GetLength(1), pixels->GetLength(0), ColorSpaceRGB::singleton());

			for (int y = 0; y < pixels->GetLength(0); y++)
			{
				for (int x = 0; x < pixels->GetLength(1); x++)
					for (int comp = 0; comp < 3; comp++)
					{
						int val = pixels[y, x, comp] * 255.0f;
						retVal->setPixel(x, y, comp, val);
					}
			}

			return retVal;
		}

	static void encodeJPEGStatic(cli::array<float, 3> ^pixels, SFStreamWriter *streamWriter, JPEGSettings ^qualitySettings)
	{
		SFImage *img = convertPixelsToImage(pixels);

		JPEGCodec *codec = JPEGCodec::alloc()->init();
		codec->setImage(img);
		codec->setDestinationStream(streamWriter);

		codec->setEncodeQuality(qualitySettings->quality);
		for (int i = 0; i < 3; i++)
		{
			codec->setHorizontalSubsamplingForComponent(qualitySettings->horizontalSubsampling[i], i);
			codec->setVerticalSubsamplingForComponent(qualitySettings->verticalSubsampling[i], i);
		}

		codec->runEncode();

		codec->release();
		img->release();
	}

	public:

		static cli::array<float, 3>^ decodeJPEGStatic(System::String ^filePath)
		{
			cli::array<wchar_t> ^buf = filePath->ToCharArray();
			pin_ptr<wchar_t> ptr = &buf[0];

			wstring path(ptr);
			SFFileStreamReader *reader = SFFileStreamReader::alloc()->initWithFileName(path);

			JPEGCodec *codec = JPEGCodec::alloc()->init();
			codec->setSourceStream(reader);
			codec->runDecode();
			SFImage *img = codec->getImage();

			cli::array<float, 3>^ pixels = convertImageToPixels(img);

			reader->release();
			codec->release();

			return pixels;
		}

		static void encodeJPEGStatic(cli::array<float, 3> ^pixels, System::String ^filePath, JPEGSettings ^qualitySettings)
		{
			cli::array<wchar_t> ^buf = filePath->ToCharArray();
			pin_ptr<wchar_t> ptr = &buf[0];
			
			wstring path(ptr);

			SFFileStreamWriter *writer= SFFileStreamWriter::alloc()->initWithFileName(path);

			encodeJPEGStatic(pixels, writer, qualitySettings);

			writer->release();
		}

		static cli::array<byte>^ encodeJPEGStatic(cli::array<float, 3> ^pixels, JPEGSettings ^qualitySettings)
		{
			SFMemoryStreamWriter *writer = SFMemoryStreamWriter::alloc()->initDynamic();

			encodeJPEGStatic(pixels, writer, qualitySettings);

			int dataLength = writer->getData()->getLength();

			cli::array<byte> ^arr = gcnew cli::array<byte>(dataLength);
			pin_ptr<byte> arrPtr = &arr[0];
			memcpy(arrPtr, writer->getData()->getData(), dataLength);

			writer->release();

			return arr;
		}
	};
}
