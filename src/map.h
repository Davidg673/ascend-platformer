#pragma once
#include "levels.h"
#include "ofMain.h"
#include <vector>

struct Edge {
	ofVec2f start;
	ofVec2f end;
};

struct TileEdges {
	Edge top;
	Edge bottom;
	Edge left;
	Edge right;
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

	void loadLevel(int levelIndex, int partIndex);
	void renderLevel();

};
