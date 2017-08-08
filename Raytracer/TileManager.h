#pragma once

struct Tile {
	int xStart;
	int yStart;

	int xFinish;
	int yFinish;
};

class TileManager {
public:
	TileManager(int tileSize, int width, int height);

	/* returns the current tile and increment the tile counter */
	Tile getTile();

private:
	int tileSize;

	/* how many tiles are in their respective axis */
	int tileCountX;
	int tileCountY;
	/* how many tiles there are in total */
	int tileCount;
	
	int screenWidth;
	int screenHeight;

	int currentTile;
	/* will be incremented each time the currentTile mod will return 0 */
	int currentSample = 1;
};
