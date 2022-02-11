#include <glew/glew.h>
#include <GL/GLU.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include "Vector.h"
#include "IsometricGrid.h"
#include "Tile.h"


color3f FLAT = { 0.6f, 0.7373f, 0.3765f };
color3f SLOPE_1 = { 0.47f, 0.64f, 0.22f };
color3f SLOPE_2 = { 0.7373f, 0.8784f, 0.5f };

struct IsometricGridColors {

};

IsometricGrid::IsometricGrid(Window* w) {
  selected = { 0,0,0 };

  width = GRID_WIDTH;
  height = GRID_HEIGHT;
  size = width * height;

  tiles = new Tile * [size];
  fillWithEmptyTiles();
}

void IsometricGrid::fillWithEmptyTiles() {
  for (int i = 0; i < width; i++)
    for (int j = 0; j < height; j++)
      tiles[(i * width) + j] = new Tile(i, j, tw, th);
}

void IsometricGrid::clearTiles() {
  Tile* t;
  for (int i = 0; i < width; i++)
    for (int j = 0; j < height; j++) {
      t = tiles[(i * width) + j];
      if (t) delete t;
    }
}


void IsometricGrid::drawTiles() {
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

void IsometricGrid::drawTileQuadColor(Tile* t, color3f c) {
  window->drawQuad(t->topx, t->topy, t->rightx, t->righty, t->bottomx, t->bottomy, t->leftx, t->lefty, c.r, c.g, c.b);
}

void IsometricGrid::drawTileWithOutline(Tile* t) {
  drawTile(t);
  drawTileOutline(t);
}

void IsometricGrid::drawTileOutline(Tile* t) {
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

void IsometricGrid::drawTileSelected(Tile* t) {
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  window->drawQuad(t->topx, t->topy, t->rightx, t->righty, t->bottomx, t->bottomy, t->leftx, t->lefty, 1.f, 1.f, 1.f);
}

bool IsometricGrid::isFlatTile(Tile* t) {
  return (t->ht == t->hr && t->hr == t->hb && t->hb == t->hl);
}

void IsometricGrid::drawTileSplitVertical(Tile* t) {
  color3f c1 = FLAT;
  color3f c2 = FLAT;

  if (t->ht == t->hr) // right flat
    c1 = FLAT;
  else if (t->ht > t->hr) // right slope down
    c1 = SLOPE_2;
  else // right slope up
    c1 = SLOPE_1;

  if (t->ht == t->hl) // left flat
    c2 = FLAT;
  else if (t->ht > t->hl) // left slope up
    c2 = SLOPE_1;
  else // left slope down
    c2 = SLOPE_2;

  this->window->drawTriangle(t->topx, t->topy, t->rightx, t->righty, t->bottomx, t->bottomy, c1.r, c1.g, c1.b);
  this->window->drawTriangle(t->topx, t->topy, t->bottomx, t->bottomy, t->leftx, t->lefty, c2.r, c2.g, c2.b);
}

void IsometricGrid::drawTileSplitHorizontal(Tile* t) {
  \
    color3f c1 = FLAT;
  color3f c2 = FLAT;

  if (t->ht == t->hr) // top flat
    c1 = FLAT;
  else if (t->ht > t->hr) // top slope up
    c1 = SLOPE_1;
  else // top slope down
    c1 = SLOPE_2;

  if (t->hb == t->hr) // bottom flat
    c2 = FLAT;
  else if (t->hb > t->hr) // bottom slope up
    c2 = SLOPE_1;
  else // bottom slope down
    c2 = SLOPE_2;

  window->drawTriangle(t->topx, t->topy, t->rightx, t->righty, t->leftx, t->lefty, c1.r, c1.g, c1.b);
  window->drawTriangle(t->rightx, t->righty, t->bottomx, t->bottomy, t->leftx, t->lefty, c2.r, c2.g, c2.b);
}

void IsometricGrid::drawTileQuad(Tile* t) {
  color3f c1 = FLAT;
  color3f c2 = FLAT;

  if (t->ht == t->hr && t->ht > t->hl) // slope up top right
    c1 = SLOPE_1;
  else if (t->ht == t->hr && t->ht < t->hl) // slope down top right
    c1 = SLOPE_2;
  else if (t->ht == t->hl && t->ht > t->hr) // slope up top left
    c1 = SLOPE_2;
  else if (t->ht == t->hl && t->ht < t->hr) // slope down top left
    c1 = SLOPE_1;
  else
    c1 = { 0.6f, 0.76f, 0.36f };
  window->drawQuad(t->topx, t->topy, t->rightx, t->righty, t->bottomx, t->bottomy, t->leftx, t->lefty, c1.r, c1.g, c1.b);

}

void IsometricGrid::drawTile(Tile* t) {

  color3f f = { 0.6f, 0.7373f, 0.3765f };

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

  if (isFlatTile(t)) { // flat tile
    drawTileQuadColor(t, f);
  }
  else if (ht == hb) { // split v
    drawTileSplitVertical(t);
  }
  else if (hr == hl) { // split h
    drawTileSplitHorizontal(t);
  }
  else { // quad
    drawTileQuad(t);
  }
}

bool IsometricGrid::isSelected(int x, int y) {
  return (x == selected.x && y == selected.y);
}

Tile* IsometricGrid::getTileTop(Tile* t) {
  if (t->row == 0 || t->col == 0)
    return nullptr;
  return getTileAtRC(t->row - 1, t->col - 1);
}

Tile* IsometricGrid::getTileTopRight(Tile* t) {
  if (t->row == 0)
    return nullptr;
  return getTileAtRC(t->row - 1, t->col);
}

Tile* IsometricGrid::getTileTopLeft(Tile* t) {
  if (t->col == 0)
    return nullptr;
  return getTileAtRC(t->row, t->col - 1);
}

Tile* IsometricGrid::getTileRight(Tile* t) {
  if (t->col == width)
    return nullptr;
  return getTileAtRC(t->row - 1, t->col + 1);
}

Tile* IsometricGrid::getTileBottom(Tile* t) {
  if (t->col == width || t->row == height)
    return nullptr;
  return getTileAtRC(t->row + 1, t->col + 1);
}

Tile* IsometricGrid::getTileBottomRight(Tile* t) {
  if (t->col == width)
    return nullptr;
  return getTileAtRC(t->row, t->col + 1);
}

Tile* IsometricGrid::getTileBottomLeft(Tile* t) {
  if (t->row == height)
    return nullptr;
  return getTileAtRC(t->row + 1, t->col);
}

Tile* IsometricGrid::getTileLeft(Tile* t) {
  if (t->col == 0 || t->row >= height)
    return nullptr;
  return getTileAtRC(t->row + 1, t->col - 1);
}

Tile* IsometricGrid::getTileAtXY(int x, int y) {
  if (x < 0 || y < 0 || x >= width || y >= height)
    return nullptr;
  else
    return tiles[y * width + x];
}

Tile* IsometricGrid::getTileAtRC(int r, int c) {
  return getTileAtXY(c, r);
}

void IsometricGrid::increaseTileHeight(Tile* t, int amount) {
  if (!t) return;
  increaseTileHT(t, amount);
  increaseTileHR(t, amount);
  increaseTileHB(t, amount);
  increaseTileHL(t, amount);
  t->calculateVertices();
}

void IsometricGrid::increaseSelectedTileHeight(int amount) {
  increaseTileHeight(getTileAtXY(selected.x, selected.y), 8);
}

void IsometricGrid::increaseTileHT(Tile* t, int amount) {
  if (!t) return;

  Tile* top, * topright, * topleft;
  int max = t->ht - amount;
  top = getTileTop(t);
  topright = getTileTopRight(t);
  topleft = getTileTopLeft(t);

  if (t->hl <= max || (t->hr <= max))
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

void IsometricGrid::increaseTileHR(Tile* t, int amount) {
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

void IsometricGrid::increaseTileHB(Tile* t, int amount) {
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

void IsometricGrid::increaseTileHL(Tile* t, int amount) {
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