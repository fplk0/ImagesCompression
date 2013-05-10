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

	virtual void free();

public:
	
	virtual JPEGEncoder* init();

	Image* getImage() { return image; };
	void setImage(Image *img) {  }

//Encoding
public:
	virtual void runEncode();
private:


//Decoding
public:
	virtual void runDecode();
private:


protected:
	virtual ~JPEGEncoder(void);
};

