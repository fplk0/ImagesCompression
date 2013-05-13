#pragma once
#include "SFObject.h"

#undef self
#undef super

#define super SFObject
#define self SFData

#ifndef byte
typedef unsigned char byte;
#endif

class SFData :
	public SFObject
{
	DEFAULT_METACLASS_DECLARATION(self, super)

	byte *data;
	size_t len;
	bool _isAllocated;

public:
	
	virtual SFData* init();
	virtual SFData* initWithData(byte *src, size_t size, bool copy);
	virtual SFData* initWithSize(size_t size = 1<<10);

	inline byte *getData() { return data; };
	inline size_t getLength() { return len; } ;

	inline byte getByte(int ind) { return data[ind]; } ;
	inline void setByte(int ind, byte bt) { data[ind] = bt; } ;

	void resize(size_t newSize);

	virtual void free();
	virtual ~SFData(void);
};

