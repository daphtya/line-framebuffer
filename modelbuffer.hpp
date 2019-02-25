#ifndef MODEL
#define MODEL

#include "color.hpp"
#include "utils.hpp"
#include "framebuffer.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

class ModelBuffer : public IFrameBuffer
{
  private:
	color **buffer;
	long int buffersize;
	int width, height;
	int xOffset, yOffset;

  public:
	ModelBuffer(int width, int height, int xOffset, int yOffset)
	{
		this->width = width;
		this->height = height;
		this->xOffset = xOffset;
		this->yOffset = yOffset;
		this->buffersize = width * height;
		this->buffer = new color *[height];
		for (int i = 0; i < height; i++)
		{
			this->buffer[i] = new color[width];
		}
	}

	~ModelBuffer()
	{
		for (int i = 0; i < this->height; i++)
		{
			delete[] this->buffer[i];
		}
		delete[] this->buffer;
	}

	int getWidth() const { return this->width; }
	int getHeight() const { return this->height; }

	void clearScreen()
	{
		for (int i = 0; i < this->height; i++)
		{
			memset(this->buffer[i], 0, this->width * sizeof(color));
		}
	}

	void lazyDraw(Coordinate *coordinate, color c)
	{
		int x = coordinate->getX() - this->xOffset;
		int y = coordinate->getY() - this->yOffset;
		if (0 <= x && x < this->getWidth())
		{
			if (0 <= y && y < this->getHeight())
			{
				this->buffer[y][x] = c;
			}
		}
	}

	void lazyDraw(int x, int y, color c)
	{
		x -= this->xOffset;
		y -= this->yOffset;
		if (0 <= x && x < this->getWidth())
		{
			if (0 <= y && y < this->getHeight())
			{
				this->buffer[y][x] = c;
			}
		}
	}

	color lazyCheck(int x, int y)
	{
		x -= this->xOffset;
		y -= this->yOffset;
		if (x >= 0 && x < this->getWidth())
		{
			if (y >= 0 && y < this->getHeight())
			{
				return this->buffer[y][x];
			}
		}
		return CBLACK;
	}

	void floodfill(color c, Coordinate* start = NULL) {
		if (start == NULL) {
			floodrec(this->height/2, this->width/2, c);
		} else {
			floodrec(start->getY(), start->getX(), c);
		}

	}

	void floodrec(int i, int j, color c) {
		this->buffer[i][j] = c;
		if (i > 0 && this->buffer[i-1][j] == 0)
			floodrec(i-1, j, c);
		if (i < this->height-1 && this->buffer[i+1][j] == 0)
			floodrec(i+1, j, c);
		if (j > 0 && this->buffer[i][j-1] == 0)
			floodrec(i, j-1, c);
		if (j < this->width-1 && this->buffer[i][j+1] == 0)
			floodrec(i, j+1, c); 
	}

	void flush(FrameBuffer *fb, int zAxis = 1)
	{
		for (int i = 0; i < this->height; i++)
		{
			for (int j = 0; j < this->width; j++)
			{
				if (this->buffer[i][j] != 0)
				{
					Coordinate *coor = new Coordinate(this->xOffset + j, this->yOffset + i);
					fb->priorityDraw(coor, this->buffer[i][j], zAxis);
					delete coor;
				}
			}
		}
	}

	void debug()
	{
		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				flogcolor(this->buffer[y][x]);
			}
			flog("-");
		}
	}
};

#endif