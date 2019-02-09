#ifndef PHYSICAL
#define PHYSICAL

#include "animated.hpp"
#include "framebuffer.hpp"
#include "utils.hpp"

#define GRAVITY_ACCEL 1
#define BOUNCE_COEFF -0.8
#define BOUNCE_THRESHOLD 10

class Physical : public Animated {
protected:
	// const int GRAVITY_ACCEL = 5;
	// const float BOUNCE_COEFF = -0.8; //when things bounce, the speed is multiplied by this constant
	// const int BOUNCE_THRESHOLD = 3;
	int xVelocity;
	int yVelocity;
	bool gravity;
	bool bounce;
	int drag;
	int dragerror;

public:
	Physical(std::string filename, color c, char id,
			 int horizonVelocity = 0,
			 int verticalVelocity = 0,
			 int drag = 0,
			 bool bounce = true,
			 bool gravity = true,
             double maxScaleVelocity = 0,
             double maxRotationVelocity = 0) :
	Animated(filename, c, id, false, pythagoreanApprox(horizonVelocity, verticalVelocity), maxScaleVelocity, maxRotationVelocity) {
		this->xVelocity = horizonVelocity;
		this->yVelocity = verticalVelocity;
		this->gravity = gravity;
		this->bounce = bounce;
		this->drag = drag;
		this->dragerror = 0;
	}

	void setBounce(bool bounce) {
		this->bounce = bounce;
	}

	void setGravity(bool grav) {
		this->gravity = grav;
	}

	void animate() {
		if (this->nextAnchorKeyframes > 0) { // if there is a target coordinate
			Coordinate* position = this->getCenter();
			int targetX = position->getX() + this->xVelocity;
			int targetY = position->getY() + this->yVelocity;

			// using protected attributes directly: beware potential problems!
			if (this->anchorKeyframes->size() == this->nextAnchorKeyframes && !(this->xVelocity == 0 && this->yVelocity == 0)) {
				Coordinate* target = new Coordinate(targetX, targetY);
				this->anchorKeyframes->push_back(target);
			}
			this->anchorKeyframes->at(this->nextAnchorKeyframes)->setX(targetX);
			this->anchorKeyframes->at(this->nextAnchorKeyframes)->setY(targetY);

			this->maxAnchorVelocity = pythagoreanApprox(this->xVelocity, this->yVelocity);

			if (this->gravity) {
				// if (this->dragerror < this->drag) {
				// 	this->dragerror ++;
				// } else {
				// 	this->dragerror = 0;
					this->yVelocity += GRAVITY_ACCEL;
				// }
			}
		}
		Animated::animate();
	}

	void draw(FrameBuffer* framebuffer) {
		Coordinate* position = this->getCenter();
		if (position->getX() <= 0 || position->getX() >= framebuffer->getXRes()) {
			if (!this->bounce /*|| abs(this->xVelocity) <= BOUNCE_THRESHOLD*/) {
				this->hide();
			} else {
				this->xVelocity *= BOUNCE_COEFF;
			}
		}
		if (position->getY() <= 0 || position->getY() >= framebuffer->getYRes()) {
			if (!this->bounce || (abs(this->yVelocity) < BOUNCE_THRESHOLD && position->getY() >= framebuffer->getYRes())) {
				this->hide();
			} else {
				this->yVelocity *= BOUNCE_COEFF;
			}
		}
		if (!this->hidden) {
			Animated::draw(framebuffer);
		}
	}
};

#endif