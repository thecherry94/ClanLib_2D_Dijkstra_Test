#pragma once

#include "precomp.hpp"
#include "Tilemap.hpp"
#include "AStarInfo.hpp"

class AStarInfo;
class Tilemap;
class Tile
{
	private:
		int m_id;
		Tilemap* m_pTilemap;
		bool m_walkable;
		AStarInfo m_astarInfo;

	public:
		Tile();
		void init(Tilemap* pMap, int id, bool walk);

		int get_id() { return m_id; }

		bool is_walkable() { return m_walkable; }
		void set_walkable(bool b) { m_walkable = b; }

		std::vector<Tile*> get_adjacent_tiles(bool walkable, bool diagonal);

		AStarInfo& get_astar_info() { return m_astarInfo; }
};