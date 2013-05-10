#pragma once

#include "GenericCodec.h"
#include "Image.h"

#undef super
#undef self
#define super GenericCodec
#define self JPEGCodec

class JPEGCodec :
	public GenericCodec
{
	DEFAULT_METACLASS_DECLARATION(self, super);

	Image *image;

	virtual void free();

public:
	
	virtual JPEGCodec* init();

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
	virtual ~JPEGCodec(void);
};

