#include "Tile.hpp"


Tile::Tile()
{
	m_astarInfo = AStarInfo();
}


void Tile::init(Tilemap* pMap, int id, bool walk)
{
	m_pTilemap = pMap;
	m_id = id;
	m_walkable = walk;
}


std::vector<Tile*> Tile::get_adjacent_tiles(bool walkable)
{
	return m_pTilemap->get_adjacent_tiles(m_id, walkable);
}