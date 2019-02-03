#ifndef EDGEBUCKET
#define EDGEBUCKET

class edgebucket {
public:
    int MaxY;
    int MinY;
    int x;
    float sign;
    int dX;
    int dY;
    double sum;

	edgebucket(){}

	edgebucket(int MaxY, int MinY, int x, float sign, int dX, int dY, double sum){
		this->MaxY = MaxY;
		this->MinY = MinY;
		this->x = x;
		this->sign = sign;
		this->dX = abs(dX);
		this->dY = abs(dY);
		this->sum = sum;
	}
};

#endif