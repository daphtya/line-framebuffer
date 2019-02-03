#ifndef FILLABLE
#define FILLABLE

#include "framebuffer.hpp"

class Fillable {
public:
	virtual void fill(FrameBuffer* framebuffer) = 0;
};

#endif