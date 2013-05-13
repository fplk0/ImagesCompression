#include "stdafx.h"

#include <cmath>
#include "Atomic.h"

#define __DEFINE_DCT_EXTERN__

#include "dct.h"

#undef __DEFINE_DCT_EXTERN__

static const double PI = acos(-1.0);

void initDCT()
{
	SFPerformOnceBegin
	{
		for (int j = 0; j < 8; j++)
		{
			for (int k = 0; k < 8; k++)
			{
				SFdctMatrix[j][k] = 0.5f * cos(PI / 8 * (k + 0.5f) * j);
				SFidctMatrix[j][k] = 0.5f * cos(PI / 8 * (j + 0.5f) * k);
				if (j == 0)
					SFdctMatrix[j][k] *= 1 / sqrt(2.0f);
				if (k == 0)
					SFidctMatrix[j][k] *= 1 / sqrt(2.0f);
			}
		}
	}
	SFPerformOnceEnd
}