#include "map.h"


void Map::loadLevel()
{
	dirtImg.load(tileImagePath);
	dirtTopImg.load(tileImagePathTop);

	currentLevel = Levels[0];
	currentPart = currentLevel[currentPartIndex];

	tiles.clear();
	traps.clear();

	for (int row = 0; row < LEVEL_HEIGHT; row++)
	{
		for (int col = 0; col < LEVEL_WIDTH; col++) {
			char tileChar = currentPart[row][col];

			float x = col * TILE_SIZE;
			float y = row * TILE_SIZE;

			if (tileChar == '#' || tileChar == '~')
			{
				Tile t;
				t.rect.set(x, y, TILE_SIZE, TILE_SIZE);
				t.type = tileChar;

				t.edges.top = ofRectangle(x+6,y,TILE_SIZE-11,2);
				t.edges.bottom = ofRectangle(x+6, y + TILE_SIZE-2, TILE_SIZE-11, 2);
				t.edges.left = ofRectangle(x, y+6, 2, TILE_SIZE-11); 
				t.edges.right = ofRectangle(x + TILE_SIZE-2, y+6, 2, TILE_SIZE-11); 

				if (tileChar == '#') t.type ='#';
				if (tileChar == '~') t.type ='~';


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

			if (tileChar == 'S')
			{
				Trap newTrap = Trap { 0,true, false ,x,y};
				traps.push_back(newTrap);
			}

			if (tileChar == 's')
			{
				Trap newTrap = Trap { 0,false, false ,x,y};
				traps.push_back(newTrap);

			}
		}
	}

}

void Map::renderLevel()
{
	for (const auto& tile : tiles)
	{

		ofDrawRectangle(tile.rect.getX(),tile.rect.getY(),tile.rect.getWidth(), tile.rect.getHeight());
		if (tile.type =='#') dirtImg.draw(tile.rect.getX(), tile.rect.getY(), tile.rect.getWidth(), tile.rect.getHeight());
		if (tile.type == '~') dirtTopImg.draw(tile.rect.getX(), tile.rect.getY(), tile.rect.getWidth(), tile.rect.getHeight());

	}

	for (const auto& trap : traps)
	{
		ofSetColor(255, 0, 0);
		ofDrawRectangle(trap.rectangle);

	}
}

bool Map::checkBorderCollision(ofRectangle playerRect) //returns true if the player moves between levels
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

