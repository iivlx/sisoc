#pragma once

#include <SDL.h>

#include "vector.h"
#include "Window.h"
#include "Tile.h"
#include "IsometricGrid.h"

class Game {
public:
  Window* window;
  IsometricGrid* grid;

  Game(Window* w);
  ~Game();
  bool quit;
  void mainloop();
  void processEvents();
  void handleKeyPress(SDL_Keycode key);
  void handleMousePress(SDL_MouseButtonEvent b);
  void handleMouseMotion();
  void handleMouseUp();
  void handleMouseScroll();
  bool mouseInTile(Tile* t);

private:
  SDL_Event event;

};