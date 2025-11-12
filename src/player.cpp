#include "player.h"

void Player::drawPlayer() {
	float deltaTime = ofGetLastFrameTime();

	/***************************************Debugging
		ofNoFill();
	ofSetColor(255, 255, 255);
	ofDrawRectangle(playerRect);

	ofFill();
	*/
	if (currentDirection == Direction::right || currentDirection == Direction::rightUp || currentDirection == Direction::rightDown) directionFloat = 1;
	if (currentDirection == Direction::left || currentDirection == Direction::leftUp || currentDirection == Direction::leftDown) directionFloat = -1;

	if (playerVel.y > 1.0f)
	{
		if (directionFloat > 0)
			animateSprite(deltaTime, fallSprite, 0, SPRITE_OFFSET_FALL);
		else
			animateSprite(deltaTime, fallSprite, SPRITE_OFFSET_FALL, fallSprite.size() - 1);
		return;
	}

	if (playerVel.y < -1.0f) {
		if (directionFloat > 0)
			animateSprite(deltaTime, jumpSprite, 0, SPRITE_OFFSET_JUMP-1,true);
		else
			animateSprite(deltaTime, jumpSprite, SPRITE_OFFSET_JUMP, jumpSprite.size() - 1,true);
		return;

	}

	if (abs(playerVel.x) < 150.0f)
	{
		if (directionFloat>0)
			animateSprite(deltaTime,idleSprite,0,SPRITE_OFFSET_IDLE-1);
		else
			animateSprite(deltaTime, idleSprite, SPRITE_OFFSET_IDLE,idleSprite.size()-1);
	}
	else
	{
		if (directionFloat > 0)
			animateSprite(deltaTime, walkSprite, 0, SPRITE_OFFSET_WALK);
		else
			animateSprite(deltaTime, walkSprite, SPRITE_OFFSET_WALK, walkSprite.size() - 1);
	}
}

void Player::update(float deltaTime, vector<Tile> tiles, vector<Trap> traps, vector<JumpPad>& pads) {

	checkInput();

	if (!onGround) {
		if (!onWallLeft && !onWallRight)
			playerVel.y += gravity * deltaTime; // apply gravity
		else
			playerVel.y += gravity/3 * deltaTime; // apply wall slide

	}


	float damping = onGround ? groundDamping : airDamping;
	playerVel.x *= (1.0f - damping);


	if (wantsToMove) //change velocity
	{
		float speed = onGround ? moveSpeed : moveSpeedAir;
		if (currentDirection == Direction::right || currentDirection == Direction::rightUp || currentDirection == Direction::rightDown) directionFloat = 1;
		if (currentDirection == Direction::left || currentDirection == Direction::leftUp || currentDirection == Direction::leftDown) directionFloat = -1;

		playerVel.x += directionFloat * speed;
	}


	if (wantsToDash && canDash) {
		dash();
	}

	if (abs(playerVel.x) < 0.01f) playerVel.x = 0; //clamp velocity

	playerRect.setX(playerRect.getX() + playerVel.x * deltaTime);
	playerRect.setY(playerRect.getY() + playerVel.y * deltaTime); //apply velocity changes

	checkForCollision(tiles,traps,pads); //colision check

	//reset wasOnWalls
	wasOnWallLeft = onWallLeft;
	wasOnWallRight = onWallRight;

	if (wantsToJump) //handle jump logic
	{
		if (onGround || onWallRight || onWallLeft)
			jump();
		else if (canDoubleJump) {
			doubleJump();
		}
	}

	if (!onGround && !onWallLeft && !onWallRight) //reset jump 
	{
		wantsToJump = false;
	}

	//canDash timer
	if (dashTimer > 0)
	{
		dashTimer -= deltaTime;
	}

	if (dashDampTimer > 0)
	{
		dashDampTimer -= deltaTime;
	}
	else if (applyDashDamping)
	{
		playerVel.x = playerVel.x * 0.3f;
		playerVel.y = playerVel.y * 0.3f;
		applyDashDamping = false;
	}

}



