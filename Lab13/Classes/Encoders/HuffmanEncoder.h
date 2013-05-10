#pragma once
#include "GenericEncoder.h"

#undef super
#define super GenericEncoder

class HuffmanEncoder :
	public GenericEncoder
{
	START_METACLASS_DECLARATION(HuffmanEncoder, super);
	DEFINE_METACLASS_DEFAULT_ALLOC(HuffmanEncoder);
	END_METACLASS_DECLARATION(HuffmanEncoder, super);

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
	virtual ~HuffmanEncoder(void);
};
