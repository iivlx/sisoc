#include "Tile.h"




Tile::Tile(int r, int c, int w, int h)
{
	row = r;
	col = c;
	r = 0;
	g = 0;
	b = 0;
	a = 0;
	ht = 0;
	hr = 0;
	hl = 0;
	hb = 0;
	tw = w;
	th = h;
	calculateVertices();
}

void Tile::calculateVertices(void)
{
	x = (col - row) * tw / 2;
	y = (col + row) * th / 2;
	topx = x;
	topy = y - ht;
	rightx = x + tw / 2.f;
	righty = y + th / 2.f - hr;
	bottomx = x;
	bottomy = y + th - hb;
	leftx = x - tw / 2.f;
	lefty = y + th / 2.f - hl;

}