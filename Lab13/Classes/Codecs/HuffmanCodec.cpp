#include "stdafx.h"
#include <queue>

#include "HuffmanCodec.h"

DEFINE_OBJECT_IMPL(HuffmanCodec);

typedef pair<long long, int> pll;

void HuffmanCodec::buildEncodeCodes()
{
	long long freqTable[256];

	memset(freqTable, 0, sizeof(freqTable));

	while (!reader->atEnd())
	{
		byte next;
		reader->readByte(next);
		freqTable[next]++;
	}

	priority_queue<pll> pq;

	for (int i = 0; i < 256; i++)
	{
		nodes[i].c = i;
		pq.push(pll(-freqTable[i], i));
	}

	int nodesCount = 256;

	for (int i = 0; i < 255; i++)
	{
		pll top = pq.top();
		pq.pop();
		pll top2 = pq.top();
		pq.pop();
		int nextNodeIndex = nodesCount++;

		int left = top.second, right = top2.second;
		if (left > right)
			swap(left, right);

		nodes[nextNodeIndex].leftChildIndex = left;
		nodes[nextNodeIndex].rightChildIndex = right;
		nodes[top.second].parentIndex = nextNodeIndex;
		nodes[top2.second].parentIndex = nextNodeIndex;

		pq.push(pll(top.first + top2.first, nextNodeIndex));
	}

	rootIndex = nodesCount - 1;
	nodes[rootIndex].parentIndex = -1;

	for (int i = 0; i < 256; i++)
	{
		codeLengths[i] = 0;
		int ci = i;
		int &cLen = codeLengths[i];
		while (true)
		{
			HuffmanTreeNode &cur = nodes[ci];
			if (cur.parentIndex == -1)
				break;
			int curCode = 1;
			if (nodes[cur.parentIndex].leftChildIndex == ci)
				curCode = 0;
			codes[i][cLen++] = curCode;
			ci = cur.parentIndex;
		}
		reverse(codes[i], codes[i] + cLen);
	}

	fwprintf(stderr, L"Table was built\n");
}

void HuffmanCodec::buildDecodeCodes()
{
	bool setLeft[512];
	memset(setLeft, 0, sizeof(setLeft));

	for (int i = 0; i < 510; i++)
	{
		int parInd;
		reader->readBits(parInd, 8);
		parInd += 256;
		nodes[i].parentIndex = parInd;
		if (setLeft[parInd] == 0)
		{
			setLeft[parInd] = 1;
			nodes[parInd].leftChildIndex = i;
		}
		else
			nodes[parInd].rightChildIndex = i;
	}

	rootIndex = 510;
}

void HuffmanCodec::runEncode()
{
	reader->openStream();

	this->buildEncodeCodes();
	reader->rewind();

	writer->openStream();

	for (int i = 0; i < rootIndex; i++)
	{
		writer->writeBits(nodes[i].parentIndex - 256, 8);
		//byte right = 1;
		//if (nodes[nodes[i].parentIndex].leftChildIndex == i)
		//	right = 0;
		//writer->writeBits(right, 1);
	}

	size_t fileSize = reader->streamSize();
	writer->writeLL(fileSize);

	while (!reader->atEnd())
	{
		byte nb;
		reader->readByte(nb);

		for (int i = 0; i < codeLengths[nb]; i++)
			writer->writeBits(codes[nb][i], 1);
	}

	reader->closeStream();
	writer->closeStream();
}

void HuffmanCodec::runDecode()
{
	reader->openStream();
	writer->openStream();

	this->buildDecodeCodes();
	unsigned long long fileSize;
	reader->readLL(fileSize);

	for (unsigned long long curInd = 0; curInd < fileSize; curInd++)
	{
		int cn = rootIndex;

		while(cn > 255)
		{
			int next;
			reader->readBits(next, 1);
			if (reader->atEnd())
				break;
			if (next)
				cn = nodes[cn].rightChildIndex;
			else
				cn = nodes[cn].leftChildIndex;
		}

		if (cn <= 255)
			writer->writeByte(cn);
		else
			break;
	}

	reader->closeStream();
	writer->closeStream();
}

HuffmanCodec::~HuffmanCodec(void)
{
}
