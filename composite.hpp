#ifndef COMPOSITE_HPP
#define COMPOSITE_HPP

#include <iostream>
#include <utility>

#include "polygon.hpp"


class Composite : public Drawable
{
private:
	Polygon** polygonList;
	int polygonNum;

public:
	Composite(int maxPolygonNum){
		this->polygonNum = 0;
		this->polygonList = new Polygon*[maxPolygonNum];
	}

	~Composite() {
		delete[] polygonList;
	}

	void insertPolygon(Polygon* pol) {
		if (this->polygonNum == 0) {
			this->polygonList[0] =  pol;
		} else {
			int i;
			for (i = this->polygonNum; i > 0; i--) {
				if (this->polygonList[i-1]->getZAxis() >= pol->getZAxis()) {
					this->polygonList[i] = pol;
					break;
				} else {
					this->polygonList[i] = this->polygonList[i-1];
				}
			}
			if (this->polygonList[0]->getZAxis() < pol->getZAxis()){
				this->polygonList[0] = pol;
			}
		}
	}

	void draw(IFrameBuffer* fb) {
		for (int i= 0; i < this->polygonNum; i++) {
			this->polygonList[i]->draw(fb);
		}
	}
	
};


#endif