#include "player.h"

void Player::drawPlayer()
{
	float deltaTime = ofGetLastFrameTime();

	if (currentDirection == Direction::right || currentDirection == Direction::rightUp || currentDirection == Direction::rightDown) directionFloat = 1;
	if (currentDirection == Direction::left || currentDirection == Direction::leftUp || currentDirection == Direction::leftDown) directionFloat = -1;

	//check for drawing dash sprite
	if (wantsToDrawDash) {
		drawDashSprite(deltaTime);
	}

	///sprite list contains two types of iamges, right oriented and left oriented. right oriented comes first in the list

	if (playerVel.y > 1.0f)
	{
		if (directionFloat > 0)
			animateSprite(deltaTime, fallSprite, 0, SPRITE_OFFSET_FALL); 
		else
			animateSprite(deltaTime, fallSprite, SPRITE_OFFSET_FALL, fallSprite.size() - 1); 
		return;
	}

	if (playerVel.y < -1.0f) {
		if (directionFloat > 0) //direction left right of the player
			animateSprite(deltaTime, jumpSprite, 0, SPRITE_OFFSET_JUMP-1,true);  //if player jumps to the right, use firt part of list
		else
			animateSprite(deltaTime, jumpSprite, SPRITE_OFFSET_JUMP, jumpSprite.size() - 1,true); //player jumps to the left use second part
		return;

	}

	if (abs(playerVel.x) < 150.0f) //if player is not moving play idle animation
	{
		if (directionFloat>0)
			animateSprite(deltaTime,idleSprite,0,SPRITE_OFFSET_IDLE-1);  //righ facing
		else
			animateSprite(deltaTime, idleSprite, SPRITE_OFFSET_IDLE,idleSprite.size()-1); //left facing
	}
	else
	{ 
		if (directionFloat > 0) //if player is facing right and moving, animate sprite using first part of the sprite list
			animateSprite(deltaTime, walkSprite, 0, SPRITE_OFFSET_WALK);
		else
			animateSprite(deltaTime, walkSprite, SPRITE_OFFSET_WALK, walkSprite.size() - 1);
	}
}