void Player::jump() {

	playerVel.y = -jumpForce;
	if (onWallRight) playerVel.x = jumpForce;
	if (onWallLeft) playerVel.x = -jumpForce;

	wantsToJump = false;
	onGround = false;
}

void Player::doubleJump()
{
	playerVel.y = -jumpForce;
	canDoubleJump = false;
}

Player::Player(float width, float height) {

	ofRectangle rect = ofRectangle(0, 0, width, height);

	playerRect.set(rect);

	//Walk animation
	for (int x=0; x < SPRITE_OFFSET_WALK; x++)  //walking right animations 0-5
	{
		ofImage img;

		img.load(ofToDataPath("images/WalkAnimation/Walk" + ofToString(x) + ".png"));

		walkSprite.push_back(img);
	}

	for (int x = 0; x < SPRITE_OFFSET_WALK; x++)
	{  //walking left animation 6-11
		ofImage img;

		img.load(ofToDataPath("images/WalkAnimation/Walk" + ofToString(x+SPRITE_OFFSET_WALK) + ".png"));

		walkSprite.push_back(img);
	}

	//Idle animations
	for (int x = 0; x < SPRITE_OFFSET_IDLE; x++) //right facing
	{
		ofImage img;

		img.load(ofToDataPath("images/IdleAnimation/Idle" + ofToString(x) + ".png"));

		idleSprite.push_back(img);
	}

	for (int x = 0; x < SPRITE_OFFSET_IDLE; x++) //left facing
	{
		ofImage img;

		img.load(ofToDataPath("images/IdleAnimation/Idle" + ofToString(x + SPRITE_OFFSET_IDLE) + ".png"));

		idleSprite.push_back(img);
	}

	//jumping animation
	for (int x = 0; x <SPRITE_OFFSET_JUMP; x++) //right facing
	{
		ofImage img;

		img.load(ofToDataPath("images/JumpAnimation/Jump" + ofToString(x) + ".png"));

		jumpSprite.push_back(img);
	}

	for (int x = 0; x < SPRITE_OFFSET_JUMP; x++) //left facing
	{
		ofImage img;

		img.load(ofToDataPath("images/JumpAnimation/Jump" + ofToString(x + SPRITE_OFFSET_JUMP) + ".png"));

		jumpSprite.push_back(img);
	}

	
	//falling animation
	for (int x = 0; x < SPRITE_OFFSET_FALL; x++) //right facing
	{
		ofImage img;

		img.load(ofToDataPath("images/FallAnimation/Fall" + ofToString(x) + ".png"));

		fallSprite.push_back(img);
	}

	for (int x = 0; x < SPRITE_OFFSET_FALL; x++) //left facing
	{
		ofImage img;

		img.load(ofToDataPath("images/FallAnimation/Fall" + ofToString(x + SPRITE_OFFSET_FALL) + ".png"));

		fallSprite.push_back(img);
	}

}

void Player::loadPlayerData(ofVec2f pos) {
	playerResetPos = pos;

	playerRect.x = pos.x;
	playerRect.y = pos.y;
}

void Player::dash() {

	if (!canDash || dashTimer > 0) return;

	dashTimer = 0.7f;

	wantsToDash = false;
	canDash = false;

	float dirX = 0;
	float dirY = 0;

	switch (currentDirection) {
	case Player::right:
		dirX = 1;
		break;
	case Player::left:
		dirX = -1;
		break;
	case Player::up:
		dirY = -1;
		break;
	case Player::down:
		dirY = 1;
		break;
	case Player::rightUp:
		dirX = 1;
		dirY = -1;
		break;
	case Player::rightDown:
		dirX = 1;
		dirY = 1;
		break;
	case Player::leftUp:
		dirX = -1;
		dirY = -1;
		break;
	case Player::leftDown:
		dirX = -1;
		dirY = 1;
		break;
	default:
		break;
	}

	float dashBoost = ((dirX == 1 || dirX == -1) && dirY == 0) ? 1.5f : 1.0f; //makes horizontal dash more effective
	playerVel.x = dashAcceleration * dirX * dashBoost;
	playerVel.y = dashAcceleration * dirY * dashBoost;

	applyDashDamping = true;
	dashDampTimer = 0.2f;

}


