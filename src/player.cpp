#include "player.h"

void Player::drawPlayer() {
	ofSetColor(135, 206, 250);
	ofDrawRectangle(playerRect.getX(), playerRect.getY(), playerRect.getWidth(), playerRect.getHeight());
}

void Player::update(float deltaTime, vector<Tile> tiles, vector<Trap> traps) {


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

	checkForCollision(tiles,traps); //colision check

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

	playerVel.x = dashAcceleration * dirX;
	playerVel.y = dashAcceleration * dirY;

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


void Player::checkForCollision(vector<Tile> tiles, vector<Trap> traps) {
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
}
