#pragma once

#include "precomp.hpp"
#include "Tilemap.hpp"

class Tilemap;
class Tile
{
	private:
		int m_id;
		Tilemap* m_pTilemap;
		bool m_walkable;

	public:
		Tile();
		void init(Tilemap* pMap, int id, bool walk);


		bool is_walkable() { return m_walkable; }
		void set_walkable(bool b) { m_walkable = b; }
};