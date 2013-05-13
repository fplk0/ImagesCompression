#include "stdafx.h"

#include "Atomic.h"

#include "JPEGCodec.h"

DEFINE_OBJECT_IMPL(self);

int JPEGCodec::_zig[64], JPEGCodec::_dezig[64];

void JPEGCodec::_fillZig()
{
	SFPerformOnceBegin
		
		int cur = 0;
		for (int i = 0; i < 8; i++)
		{
			int col, row, diff;
			if (i & 1)
			{
				col = i;
				row = 0;
				diff = 1;
			}
			else
			{
				row = i;
				col = 0;
				diff = -1;
			}
			for (int j = 0; j <= i; j++)
			{
				_zig[cur++] = row * 8 + col;
				col -= diff;
				row += diff;
			}
		}

		for (int i = 0; i < 7; i++)
		{
			int col, row, diff;
			if ((i & 1) == 0)
			{
				col = i+1;
				row = 7;
				diff = -1;
			}
			else
			{
				col = 7;
				row = i+1;
				diff = 1;
			}
			for (int j = 0; j < 7 - i; j++)
			{
				_zig[cur++] = row * 8 + col;
				col -= diff;
				row += diff;
			}
		}

		for (int i = 0; i < 64; i++)
			_dezig[_zig[i]] = i;
		
	SFPerformOnceEnd
}

JPEGCodec* JPEGCodec::init()
{
	super::initWithStreams(NULL, NULL);

	_fillZig();
	image = NULL;
	encodeQuality = 90;

	return this;
}

void JPEGCodec::free()
{
	image->release();
	image = NULL;

	super::free();
}

JPEGCodec::~JPEGCodec(void)
{

}
