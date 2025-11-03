#pragma once
#include "ofMain.h"
#include "map.h"


class Player {

private:
	float jumpForce = 500.0f;
	float gravity = 1800.0f;
	float moveSpeed = 25.0f;
	float moveSpeedAir = 10.0f;
	float dashSpeed = 20.0f;
	float groundDamping=0.1f;
	float airDamping=0.02f;

	bool onGround = false;

	void checkForCollision(vector<Tile> tiles);

	bool wantsToJump;

	float jumpBufferTimer = 0.1f;
	float jumpBufferTime = 0.0f;

	bool facingRight;

public:
	ofVec2f playerVel;
	ofVec2f acceleration;

	ofRectangle playerRect;

	bool wantsToMove;


	void drawPlayer();

	void update(float deltaTime, vector<Tile> tiles);

	void jump();

	void dash();

	Player(float width, float height);

	void loadPlayerData(ofVec2f pos);

	void checkInput();
};
