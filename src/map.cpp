#include "map.h"


void Map::loadLevel() {
	dirtImg.load(tileImagePath);
	dirtTopImg.load(tileImagePathTop);
	stoneImg.load(tileImageStonePath);
	spikeImg.load(spikeImagePath);

	for (int x = 0; x < 8;x++)
	{
		ofImage img;

		img.load(ofToDataPath("images/Traps/Saw"+ ofToString(x) + ".png"));

		sawSprite.push_back(img);
	}

	for (int x = 0; x < 8; x++) {
		ofImage img;

		img.load(ofToDataPath("images/JumpPad/JumpPad" + ofToString(x) + ".png"));

		jumpPadSprite.push_back(img);
	}

	for (int x = 0; x < 5; x++)
	{
		ofImage img;

		img.load(ofToDataPath("images/Background/Layer" + ofToString(x) + ".png"));

		backgroundSprite.push_back(img);
	}

	currentLevel = Levels[0];
	currentPart = currentLevel[currentPartIndex];

	tiles.clear();
	traps.clear();
	jumpPads.clear();

	for (int row = 0; row < LEVEL_HEIGHT; row++)
	{
		for (int col = 0; col < LEVEL_WIDTH; col++) {
			char tileChar = currentPart[row][col];

			float x = col * TILE_SIZE;
			float y = row * TILE_SIZE;

			if (tileChar == '#' || tileChar == '~' || tileChar == '|')
			{

				Tile t;
				t.rect.set(x, y, TILE_SIZE, TILE_SIZE);
				t.type = tileChar;

				t.edges.top = ofRectangle(x+10,y,TILE_SIZE-15,2);
				t.edges.bottom = ofRectangle(x+10, y + TILE_SIZE-2, TILE_SIZE-15, 2);
				t.edges.left = ofRectangle(x, y+10, 2, TILE_SIZE-15); 
				t.edges.right = ofRectangle(x + TILE_SIZE-2, y+10, 2, TILE_SIZE-15); 

				if (tileChar == '#') t.type ='#';
				if (tileChar == '~') t.type ='~';
				if (tileChar == '|') t.type = '|';

				tiles.push_back(t);

			}
			if (tileChar == 'P' && nextLevel) //if next level, spawn player at start position
			{
				playerPos.x = x;
				playerPos.y = y;
			}
			if (tileChar == 'p' && !nextLevel) //if previous level start player at end pos
			{
				playerPos.x = x;
				playerPos.y = y;
			}

			if (tileChar == 't')
			{
				float randomPhase = static_cast<float>(rand()) / RAND_MAX * TWO_PI;
				Trap newTrap = Trap { 0,true, false ,x,y,randomPhase};
				newTrap.type = "t";
				traps.push_back(newTrap);
			}

			if (tileChar == 'T') {
				float randomPhase = static_cast<float>(rand()) / RAND_MAX * TWO_PI;
				Trap newTrap = Trap { 0, false, true, x, y, randomPhase};
				newTrap.type = "T";
				traps.push_back(newTrap);
			}

			if (tileChar == 's')
			{
				Trap newTrap = Trap { 0, false, false, x, y};
				newTrap.type = "s";
				traps.push_back(newTrap);
			}

			if (tileChar == 'j') {
				JumpPad p;
				p.rect = ofRectangle(x, y+1, TILE_SIZE, TILE_SIZE);
				jumpPads.push_back(p);
			}
		}
	}

}

void Map::renderLevel()
{
	for (RainDrop drop : rainDrops)
	{
		float radians = ofDegToRad(90-drop.rotation);
		float x = drop.pos.x + cos(radians) * drop.length;
		float y = drop.pos.y + sin(radians) * drop.length;

		ofVec2f endPos = ofVec2f(x , y);
		ofDrawLine(drop.pos, endPos);
	}

	for (Tile tile : tiles)
	{

		ofDrawRectangle(tile.rect.getX(),tile.rect.getY(),tile.rect.getWidth(), tile.rect.getHeight());
		if (tile.type =='#') dirtImg.draw(tile.rect.getX(), tile.rect.getY(), tile.rect.getWidth(), tile.rect.getHeight());
		if (tile.type == '~') dirtTopImg.draw(tile.rect.getX(), tile.rect.getY(), tile.rect.getWidth(), tile.rect.getHeight());
		if (tile.type == '|') stoneImg.draw(tile.rect.getX(), tile.rect.getY(), tile.rect.getWidth(), tile.rect.getHeight());
	}

	for (Trap trap : traps)
	{
		if (trap.type == "s") spikeImg.draw(trap.rectangle.x, trap.rectangle.y, trap.rectangle.width, trap.rectangle.height);
		if (trap.type == "t" || trap.type == "T") animateSprite(ofGetLastFrameTime(), sawSprite, currentSawFrame, sawAnimationSpeed, trap.rectangle);

		/****************************************************************Debugging
		ofNoFill();
		ofSetColor(255, 0, 0); 
		ofDrawRectangle(trap.rectangle);

		ofFill();
		*/
	}

	for (JumpPad& pad : jumpPads)
	{
		if (pad.launching) {
			if (pad.animationFrame < 6)
				animateSprite(ofGetLastFrameTime(), jumpPadSprite, pad.animationFrame, jumpPadAnimationSpeed, pad.rect);
			else {
				pad.launching = false;
				pad.animationFrame = 7;
			}
		} else {
			jumpPadSprite[pad.animationFrame].draw(pad.rect.x, pad.rect.y , pad.rect.width, pad.rect.height);
		}
	}
}