void Player::update(float deltaTime, vector<Tile> tiles, vector<Trap> traps, vector<JumpPad>& pads) {

	checkInput();

	if (!onGround) {
		inAir = true;
		if (!onWallLeft && !onWallRight)
			playerVel.y += gravity * deltaTime; // apply gravity
		else
			playerVel.y += gravity/3 * deltaTime; // apply wall slide
	}
	else if (inAir) // if landed play sound
	{
		inAir = false;
		landSound.play();
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

	if (abs(playerVel.x) < 0.01f)
	{
		playerVel.x = 0; //clamp velocity
	}

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

	//handle Timers
	if (dashTimer > 0)
	{
		dashTimer -= deltaTime;
	}

	if (dashDampTimer > 0)
	{
		dashDampTimer -= deltaTime;
	}
	else if (applyDashDamping) {
		playerVel.x = playerVel.x * 0.3f;
		playerVel.y = playerVel.y * 0.3f;
		applyDashDamping = false;
	}

	//play walk sound
	distanceTraveled += abs(deltaTime * playerVel.x);

	if (abs(playerVel.x) > 0.01f && onGround && distanceTraveled >= 75.0f) {
		walkSound.play();
		walkSoundTimer = 0.5f;
		distanceTraveled = 0;
	}
}

void Player::jump() {

	playerVel.y = -jumpForce;
	if (onWallRight) playerVel.x = jumpForce;
	if (onWallLeft) playerVel.x = -jumpForce;

	wantsToJump = false;
	onGround = false;

	jumpSound.play();
}

void Player::doubleJump()
{
	playerVel.y = -jumpForce;
	canDoubleJump = false;

	jumpSound.play();
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

	//dash animation
	for (int x = 0; x < 11; x++) //right facing
	{
		ofImage img;

		img.load(ofToDataPath("images/dash/Dash" + ofToString(x) + ".png"));

		dashSprite.push_back(img);
	}


	//load sounds
	jumpSound.load(ofToDataPath("sounds/jump.wav"));
	jumpSound.setMultiPlay(false);
	jumpSound.setVolume(0.03f);


	walkSound.load(ofToDataPath("sounds/walk.wav"));
	walkSound.setVolume(0.05f);
	walkSound.setMultiPlay(false);


	landSound.load(ofToDataPath("sounds/land.wav"));
	landSound.setMultiPlay(false);
	landSound.setVolume(0.2f);

	dashSound.load(ofToDataPath("sounds/dash.wav"));
	dashSound.setMultiPlay(false);
	dashSound.setVolume(0.1f);

	deathSound.load(ofToDataPath("sounds/death.ogg"));
	deathSound.setMultiPlay(false);
	deathSound.setVolume(0.1f);

	jumpPadSound.load(ofToDataPath("sounds/jumpPad.wav"));
	jumpPadSound.setMultiPlay(false);
	jumpPadSound.setVolume(0.08f);
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
	float angle = 0;

	switch (currentDirection) {
		case Player::right:
			dirX = 1;
			angle = 0.0f;
			break;
		case Player::down:
			dirY = 1;
			angle = 90.0f;
			break;
		case Player::left:
			dirX = -1;
			angle = 180.0f;
			break;
		case Player::up:
			dirY = -1;
			angle = 270.0f;
			break;
		case Player::rightDown:
			dirX = 1;
			dirY = 1;
			angle = 45.0f;
			break;
		case Player::leftDown:
			dirX = -1;
			dirY = 1;
			angle = 135.0f;
			break;
		case Player::leftUp:
			dirX = -1;
			dirY = -1;
			angle = 225.0f;
			break;
		case Player::rightUp:
			dirX = 1;
			dirY = -1;
			angle = 315.0f;
			break;
	}

	float dashBoost = ((dirX == 1 || dirX == -1) && dirY == 0) ? 1.5f : 1.0f; //makes horizontal dash more effective
	playerVel.x = dashAcceleration * dirX * dashBoost;
	playerVel.y = dashAcceleration * dirY * dashBoost;

	applyDashDamping = true;
	dashDampTimer = 0.2f;

	dashSound.play();

	dashImageAngle = fmod(angle + 180,360);

	dashImagePosX = playerRect.x;
	dashImagePosY = playerRect.y;

	wantsToDrawDash = true;
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

	if (playerRect.y < 0) {  //player hit ceiling
		playerRect.y = 0;
		playerVel.y = 0;
	}
	 
	if (playerRect.y > ofGetHeight()) { //player fell and died
		playerRect.y = playerResetPos.y;
		playerRect.x = playerResetPos.x;
		canDash = true;
		canDoubleJump = true;
		deathSound.play();
	}

	for (const auto & trap : traps) { //player hit a trap
		if (playerRect.intersects(trap.rectangle)) {
			playerRect.y = playerResetPos.y;
			playerRect.x = playerResetPos.x;
			canDash = true;
			canDoubleJump = true;
			deathSound.play();
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
			jumpPadSound.play();
		}
	}
}


void Player::animateSprite(float deltaTime, vector<ofImage>& spriteList, float startIndex, float endIndex, float isJumping)
{
	if (currentFrame < startIndex) currentFrame = startIndex; //if sprite rotated from left to right, reset to right to avoid stutters in animation

	currentFrame += animationSpeed * deltaTime;

	if (currentFrame > endIndex)
	{
		currentFrame =  isJumping ? endIndex : startIndex; //sets jump sprite one frame back to avoid stuttering
	}

	spriteList[currentFrame].draw(playerRect.x-10, playerRect.y, playerRect.width+20, playerRect.height);
}

void Player::drawDashSprite(float deltaTime)
{
	dashAnimationFrame += dashAnimationSpeed * deltaTime;

	if (dashAnimationFrame > dashSprite.size()) {
		dashAnimationFrame = 0;
		wantsToDrawDash = false;
		return;
	}

	ofImage currentImage = dashSprite[dashAnimationFrame];

	ofPushMatrix();

	float radians = ofDegToRad(dashImageAngle);

	float centerX = (dashImagePosX + playerRect.width / 2) + cos(radians) * 50;
	float centerY = (dashImagePosY + playerRect.height / 2) + sin(radians) * 50;

	ofTranslate(centerX, centerY);
	ofRotateDeg(dashImageAngle);

	currentImage.draw(-currentImage.getWidth()/2 , - currentImage.getHeight()/2 );

	ofPopMatrix();

}
