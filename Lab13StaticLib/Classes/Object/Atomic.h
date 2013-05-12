#pragma once

#include <Windows.h>

#define SFCompareAndSwap(oldValue, newValue, address) (InterlockedCompareExchange(address, newValue, oldValue) == oldValue)
#define SFCompareAndSwap32(oldValue, newValue, address) (InterlockedCompareExchange(address, newValue, oldValue) == oldValue)
#define SFCompareAndSwap64(oldValue, newValue, address) (InterlockedCompareExchange64(address, newValue, oldValue) == oldValue)
#define SFSleep Sleep

const int SFONCE_NON_STARTED  = 0;
const int SFONCE_IN_PROGRESS  = 1;
const int SFONCE_COMPLETED  = 2;

#define SFPerformOnceBegin \
{\
	static unsigned int SF_ONCE_VAR = SFONCE_NON_STARTED; \
	volatile unsigned int *SF_ONCE_VAR_PTR = (volatile unsigned int*)&(SF_ONCE_VAR); \
	if (*SF_ONCE_VAR_PTR != SFONCE_COMPLETED) \
	{\
		if (*SF_ONCE_VAR_PTR == SFONCE_IN_PROGRESS) \
		{ \
			long long itCnt = 0; \
			while (*SF_ONCE_VAR_PTR == SFONCE_IN_PROGRESS) \
			{\
				itCnt++;\
				if (itCnt > 100000) \
					SFSleep(1); \
			}\
		}\
		else if (SFCompareAndSwap32(SFONCE_NON_STARTED, SFONCE_IN_PROGRESS, SF_ONCE_VAR_PTR)) \
		{

#define SFPerformOnceEnd \
			*SF_ONCE_VAR_PTR = SFONCE_COMPLETED;\
		} \
		else \
		{ \
			long long itCnt = 0; \
			while (*SF_ONCE_VAR_PTR == SFONCE_IN_PROGRESS) \
			{\
				itCnt++;\
				if (itCnt > 100000) \
					SFSleep(1); \
			}\
		}\
	} \
}\