bool Map::checkBorderCollision(ofRectangle playerRect, bool& running) //returns true if the player moves between levels
{
	if (playerRect.position.x < 0)
	{
		if (currentPartIndex > 0)
		{
			currentPartIndex--;
		}
		else
		{
			return false;
			playerRect.x = 0;
		}
		nextLevel = false;
		return true;
	}

	if (playerRect.position.x > ofGetWidth())
	{
		int levelPartsLength = getLength(currentLevel);
		if (currentPartIndex < levelPartsLength-1)
		{
			currentPartIndex++;
		} else
		{
			running = false;
			return false;
		}
		nextLevel = true;
		return true;
	}

	return false;
}

int Map::getLength(const char *** level) {
	int count = 0;
	while (level[count] != nullptr)
		count++;
	return count;
}


void Map::animateSprite(float deltaTime, vector<ofImage> & spriteList , float& currentFrame, float animationSpeed, ofRectangle rectangle) {
	currentFrame += jumpPadAnimationSpeed * deltaTime;

	if (currentFrame > spriteList.size()-1) {
		currentFrame = 0.0f;
	}
	currentFrame += animationSpeed * deltaTime;

	if (currentFrame >= 0 && currentFrame < spriteList.size())
		spriteList[currentFrame].draw(rectangle.x, rectangle.y, rectangle.width, rectangle.height);
}


void Map::moveBackground(ofVec2f playerVel)
{
	backgroundSprite[0].draw(0, 0, ofGetWidth(), ofGetHeight());

	float moveVel =-0.5f;



	firstBackgroundPos += moveVel;
	firstBackgroundCopyPos += moveVel;
	secondBackgroundPos += moveVel * 0.5f; 
	secondBackgroundCopyPos += moveVel * 0.5f;

	if (firstBackgroundPos + ofGetWidth() < 0) //if right corner of original image goes beyond left border
		firstBackgroundPos += ofGetWidth()*2;   //set left corner to right border 

	if (firstBackgroundCopyPos + ofGetWidth() < 0) 
		firstBackgroundCopyPos += ofGetWidth()*2;

	if (secondBackgroundPos + ofGetWidth() < 0) //if right corner of original image goes beyond left border
		secondBackgroundPos += ofGetWidth()*2; //set left corner to right border

	if (secondBackgroundCopyPos + ofGetWidth() < 0) 
		secondBackgroundCopyPos += ofGetWidth()*2;

		
	backgroundSprite[1].draw(firstBackgroundPos, 0, ofGetWidth(), ofGetHeight());
	backgroundSprite[2].draw(firstBackgroundCopyPos, 0, ofGetWidth(), ofGetHeight());

	backgroundSprite[3].draw(secondBackgroundPos, 0, ofGetWidth(), ofGetHeight());
	backgroundSprite[4].draw(secondBackgroundCopyPos, 0, ofGetWidth(), ofGetHeight());
}

void Map::setupRain()
{
	rainDrops.reserve(MAX_RAINDROPS);

	for (int x = 0; x < MAX_RAINDROPS; x++)
	{
		ofVec2f pos = ofVec2f(ofRandom(ofGetWidth()),ofRandom(-ofGetHeight(),0));
		float speed = ofRandom(3.0f,5.0f);
		float size = ofRandom(5.0f, 10.0f);

		rainDrops.push_back(RainDrop { pos, speed, size, -25 });
	}
}

void Map::updateRain()
{
	for (int x=0 ; x<rainDrops.size(); x++)
	{
		RainDrop& dropRef = rainDrops[x];
		float radians = ofDegToRad(90 - rainDrops[x].rotation);

		dropRef.pos.x += cos(radians) * dropRef.speed;
		dropRef.pos.y += sin(radians) * dropRef.speed;

		if (dropRef.pos.y > ofGetHeight())
		{
			//delete drop 
			rainDrops.erase(rainDrops.begin() + x);

			//make new drop
			ofVec2f pos = ofVec2f(ofRandom(ofGetWidth()), 0);
			float speed = ofRandom(3.0f, 5.0f);
			float size = ofRandom(5.0f, 10.0f);

			rainDrops.push_back(RainDrop { pos, speed, size, -25 });

		}
	}
}
