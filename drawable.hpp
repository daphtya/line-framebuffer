#ifndef DRAWABLE
#define DRAWABLE

#include "framebuffer.hpp"

class Drawable {
public:
	virtual void draw(FrameBuffer* framebuffer) = 0;
    virtual void animate() { }
};

#endif