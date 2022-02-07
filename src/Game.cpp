#include <iostream>

#include <glew/glew.h>
#include <GL/GLU.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include "IsometricGrid.h"
#include "Game.h"
#include "Tile.h"


Game::Game(Window* w) {
	window = w;
	event = SDL_Event();
	quit = false;

	grid = new IsometricGrid(w);
}

void Game::mainloop() {
	processEvents();

	window->clear();
	window->translateCamera();
	grid->drawTiles();
	window->display();
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
		if(!(grid->selected.y == 0))
			grid->selected.y -= 1;
		break;
	case SDLK_s:
		if (!(grid->selected.y == grid->height - 1))
			grid->selected.y += 1;
		break;
	case SDLK_a:
		if (!(grid->selected.x == 0))
			grid->selected.x -= 1;
		break;
	case SDLK_d:
		if (!(grid->selected.x == grid->width - 1))
			grid->selected.x += 1;
		break;
	case SDLK_SPACE:
		grid->increaseTileHeight(grid->getTileAtXY(grid->selected.x, grid->selected.y), 8);
		break;
	case SDLK_r: // Reset Camera
		std::cout << "r" << std::endl;
		window->camx = 0;
		window->camy = 0;
		window->camzoom = 1;
		break;
	case SDLK_v:
		grid->wireframe = !grid->wireframe;
		break;
	}
}
