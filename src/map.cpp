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

				t.edges.top = { { x, y }, { x + TILE_SIZE, y } };
				t.edges.bottom = { { x, y + TILE_SIZE }, { x + TILE_SIZE, y + TILE_SIZE } };
				t.edges.left = { { x, y }, { x, y + TILE_SIZE } };
				t.edges.right = { { x + TILE_SIZE, y }, { x + TILE_SIZE, y + TILE_SIZE } };

				tiles.push_back(t);

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
		ofDrawLine(tile.edges.top.start, tile.edges.top.end);

		ofSetColor(0, 255, 0); // Bottom edge in green
		ofDrawLine(tile.edges.bottom.start, tile.edges.bottom.end);

		ofSetColor(0, 0, 255); // Left edge in blue
		ofDrawLine(tile.edges.left.start, tile.edges.left.end);

		ofSetColor(255, 255, 0); // Right edge in yellow
		ofDrawLine(tile.edges.right.start, tile.edges.right.end);
	}
}

