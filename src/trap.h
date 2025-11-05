#pragma once
#include "ofMain.h"

class Trap
{
private:
	float width = 32.0f;
	float height = 32.0f;
	bool goingUp = true;

public:
	ofRectangle rectangle;

	float baseX;
	float baseY;

	ofImage image;
	float rotation;

	bool isMovingX;
	bool isMovingY;

	void move(float maxVal, float speed, float deltaTime);

	Trap(float rotation, bool isMovingX, bool isMovingY, float posX, float posY);

};
