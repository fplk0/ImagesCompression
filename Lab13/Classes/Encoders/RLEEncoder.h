#pragma once
#include "GenericEncoder.h"

#undef super
#undef self

#define super GenericEncoder

class RLEEncoder :
	public super
{
	START_METACLASS_DECLARATION(RLEEncoder, super);
	DEFINE_METACLASS_DEFAULT_ALLOC(RLEEncoder);
	END_METACLASS_DECLARATION(RLEEncoder, super);
private:

	void _flush(byte *buf, bool same, int count);

public:

	void runEncode();
	void runDecode();

	virtual ~RLEEncoder(void);
};

