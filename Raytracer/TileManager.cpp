#include "TileManager.h";

TileManager::TileManager(int tileSize, int width, int height) : tileSize(tileSize) , screenWidth(width) , screenHeight(height), currentTile(0) {
	tileCountX = width  / tileSize + 1;
	tileCountY = height / tileSize + 1;

	tileCount  = tileCountX * tileCountY;
};

Tile TileManager::getTile() {
	Tile returnTile;

	returnTile.xStart = (currentTile % tileCountX) * tileSize;
	returnTile.yStart = (currentTile / tileCountX) * tileSize;

	returnTile.xFinish = returnTile.xStart + tileSize < screenWidth ? returnTile.xStart + tileSize : screenWidth;
	returnTile.yFinish = returnTile.yStart + tileSize < screenHeight ? returnTile.yStart + tileSize : screenHeight;

	currentTile = currentTile++ % tileCount;
	if (currentTile == 0) currentSample++;

	return returnTile;
};