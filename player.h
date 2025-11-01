#pragma once
#include "ofMain.h"
#include "map.h"


class Player
{

private:
	float jumpForce=300.0f;
	float gravity=1800.0f;
	float moveSpeed=300.0f;
	float dashSpeed=600.0f;

	bool onGround = false;


public:
	ofVec2f playerPos;
	ofVec2f playerVel;
	ofVec2f acceleration;

	ofRectangle playerRect;

	void DrawPlayer();

	void update(float deltaTime,vector<Tile> tiles);

	void jump();

	void dash();

	void checkForCollision(vector<Tile> tiles);

};
