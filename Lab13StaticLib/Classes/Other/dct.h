#pragma once

#undef __SF_DCT_EXTERN__

#ifdef __DEFINE_DCT_EXTERN__
#define __SF_DCT_EXTERN__
#else
#define __SF_DCT_EXTERN__ extern
#endif

__SF_DCT_EXTERN__ float SFdctMatrix[8][8];
__SF_DCT_EXTERN__ float SFidctMatrix[8][8];

void initDCT();

inline void perform1DDCT(float src[8], float dst[8], bool inv)
{
	float (*dctMatr)[8];
	if (inv)
		dctMatr = SFidctMatrix;
	else
		dctMatr = SFdctMatrix;
	for (int j = 0; j < 8; j++)
	{
		float sum = 0;
		for (int k = 0; k < 8; k++)
			sum += src[k] * dctMatr[j][k];
		dst[j] = sum;
	}
}

inline void perform1DDCT(float src[8], float dst[8])
{
	perform1DDCT(src, dst, false);
}

inline void perform1DIDCT(float src[8], float dst[8])
{
	perform1DDCT(src, dst, true);
}

#undef __SF_DCT_EXTERN__