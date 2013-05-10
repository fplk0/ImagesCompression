#include "stdafx.h"
#include "Object.h"

#include "Atomic.h"

DEFINE_OBJECT_IMPL(Object);

Object* Object::init()
{
	return this;
}

Object* Object::copy()
{
	this->metaClass()->alloc();
	return this->metaClass()->alloc();
}

Object* Object::retain()
{
	if (this == NULL)
		return NULL;
	volatile UInt32 *retainPtr = (volatile UInt32*)&(this->_retainCount);
	
	UInt32 origCnt, newCnt;

	do
	{
		origCnt = *retainPtr;
		newCnt = origCnt + 1;
	}
	while (!SFCompareAndSwap(origCnt, newCnt, retainPtr));


	return this;
}

void Object::release()
{
	if (this == NULL)
		return;
	volatile UInt32 *retainPtr = (volatile UInt32*)&(this->_retainCount);
	
	UInt32 origCnt, newCnt;

	do
	{
		origCnt = *retainPtr;
		newCnt = origCnt - 1;
	}
	while (!SFCompareAndSwap(origCnt, newCnt, retainPtr));

	if (newCnt <= 0)
		this->free();
}

void Object::free()
{
	delete this;
}

Object::~Object(void)
{

}
