#pragma once

#include "Tile.hpp"

class Tile;
class AStarInfo
{
	public:

		AStarInfo()
		{
			G = 0;
			H = 0;
			previous_tile = NULL;
			visited = false;
		}

		int G;
		int H;
		Tile* previous_tile;
		bool visited;

		int F() { return G + H; }
};