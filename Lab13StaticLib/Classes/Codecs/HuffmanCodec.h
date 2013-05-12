#pragma once
#include "GenericCodec.h"

#undef super
#define super GenericCodec

class HuffmanCodec :
	public GenericCodec
{
	START_METACLASS_DECLARATION(HuffmanCodec, super);
	DEFINE_METACLASS_DEFAULT_ALLOC(HuffmanCodec);
	END_METACLASS_DECLARATION(HuffmanCodec, super);

	struct HuffmanTreeNode
	{
		byte c;
		int leftChildIndex, rightChildIndex, parentIndex;
	} nodes[512];

	int rootIndex;

	//Encoding
	byte codes[256][256];
	int codeLengths[256];

	void buildEncodeCodes();
	void buildDecodeCodes();

public:
	virtual void runEncode();
	virtual void runDecode();
	virtual ~HuffmanCodec(void);
};
