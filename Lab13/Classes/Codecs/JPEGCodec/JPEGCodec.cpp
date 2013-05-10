#include "stdafx.h"

#include "JPEGCodec.h"

DEFINE_OBJECT_IMPL(self);

JPEGCodec* JPEGCodec::init()
{
	super::init();

	image = NULL;

	return this;
}

void JPEGCodec::free()
{
	image->release();
	image = NULL;

	super::free();
}

JPEGCodec::~JPEGCodec(void)
{

}
