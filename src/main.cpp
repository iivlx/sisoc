#pragma once

#include <glew/glew.h>
#include <GL/GLU.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include "Window.h"
#include "Game.h"
#include "Tile.h"

int main(int argc, char* argv[]) {
  Window* wmain;
  Game* game;

  wmain = new Window();

  int error = wmain->init();
  if (error > 0) {
    wmain->quit();
    return error;
  }

  game = new Game(wmain);

  while (!game->quit) {
    game->mainloop();
  }
  wmain->quit();

  return 0;
}