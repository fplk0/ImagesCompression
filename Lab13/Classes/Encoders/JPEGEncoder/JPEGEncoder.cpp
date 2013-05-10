#include "stdafx.h"

#include "JPEGEncoder.h"

DEFINE_OBJECT_IMPL(self);

JPEGEncoder* JPEGEncoder::init()
{
	super::init();

	image = NULL;

	return this;
}

void JPEGEncoder::free()
{
	image->release();
	image = NULL;

	super::free();
}

JPEGEncoder::~JPEGEncoder(void)
{

}
