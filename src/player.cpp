#include "player.h"


void Player::checkForCollision(vector<Tile> tiles)
{
	onGround = false; //set up for ground collision check

	for (const auto& tile : tiles)
	{
		if (playerVel.y > 0 && playerRect.intersects(tile.edges.top)) //player is falling - ceiling collision
		{
			playerRect.setY(tile.edges.top.getY() - playerRect.getHeight());
			playerVel.y = 0;
			onGround = true;
		}
		else if (playerVel.y < 0 && playerRect.intersects(tile.edges.bottom)) //player is jumping - ground collision
		{
			playerRect.setY(tile.edges.bottom.getY() + tile.edges.top.getWidth());
			playerVel.y = 0;
		}
		else if (playerVel.x > 0 && playerRect.intersects(tile.edges.left)) //left wall collision
		{
			playerRect.setX(tile.edges.left.getX() - playerRect.getWidth());
			playerVel.x = 0;
		}
		else if (playerVel.x < 0 && playerRect.intersects(tile.edges.right)) //right wall collision
		{
			playerRect.setX(tile.edges.right.getX() + tile.edges.right.getWidth());
			playerVel.x = 0;
		}
	}

	if (playerRect.getX() < 0)
	{
		//TODO: handle map edge collision
	}
}

void Player::drawPlayer()
{
	ofSetColor(135, 206, 250);
	ofDrawRectangle(playerRect.getX(), playerRect.getY(), playerRect.getWidth(), playerRect.getHeight());
}

void Player::update(float deltaTime, vector<Tile> tiles)
{
	if (!onGround)
	{
		playerVel.y += gravity * deltaTime;  // apply gravity 
	}

	checkInput();

	float damping = onGround ? groundDamping : airDamping;
	playerVel.x *= (1.0f - damping);

	if (wantsToJump && onGround) //handle jump logic
	{
		playerVel.y = -jumpForce;
		wantsToJump = false;
		onGround = false;
	}

	if (wantsToMove) //change velocity
	{
		float speed = onGround ? moveSpeed : moveSpeedAir;
		playerVel.x +=(facingRight ? 1 : -1) * speed;
	}

	if (abs(playerVel.x) < 0.01f) playerVel.x = 0; //clamp velocity


	playerRect.setX(playerRect.getX() + playerVel.x * deltaTime);
	playerRect.setY(playerRect.getY() + playerVel.y * deltaTime); //apply velocity changes

	checkForCollision(tiles);  //colision check

	if (!onGround)  //reset jump intent
	{
		wantsToJump = false;
	}
}

void Player::jump()
{
	wantsToJump = true;

}

Player::Player(float width, float height)
{
	ofRectangle rect=ofRectangle(0,0, width, height);

	playerRect.set(rect);
}

void Player::loadPlayerData(ofVec2f pos)
{
	playerRect.x = pos.x;
	playerRect.y = pos.y;
}

void Player::checkInput()
{
	bool leftPressed = ofGetKeyPressed('a');
	bool rightPressed = ofGetKeyPressed('d');
	bool jumpPressed = ofGetKeyPressed(' ');

	wantsToMove = leftPressed || rightPressed;
	facingRight = rightPressed ? true : false;

	if (jumpPressed) jump();


}
