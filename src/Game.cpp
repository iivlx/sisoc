#include <iostream>

#include <glew/glew.h>
#include <GL/GLU.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include "Game.h"
#include "Tile.h"


Game::Game(Window* w) {
	window = w;
	event = SDL_Event();
	quit = false;

	selected = { 0,0,0 };

	width = GRID_WIDTH;
	height = GRID_HEIGHT;
	size = width * height;

	tiles = new Tile*[size];
	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++)
			tiles[(i*width)+j] = new Tile(i, j, tw, th);
}

void Game::mainloop() {
	processEvents();

	window->clear();
	window->translateCamera();
	drawTiles();
	window->display();
}

void Game::drawTiles() {
	Tile* t;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			t = tiles[(i * width) + j];
			if (isSelected(j, i))
				drawTileSelected(t);
			else
				drawTileWithOutline(t);
		}
	}
}

void Game::drawTileWithOutline(Tile* t) {
	drawTile(t);
	drawTileOutline(t);
}

void Game::drawTileOutline(Tile* t) {
	float x = (t->col - t->row) * tw / 2;
	float y = (t->col + t->row) * th / 2;
	float topx = x;
	float topy = y - t->ht;
	float rightx = x + tw / 2.f;
	float righty = y + th / 2.f - t->hr;
	float bottomx = x;
	float bottomy = y + th - t->hb;
	float leftx = x - tw / 2.f;
	float lefty = y + th / 2.f - t->hl;

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(1.f);
	window->drawQuad(topx, topy, rightx, righty, bottomx, bottomy, leftx, lefty, .57f, .7f, .286f);
}

void Game::drawTileSelected(Tile* t) {
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	window->drawQuad(t->topx, t->topy, t->rightx, t->righty, t->bottomx, t->bottomy, t->leftx, t->lefty, 1.f, 1.f, 1.f);
}

void Game::drawTile(Tile* t) {
	float r = 0.1f + (t->col * 0.1f);
	float g = 0.1f + (t->row * 0.1f);
	float b = 0.0f;

	color3f f = { 0.6f, 0.7373f, 0.3765f};

	color3f s = { 0.47f, 0.64f, 0.22f };
	color3f s2 = { 0.7373f, 0.8784f, 0.5f };
	color3f r1 = { 0.5f, 0.5f, 0.5f };

	color3f c1 = f;
	color3f c2 = f;

	c2.r = 0;
	c2.g = .5f;
	c2.b = .3f;
	int ht = t->ht;
	int hr = t->hr;
	int hb = t->hb;
	int hl = t->hl;

	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (ht == hr && hr == hb  && hb == hl) { // flat tile
		c1 = f;
		window->drawQuad(t->topx, t->topy, t->rightx, t->righty, t->bottomx, t->bottomy, t->leftx, t->lefty, c1.r, c1.g, c1.b);
	} else if (ht == hb) { // split v

		if (ht == hr) // right flat
			c1 = f;
		else if (ht > hr) // right slope down
			c1 = s2;
		else // right slope up
			c1 = s;

		if (ht == hl) // left flat
			c2 = f;
		else if (ht > hl) // left slope up
			c2 = s;
		else // left slope down
			c2 = s2;

		window->drawTriangle(t->topx, t->topy, t->rightx, t->righty, t->bottomx, t->bottomy, c1.r, c1.g, c1.b);
		window->drawTriangle(t->topx, t->topy, t->bottomx, t->bottomy, t->leftx, t->lefty, c2.r, c2.g, c2.b);
	}
	else if (hr == hl) { // split h
		if (ht == hr) // top flat
			c1 = f;
		else if (ht > hr) // top slope up
			c1 = s;
		else // top slope down
			c1 = s2;

		if (hb == hr) // bottom flat
			c2 = f;
		else if (hb > hr) // bottom slope up
			c2 = s;
		else // bottom slope down
			c2 = s2;

		window->drawTriangle(t->topx, t->topy, t->rightx, t->righty, t->leftx, t->lefty, c1.r, c1.g, c1.b);
		window->drawTriangle(t->rightx, t->righty, t->bottomx, t->bottomy, t->leftx, t->lefty, c2.r, c2.g, c2.b);
	}
	else { // quad
		if (ht == hr && ht > hl) // slope up top right
			c1 = s;
		else if (ht == hr && ht < hl) // slope down top right
			c1 = s2;
		else if (ht == hl && ht > hr) // slope up top left
			c1 = s2;
		else if (ht == hl && ht < hr) // slope down top left
			c1 = s;
		else
			c1 = { 0.6f, 0.76f, 0.36f };
		window->drawQuad(t->topx, t->topy, t->rightx, t->righty, t->bottomx, t->bottomy, t->leftx, t->lefty, c1.r, c1.g, c1.b);
	}
}

