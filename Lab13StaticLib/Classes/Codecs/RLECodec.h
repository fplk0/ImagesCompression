#pragma once
#include "GenericCodec.h"

#undef super
#undef self

#define super GenericCodec

class RLECodec :
	public super
{
	START_METACLASS_DECLARATION(RLECodec, super);
	DEFINE_METACLASS_DEFAULT_ALLOC(RLECodec);
	END_METACLASS_DECLARATION(RLECodec, super);
private:

	void _flush(byte *buf, bool same, int count);

public:

	virtual void runEncode();
	virtual void runDecode();

	virtual ~RLECodec(void);
};

