#include "Tile.hpp"


Tile::Tile()
{
}


void Tile::init(Tilemap* pMap, int id, bool walk)
{
	m_pTilemap = pMap;
	m_id = id;
	m_walkable = walk;
}