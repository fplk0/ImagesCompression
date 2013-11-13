#include "stdafx.h"
#include "RLECodec.h"

DEFINE_OBJECT_IMPL(RLECodec);

void RLECodec::_flush(byte *buf, bool same, int count)
{
	if (count == 0)
		return;
	if (same)
		writer->writeByte(0x80 | (count-2));
	else
		writer->writeByte(count-1);
	if (same)
	{
		writer->writeByte(buf[0]);
	}
	else
	{
		for (int i = 0; i < count; i++)
			writer->writeByte(buf[i]);
	}
}

void RLECodec::runEncode()
{
	reader->openStream();
	writer->openStream();

	const int MAXSIZE = 128;
	int neededSameCount = 2;
	int MAXSAME = MAXSIZE + 1;
	byte buf[MAXSIZE+5];
	int pos = 0;
	bool same = false;
	int sameCount = 0;

	while (!reader->atEnd())
	{
		byte next;
		reader->readByte(next);
		switch (pos)
		{
			case 0:
			{
				buf[pos++] = next;
				sameCount = 1;
				same = 0;
			}
				break;
			default:
			{
				if (same)
				{
					if (next == buf[0])
						sameCount++;
					else
					{
						_flush(buf, same, sameCount);
						same = 0;
						sameCount = 1;
						pos = 0;
						buf[pos++] = next;
					}
				}
				else
				{
					if (next == buf[pos-1])
					{
						sameCount++;
					}
					else
						sameCount = 1;
					buf[pos++] = next;
					if (sameCount >= neededSameCount)
					{
						_flush(buf, same, pos - sameCount);
						buf[0] = next;
						pos = 1;
						same = 1;
					}
				}

				if (pos >= MAXSIZE || sameCount >= MAXSAME)
				{
					_flush(buf, same, same ? sameCount : pos);
					pos = 0;
					same = 0;
					sameCount = 0;
				}
			}
				break;
		}
	}

	_flush(buf, same, same ? sameCount : pos);

	reader->closeStream();
	writer->closeStream();
}

void RLECodec::runDecode()
{
	reader->openStream();
	writer->openStream();

	const int MAXSIZE = 128;
	int neededSameCount = 2;
	int MAXSAME = MAXSIZE + 2;

	while (!reader->atEnd())
	{
		byte count;
		reader->readByte(count);

		bool same = 0;
		if (count & 0x80)
		{
			same = 1;
			count = count ^ 0x80;
			count += 2;
		}
		else
			count++;

		if (same)
		{
			byte sb;
			reader->readByte(sb);
			for (int i = 0; i < count; i++)
			{
				writer->writeByte(sb);
			}
		}
		else
		{
			byte bb;
			for (int i = 0; i < count; i++)
			{
				reader->readByte(bb);
				writer->writeByte(bb);
			}
		}
	}

	reader->closeStream();
	writer->closeStream();
}

RLECodec::~RLECodec(void)
{
}
