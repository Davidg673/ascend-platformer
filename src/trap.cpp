#include "trap.h"

Trap::Trap(float rotation, bool isMovingX, bool isMovingY, float posX, float posY, float phaseOffset)
	: rotation(rotation),
	isMovingX(isMovingX),
	isMovingY(isMovingY),
	phaseOffset(phaseOffset)
{
	rectangle = ofRectangle(posX, posY, width, height);
	baseX = posX;
	baseY = posY;
}


void Trap::move(float maxRange, float speed,float deltaTime)
{
	float t = ofGetElapsedTimef() * speed + phaseOffset;
	if (isMovingX) rectangle.x = baseX + sin(t) * maxRange;
	if (isMovingY) rectangle.y = baseY + sin(t) * maxRange;
}

