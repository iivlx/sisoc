#pragma once

struct RGBA
{
  int r, g, b, a;

};

class Tile
{
public:
  Tile(int r, int c, int w, int h);
  int r, g, b, a;
  int ht, hr, hl, hb;
  int row, col;
  int tw, th;
  
  float x, y;
  float topx, topy, rightx, righty, bottomx, bottomy, leftx, lefty;
  
  void calculateVertices(void);

};
