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
			mark_path = false;
		}

		int G;
		int H;
		Tile* previous_tile;
		bool visited;
		bool mark_path;

		int F() { return G + H; }

		void reset()
		{
			G = 0;
			H = 0;
			previous_tile = NULL;
			visited = false;
			mark_path = false;
		}
};