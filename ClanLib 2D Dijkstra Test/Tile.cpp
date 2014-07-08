#include "Tile.hpp"


Tile::Tile()
{
	m_astarInfo = AStarInfo();
	m_mazeInfo = CustomMazeInfo();
}


void Tile::init(Tilemap* pMap, int id, bool walk)
{
	m_pTilemap = pMap;
	m_id = id;
	m_walkable = walk;
}


std::vector<Tile*> Tile::get_adjacent_tiles(bool walkable, bool diagonal)
{
	return m_pTilemap->get_adjacent_tiles(m_id, walkable, diagonal);
}


clan::Point Tile::get_array_pos()
{
	clan::Point retval;
	retval.y = m_id % (m_pTilemap->get_size());
	retval.x = m_id / (m_pTilemap->get_size());
	
	return retval;
}