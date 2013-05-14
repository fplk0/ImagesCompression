#pragma once
#include "SFObject.h"

#undef super
#undef self
#define self SFTask
#define super SFObject

class SFTask :
	public SFObject
{
	ABSTRACT_METACLASS_DECLARATION(self, super);

public:
	virtual void runTask() = 0;
	virtual ~SFTask(void);
};

