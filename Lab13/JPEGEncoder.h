#pragma once

#include "GenericEncoder.h"
#include "Image.h"

#undef super
#undef self
#define super GenericEncoder
#define self JPEGEncoder

class JPEGEncoder :
	public GenericEncoder
{
	DEFAULT_METACLASS_DECLARATION(self, super);

	Image *image;

public:
	
	virtual JPEGEncoder* init();

	Image* getImage() { return image; };
	void setImage(Image *img) {  }

	virtual void runEncode();
	virtual void runDecode();

	virtual ~JPEGEncoder(void);
};