bool Game::isSelected(int x, int y) {
	return (x == selected.x && y == selected.y);
}

Tile* Game::getTileTop(Tile* t) {
	if (t->row == 0 || t->col == 0)
		return nullptr;
	return getTileAtRC(t->row-1, t->col-1);
}

Tile* Game::getTileTopRight(Tile* t) {
	if (t->row == 0)
		return nullptr;
	return getTileAtRC(t->row-1, t->col);
}

Tile* Game::getTileTopLeft(Tile* t) {
	if (t->col == 0)
		return nullptr;
	return getTileAtRC(t->row, t->col - 1);
}

Tile* Game::getTileRight(Tile* t) {
	if (t->col == width)
		return nullptr;
	return getTileAtRC(t->row - 1, t->col + 1);
}

Tile* Game::getTileBottom(Tile* t) {
	if (t->col == width || t->row == height)
		return nullptr;
	return getTileAtRC(t->row + 1, t->col + 1);
}

Tile* Game::getTileBottomRight(Tile* t) {
	if (t->col == width)
		return nullptr;
	return getTileAtRC(t->row, t->col + 1);
}

Tile* Game::getTileBottomLeft(Tile* t) {
	if (t->row == height)
		return nullptr;
	return getTileAtRC(t->row + 1, t->col);
}

Tile* Game::getTileLeft(Tile* t){
	if (t->col == 0 || t->row >= height)
		return nullptr;
	return getTileAtRC(t->row + 1, t->col - 1);
}

Tile* Game::getTileAtXY(int x, int y) {
	if (x < 0 || y < 0 || x >= width || y >= height)
		return nullptr;
	else
		return tiles[y * width + x];
}

Tile* Game::getTileAtRC(int r, int c) {
	return getTileAtXY(c, r);
}

void Game::increaseTileHeight(Tile* t, int amount) {
	if (!t) return;
	increaseTileHT(t, amount);
	increaseTileHR(t, amount);
	increaseTileHB(t, amount);
	increaseTileHL(t, amount);
	t->calculateVertices();
}

void Game::increaseTileHT(Tile* t, int amount) {
	if (!t) return;

	Tile* top, * topright, * topleft;
	int max = t->ht - amount;
	top = getTileTop(t);
	topright = getTileTopRight(t);
	topleft = getTileTopLeft(t);

	if(t->hl <= max || (t->hr <= max))
		return;
	if (top)
		if (top->hl <= max || top->hr <= max)
			return;
	if (topright)
		if (topright->ht <= max || topright->hb <= max)
			return;
	if (topleft)
		if (topleft->ht <= max || topleft->hb <= max)
			return;

	t->ht += amount;
	if (top) {
		top->hb += amount;
		top->calculateVertices();
	}
	if (topright) {
		topright->hl += amount;
		topright->calculateVertices();
	}
	if (topleft) {
		topleft->hr += amount;
		topleft->calculateVertices();
	}
}

void Game::increaseTileHR(Tile* t, int amount) {
	if (!t) return;

	Tile* topright, * right, * bottomright;
	int max = t->hr - amount;
	topright = getTileTopRight(t);
	right = getTileRight(t);
	bottomright = getTileBottomRight(t);

	if (t->ht <= max || t->hb <= max)
		return;
	if (topright)
		if (topright->hl <= max || topright->hr <= max)
			return;
	if (right)
		if (right->ht <= max || right->hb <= max)
			return;
	if (bottomright)
		if (bottomright->hl <= max || bottomright->hr <= max)
			return;

	t->hr += amount;
	if (topright) {
		topright->hb += amount;
		topright->calculateVertices();
	}
	if (right) {
		right->hl += amount;
		right->calculateVertices();
	}
	if (bottomright) {
		bottomright->ht += amount;
		bottomright->calculateVertices();
	}
}

