#pragma once
#include "ofMain.h"
#include "map.h"


class Player {

private:

	enum Direction
	{
		right,
		left,
		up,
		down,
		rightUp,
		rightDown,
		leftUp,
		leftDown
	};

	float jumpForce = 500.0f;
	float gravity = 1800.0f;
	float moveSpeed = 25.0f;
	float moveSpeedAir = 10.0f;
	float dashAcceleration = 700.0f;
	float groundDamping=0.1f;
	float airDamping=0.02f;

	bool onGround = false;
	bool onWallRight = false;
	bool wasOnWallLeft = false;
	bool wasOnWallRight = false;
	bool onWallLeft = false;


	void checkForCollision(vector<Tile> tiles,vector<Trap> traps);

	bool wantsToJump;
	bool wantsToDash;
	bool canDash = true;
	bool canDoubleJump;

	float jumpBufferTimer = 0.1f;
	float jumpBufferTime = 0.0f;
	Direction currentDirection;
	float directionFloat=0;
	float dashTimer = 0;


public:

	bool leftPressed;
	bool rightPressed;
	bool upPressed;
	bool downPressed;
	bool dashPressed;
	bool jumpPressed;
	bool prevJumpPressed;

	ofVec2f playerVel;
	ofVec2f acceleration;
	ofVec2f playerResetPos;

	ofRectangle playerRect;

	bool wantsToMove;


	void drawPlayer();

	void update(float deltaTime, vector<Tile> tiles, vector<Trap> traps);

	void jump();
	void doubleJump();

	void dash();

	Player(float width, float height);

	void loadPlayerData(ofVec2f pos);

	void checkInput();

};
