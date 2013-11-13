#pragma once
#include "SFObject.h"
#include <string>
#include <algorithm>

using namespace std;

#undef super
#define super SFObject

#ifndef byte

typedef unsigned char byte;

#endif

class SFStream :
	public super
{
	START_METACLASS_DECLARATION(SFStream, super);
	DEFINE_METACLASS_DEFAULT_ALLOC(SFStream);
	END_METACLASS_DECLARATION(SFStream, super);

protected:
	byte *buf;
	size_t bitPos;
	size_t bufSize;
	bool _opened;

public:
	
	size_t bufPos;
	virtual SFStream* init();
	virtual SFStream* initWithBufSize(size_t _bufSize = 1<<14);
	virtual void openStream();
	virtual void closeStream();

	virtual void free();

	virtual ~SFStream(void);
};

