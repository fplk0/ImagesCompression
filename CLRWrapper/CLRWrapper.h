// CLRWrapper.h

#pragma once

using namespace System;

#include "JPEGCodec.h"
#include "SFFileStreamReader.h"
#include "SFImage.h"

namespace CLRWrapper 
{

	public ref class Lab1Wrapper
	{
		// TODO: Add your methods for this class here.

	public:
		static cli::array<float, 3>^ decodeJPEGStatic(System::String ^filePath)
		{
			cli::array<wchar_t> ^buf = filePath->ToCharArray();
			pin_ptr<wchar_t> ptr = &buf[0];

			wstring path(ptr);
			SFFileStreamReader *reader = SFFileStreamReader::alloc()->initWithFileName(path);

			JPEGCodec *test = JPEGCodec::alloc()->init();
			test->setSourceStream(reader);
			test->runDecode();
			SFImage *img = test->getImage();

			cli::array<float, 3> ^pixels = gcnew cli::array<float, 3>(img->getHeight(), img->getWidth(), 3);

			for (int y = 0; y < img->getHeight(); y++)
				for (int x = 0; x < img->getWidth(); x++)
					for (int comp = 0; comp < 3; comp++)
					{
						int val = img->getPixel(x, y, comp);
						pixels[y, x, comp] = val / 255.0f;
					}

					reader->release();
					test->release();

			return pixels;
		}

		void decodeJPEG(System::String ^filePath)
		{
			cli::array<wchar_t> ^buf = filePath->ToCharArray();
			pin_ptr<wchar_t> ptr = &buf[0];

			wstring path(ptr);
			JPEGCodec *test;
		}
	};
}
