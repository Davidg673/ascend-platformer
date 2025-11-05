#include "trap.h"

Trap::Trap(float rotation, bool isMovingX, bool isMovingY, float posX, float posY)
	: rotation(rotation),
	isMovingX(isMovingX),
	isMovingY(isMovingY)
{
	rectangle = ofRectangle(posX, posY, width, height);
	baseX = posX;
	baseY = posY;

	//image.load(imagePath);
}


void Trap::move(float maxRange, float speed,float deltaTime)
{
	float t = ofGetElapsedTimef() * speed;
	if (isMovingX) rectangle.x = baseX + sin(t) * maxRange;
	if (isMovingY) rectangle.y = baseY + sin(t) * maxRange;
}

