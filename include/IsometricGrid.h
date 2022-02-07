#pragma once

#include "Window.h"
#include "vector.h"
#include "Tile.h"

const int GRID_WIDTH = 40;
const int GRID_HEIGHT = 40;

class IsometricGrid {

public:
	Window* window;

	Tile** tiles;

	vec3i selected;

	int width, height, size;

	float tw = 64;
	float th = 32;
	bool wireframe = false;

	IsometricGrid(Window* w);
	void drawTiles();
	void drawTile(Tile* t);
	void drawTileWithOutline(Tile* t);
	void drawTileSelected(Tile* t);
	void drawTileOutline(Tile* t);
	bool isSelected(int x, int y);
	Tile* getTileTop(Tile* t);
	Tile* getTileTopRight(Tile* t);
	Tile* getTileTopLeft(Tile* t);
	Tile* getTileRight(Tile* t);
	Tile* getTileBottom(Tile* t);
	Tile* getTileBottomRight(Tile* t);
	Tile* getTileBottomLeft(Tile* t);
	Tile* getTileLeft(Tile* t);
	Tile* getTileAtXY(int x, int y);
	Tile* getTileAtRC(int r, int c);
	void increaseTileHeight(Tile* t, int amount);
	void increaseTileHT(Tile* t, int amount);
	void increaseTileHR(Tile* t, int amount);
	void increaseTileHB(Tile* t, int amount);
	void increaseTileHL(Tile* t, int amount);

};