void Player::checkInput() {


	wantsToMove = leftPressed || rightPressed;

	if (rightPressed) currentDirection = Direction::right;
	if (leftPressed) currentDirection = Direction::left;
	if (downPressed) currentDirection = Direction::down;
	if (upPressed) currentDirection = Direction::up;
	if (rightPressed && upPressed) currentDirection = Direction::rightUp;
	if (rightPressed && downPressed) currentDirection = Direction ::rightDown;
	if (leftPressed && upPressed) currentDirection = Direction::leftUp;
	if (leftPressed && downPressed) currentDirection = Direction ::leftDown;

	wantsToDash = dashPressed ? true : false;

	wantsToJump = (jumpPressed && !prevJumpPressed) ? true : false;

	prevJumpPressed = jumpPressed;
}


void Player::checkForCollision(vector<Tile> tiles, vector<Trap> traps, vector<JumpPad>& pads) {
	onGround = false; //set up for ground collision check
	onWallRight = false;
	onWallLeft = false;

	for (const auto & tile : tiles) {
		if (playerRect.intersects(tile.edges.top)) //ground collision
		{
			playerRect.setY(tile.edges.top.getY() - playerRect.getHeight() + 1);
			playerVel.y = 0;
			onGround = true;
			canDoubleJump = true;

			if (dashTimer < 0)
				canDash = true;
		} else if (playerRect.intersects(tile.edges.bottom)) //ceiling collision
		{
			playerRect.setY(tile.edges.bottom.getY() + 2);
			playerVel.y = 0;
		} else if (playerRect.intersects(tile.edges.left) && !leftPressed) //left wall collision
		{
			playerRect.setX(tile.edges.left.getX() - playerRect.getWidth() + 1);
			if (!wasOnWallLeft) {
				playerVel.x = 0;
				playerVel.y = 0;
			}

			onWallLeft = true;
		} else if (playerRect.intersects(tile.edges.right) && !rightPressed) //right wall collision
		{
			playerRect.setX(tile.edges.right.getX() + tile.edges.right.getWidth() - 1);
			playerVel.x = 0;
			if (!wasOnWallRight) {
				playerVel.x = 0;
				playerVel.y = 0;
			}

			onWallRight = true;
		}
	}

	if (playerRect.y < 0) {
		playerRect.y = 0;
		playerVel.y = 0;
	}

	if (playerRect.y > ofGetHeight()) {
		playerRect.y = playerResetPos.y;
		playerRect.x = playerResetPos.x;
	}

	for (const auto & trap : traps) {
		if (playerRect.intersects(trap.rectangle)) {
			playerRect.y = playerResetPos.y;
			playerRect.x = playerResetPos.x;
		}
	}

	for (auto & pad : pads)
	{
		if (playerRect.intersects(pad.rect))
		{
			pad.launching = true;
			pad.animationFrame = 0;
			playerVel.y = -jumpForce * 2.0f;
			canDoubleJump = true;
			canDash = true;
		}
	}
}


void Player::animateSprite(float deltaTime, vector<ofImage>& spriteList, float start, float end, float isJumping)
{
	if (currentFrame < start) currentFrame = start;

	currentFrame += animationSpeed * deltaTime;

	if (currentFrame > end)
	{
		currentFrame =  isJumping ? end : start;
	}
	currentFrame += animationSpeed * deltaTime;

	spriteList[currentFrame].draw(playerRect.x-10, playerRect.y, playerRect.width+20, playerRect.height);
}
