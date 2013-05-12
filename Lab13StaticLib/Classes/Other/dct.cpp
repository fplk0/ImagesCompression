#include "stdafx.h"

#include <cmath>
#include "Atomic.h"

static float dctMatrix[8][8];
static float idctMatrix[8][8];
static const double PI = acos(-1.0);

void initDCT()
{
	SFPerformOnceBegin
	{
		for (int j = 0; j < 8; j++)
		{
			for (int k = 0; k < 8; k++)
			{
				dctMatrix[j][k] = 0.5f * cos(PI / 8 * (k + 0.5f) * j);
				idctMatrix[j][k] = 0.5f * cos(PI / 8 * (j + 0.5f) * k);
				if (j == 0)
					dctMatrix[j][k] *= 1 / sqrt(2.0f);
				if (k == 0)
					idctMatrix[j][k] *= 1 / sqrt(2.0f);
			}
		}
	}
	SFPerformOnceEnd
}

void perform1DDCT(float src[8], float dst[8], bool inv)
{
	float (*dctMatr)[8];
	if (inv)
		dctMatr = idctMatrix;
	else
		dctMatr = dctMatrix;
	for (int j = 0; j < 8; j++)
	{
		float sum = 0;
		for (int k = 0; k < 8; k++)
			sum += src[k] * dctMatr[j][k];
		dst[j] = sum;
	}
}

void perform1DDCT(float src[8], float dst[8])
{
	perform1DDCT(src, dst, false);
}

void perform1DIDCT(float src[8], float dst[8])
{
	perform1DDCT(src, dst, true);
}