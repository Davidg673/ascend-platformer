#include "map.h"


void Map::loadLevel(int levelIndex, int partIndex)
{
	currentLevel = Levels[levelIndex];
	currentPart = currentLevel[partIndex];

	tiles.clear();

	for (int row = 0; row < LEVEL_HEIGHT; row++)
	{
		for (int col = 0; col < LEVEL_WIDTH; col++) {
			char tileChar = currentPart[row][col];

			float x = col * TILE_SIZE;
			float y = row * TILE_SIZE;

			if (tileChar == '#')
			{
				Tile t;
				t.rect.set(x, y, TILE_SIZE, TILE_SIZE);
				t.type = tileChar;

				t.edges.top = ofRectangle(x+5,y,TILE_SIZE-10,2);
				t.edges.bottom = ofRectangle(x+5, y + TILE_SIZE-2, TILE_SIZE-10, 2);
				t.edges.left = ofRectangle(x, y+5, 2, TILE_SIZE-5); 
				t.edges.right = ofRectangle(x + TILE_SIZE-2, y+5, 2, TILE_SIZE-5); 

				tiles.push_back(t);

			} else if (tileChar == 'P')
			{
				playerPos.x = x;
				playerPos.y = y;
			}
		}
	}

}

void Map::renderLevel()
{
	ofSetColor(200, 200, 200);
	for (const auto& tile : tiles)
	{
		ofDrawRectangle(tile.rect.getX(),tile.rect.getY(),tile.rect.getWidth(), tile.rect.getHeight());

		// Draw edges for visualization
		ofSetColor(255, 0, 0); // Top edge in red
		ofDrawRectangle(tile.edges.top);

		ofSetColor(0, 255, 0); // Bottom edge in green
		ofDrawRectangle(tile.edges.bottom);

		ofSetColor(0, 0, 255); // Left edge in blue
		ofDrawRectangle(tile.edges.left);

		ofSetColor(255, 255, 0); // Right edge in yellow
		ofDrawRectangle(tile.edges.right);
	}
}

