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

class Map {
private:
	const char ** currentPart;
	const char *** currentLevel;
	bool nextLevel = true;

	std::string tileImagePath = ofToDataPath("images/dirt.jpg");
	std::string tileImagePathTop=ofToDataPath("images/dirtTop.jpg");
	std::string trapImgagePath;

	ofImage dirtImg;
	ofImage dirtTopImg;
	ofImage trapImg;


	const int TILE_SIZE = 32;

public:
	int currentPartIndex=0;

	vector<Tile> tiles;
	ofVec2f playerPos;
	vector<Trap> traps;

	void loadLevel(); //next level or previous
	void renderLevel();
	bool checkBorderCollision(ofRectangle playerRect);
	int getLength(const char *** level);
};