void Game::increaseTileHB(Tile* t, int amount) {
	if (!t) return;

	Tile* bottomright, * bottom, * bottomleft;
	int max = t->hb - amount;
	bottomright = getTileBottomRight(t);
	bottom = getTileBottom(t);
	bottomleft = getTileBottomLeft(t);

	if (t->hl <= max || t->hr <= max)
		return;
	if (bottomright)
		if (bottomright->ht <= max || bottomright->hb <= max)
			return;
	if (bottom)
		if (bottom->hl <= max || bottom->ht <= max)
			return;
	if (bottomleft)
		if (bottomleft->ht <= max || bottomleft->hb <= max)
			return;

	t->hb += amount;
	if (bottomright) {
		bottomright->hl += amount;
		bottomright->calculateVertices();
	}
	if (bottom) {
		bottom->ht += amount;
		bottom->calculateVertices();
	}
	if (bottomleft) {
		bottomleft->hr += amount;
		bottomleft->calculateVertices();
	}
}

void Game::increaseTileHL(Tile* t, int amount) {
	if (!t) return;

	Tile* bottomleft, * left, * topleft;
	int max = t->hl - amount;
	bottomleft = getTileBottomLeft(t);
	left = getTileLeft(t);
	topleft = getTileTopLeft(t);

	if (t->ht <= max || t->hb <= max)
		return;
	if (bottomleft)
		if (bottomleft->hl <= max || bottomleft->hr <= max)
			return;
	if (left)
		if (left->ht <= max || left->hb <= max)
			return;
	if (topleft)
		if (topleft->hl <= max || topleft->hr <= max)
			return;

	t->hl += amount;
	if (bottomleft) {
		bottomleft->ht += amount;
		bottomleft->calculateVertices();
	}
	if (left) {
		left->hr += amount;
		left->calculateVertices();
	}
	if (topleft) {
		topleft->hb += amount;
		topleft->calculateVertices();
	}
}

void Game::processEvents() {
	while (SDL_PollEvent(&event)) {
		SDL_Keycode key = event.key.keysym.sym;

		switch (event.type) {
		case SDL_QUIT:
			quit = true;
			break;
		case SDL_KEYDOWN:
			handleKeyPress(key);
			break;
		case SDL_MOUSEBUTTONDOWN:
			handleMousePress(event.button);
			break;
		case SDL_MOUSEBUTTONUP:
			handleMouseUp();
			break;
		case SDL_MOUSEMOTION:
			handleMouseMotion();
			break;
		case SDL_MOUSEWHEEL:
			handleMouseScroll();
			break;
		default:
			//std::cout << event.type << std::endl;
			break;
		}
	}
}

bool mouseInTile(Tile* t) {
	return false;
}

void Game::handleMousePress(SDL_MouseButtonEvent b) {
	if (b.button == SDL_BUTTON_RIGHT)
		;
	else
		window->setMouse(true);
}

void Game::handleMouseMotion() {
	if (window->mouseclicked)
		window->scrollCamera();
}

void Game::handleMouseUp() {
	window->scrollCamera();
	window->setMouse(false);
}

void Game::handleMouseScroll() {
	if (event.wheel.y > 0)
		window->zoom(1);
	else
		window->zoom(-1);
}

void Game::handleKeyPress(SDL_Keycode key)
{
	switch (key) {
	case SDLK_w:
		if(!(selected.y == 0))
			selected.y -= 1;
		break;
	case SDLK_s:
		if (!(selected.y == height - 1))
			selected.y += 1;
		break;
	case SDLK_a:
		if (!(selected.x == 0))
			selected.x -= 1;
		break;
	case SDLK_d:
		if (!(selected.x == width - 1))
			selected.x += 1;
		break;
	case SDLK_SPACE:
		increaseTileHeight(getTileAtXY(selected.x, selected.y), 8);
		break;
	case SDLK_r: // Reset Camera
		std::cout << "r" << std::endl;
		window->camx = 0;
		window->camy = 0;
		window->camzoom = 1;
		break;
	case SDLK_v:
		wireframe = !wireframe;
		break;
	}
}
