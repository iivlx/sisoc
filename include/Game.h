#pragma once

#include <SDL.h>

#include "Window.h"
#include "Tile.h"

const int GRID_WIDTH = 40;
const int GRID_HEIGHT = 40;

//struct 

struct color3f {
	float r, g, b;
};

struct color4f {
	float r, g, b, a;
};

struct vec3f {
	float x, y, z;
};

struct vec3i {
	int x, y, z;
};


class Game {
public:
	Window* window;

	Game(Window* w);
	bool quit;
	void mainloop();
	void processEvents();
	void handleKeyPress(SDL_Keycode key);
	void handleMousePress(SDL_MouseButtonEvent b);
	void handleMouseMotion();
	void handleMouseUp();
	void handleMouseScroll();
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
	bool mouseInTile(Tile* t);

private:
	Tile** tiles;
	SDL_Event event;

	vec3i selected;

	int width, height, size;

	float tw = 64;
	float th = 32;

	bool wireframe = false;
};