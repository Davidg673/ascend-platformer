#pragma once
#include "levels.h"
#include "ofMain.h"
#include <vector>

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


class Map
{
private:
	const char ** currentPart;
	const char *** currentLevel;

	const int TILE_SIZE = 32;


public:

	vector<Tile> tiles;
		ofVec2f playerPos;

	void loadLevel(int levelIndex, int partIndex);
	void renderLevel();

};
