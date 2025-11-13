#pragma once
#include "levels.h"
#include "ofMain.h"
#include <vector>
#include "trap.h"

struct Edge {
	float x;
	float y;
	float width;
	float height;
};

struct TileEdges {
	ofRectangle top;
	ofRectangle bottom;
	ofRectangle left;
	ofRectangle right;
};

struct Tile {
	ofRectangle rect;
	char type;
	TileEdges edges;
};

struct JumpPad {
	ofRectangle rect;
	float animationFrame = 7.0f;
	bool launching = false;
};

struct RainDrop
{
	ofVec2f pos;
	float speed;
	float length;
	float rotation;
};


class Map {
private:
	const char ** currentPart;
	const char *** currentLevel;
	bool nextLevel = true;

	std::string tileImagePath = ofToDataPath("images/dirt.jpg");
	std::string tileImagePathTop=ofToDataPath("images/dirtTop.jpg");
	std::string tileImageStonePath = ofToDataPath("images/cobblestone.jpg");
	std::string spikeImagePath = ofToDataPath("images/Traps/Spike.png");

	ofImage dirtImg;
	ofImage dirtTopImg;
	ofImage stoneImg;
	vector<ofImage> sawSprite;
	vector<ofImage> jumpPadSprite;
	vector<ofImage> backgroundSprite;
	vector<RainDrop> rainDrops;
	ofImage spikeImg;

	
	ofSoundPlayer rainSound;

	float currentSawFrame = 0.0f;
	float sawAnimationSpeed = 5.0f;
	float jumpPadAnimationSpeed = 5.0f;

	float firstBackgroundPos=0;
	float firstBackgroundCopyPos=-ofGetWidth();
	float secondBackgroundPos=0;
	float secondBackgroundCopyPos = -ofGetWidth();


	const int TILE_SIZE = 32;
	const int MAX_RAINDROPS = 100;

public:

	int currentPartIndex=0;
	
	vector<Tile> tiles;
	ofVec2f playerPos;
	vector<Trap> traps;
	vector<JumpPad> jumpPads;

	void loadLevel(); //next level or previous
	void renderLevel();
	bool checkBorderCollision(ofRectangle playerRect, bool& running);
	int getLength(const char *** level);

	void animateSprite(float deltaTime, vector<ofImage> & spriteList, float & currentFrame, float animationSpeed, ofRectangle rectangle);

	void checkJumpPadCollision(ofRectangle& playerRect);
	void moveBackground(ofVec2f playerVel);

	void setupRain();
	void updateRain();
};
