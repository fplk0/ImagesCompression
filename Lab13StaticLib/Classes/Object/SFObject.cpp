#include "stdafx.h"
#include "SFObject.h"

#include "Atomic.h"

DEFINE_OBJECT_IMPL(SFObject);

SFObject* SFObject::init()
{
	return this;
}

SFObject* SFObject::copy()
{
	this->metaClass()->alloc();
	return this->metaClass()->alloc();
}

SFObject* SFObject::retain()
{
	if (this == NULL)
		return NULL;
	volatile unsigned int *retainPtr = (volatile unsigned int*)&(this->_retainCount);
	
	unsigned int origCnt, newCnt;

	do
	{
		origCnt = *retainPtr;
		newCnt = origCnt + 1;
	}
	while (!SFCompareAndSwap(origCnt, newCnt, retainPtr));


	return this;
}

void SFObject::release()
{
	if (this == NULL)
		return;
	volatile unsigned int *retainPtr = (volatile unsigned int*)&(this->_retainCount);
	
	unsigned int origCnt, newCnt;

	do
	{
		origCnt = *retainPtr;
		newCnt = origCnt - 1;
	}
	while (!SFCompareAndSwap(origCnt, newCnt, retainPtr));

	if (newCnt <= 0)
		this->free();
}

void SFObject::free()
{
	delete this;
}

SFObject::~SFObject(void)
{

}
