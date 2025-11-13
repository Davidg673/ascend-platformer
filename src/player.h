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
	bool inAir = false;
	bool onWallRight = false;
	bool wasOnWallLeft = false;
	bool wasOnWallRight = false;
	bool onWallLeft = false;

	void checkForCollision(vector<Tile> tiles, vector<Trap> traps, vector<JumpPad>& pads);

	bool wantsToJump;
	bool wantsToDash;
	bool canDash = true;
	bool canDoubleJump;
	bool applyDashDamping;
	bool wantsToDrawDash = false;


	float jumpBufferTimer = 0.1f;
	float jumpBufferTime = 0.0f;
	Direction currentDirection=right;
	float directionFloat=0;
	float dashTimer = 0;
	float dashDampTimer = 0;
	float walkSoundTimer = 0;
	float distanceTraveled = 0;
	float dashImageAngle = 0;
	float dashImagePosX = 0;
	float dashImagePosY = 0;


	vector<ofImage> jumpSprite;
	vector<ofImage> idleSprite;
	vector<ofImage> walkSprite;
	vector<ofImage> fallSprite;
	vector<ofImage> dashSprite;


	ofSoundPlayer jumpSound;
	ofSoundPlayer walkSound;
	ofSoundPlayer landSound;
	ofSoundPlayer dashSound;
	ofSoundPlayer deathSound;
	ofSoundPlayer jumpPadSound;

	void drawDashSprite(float deltaTime);


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

	float currentFrame = 0;
	float animationSpeed = 3.0f;
	float dashAnimationSpeed = 15.0f;
	float dashAnimationFrame = 0;



	static const int SPRITE_OFFSET_JUMP = 4;
	static const int SPRITE_OFFSET_WALK = 6;
	static const int SPRITE_OFFSET_IDLE = 5;
	static const int SPRITE_OFFSET_FALL = 4;


	void drawPlayer();

	void update(float deltaTime, vector<Tile> tiles, vector<Trap> traps, vector<JumpPad>& pads);

	void jump();
	void doubleJump();

	void dash();

	Player(float width, float height);

	void loadPlayerData(ofVec2f pos);

	void checkInput();

	void animateSprite(float deltaTime, vector<ofImage> & spriteList,float startIndex, float endIndex, float isJumping=false);

};
