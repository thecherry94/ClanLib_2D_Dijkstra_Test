#include "Tilemap.hpp"
#include <fstream>


Tilemap::Tilemap()
{
	m_tileSize = 16;
}

void Tilemap::build_map(int size)
{
	reset();
	m_size = size;

	m_pTiles = new Tile**[m_size];
	for(int i = 0; i < m_size; i++)
	{
		m_pTiles[i] = new Tile*[m_size];
		for(int j = 0; j < m_size; j++)
		{
			m_pTiles[i][j] = new Tile();
			m_pTiles[i][j]->init(this, get_id_by_array_pos(i, j), true);
		}
	}

	make_rect_wall_by_arraypos(0, 0, 29, 29);
}


void Tilemap::draw(clan::Canvas c)
{
	for(int i = 0; i < m_size; i++)
	{
		for(int j = 0; j < m_size; j++)
		{
			Tile* current_tile = m_pTiles[i][j];
			clan::Colorf draw_color = current_tile->is_walkable() ? clan::Colorf::green : clan::Colorf::red;

			c.fill_rect(clan::Rect(clan::Point(i * m_tileSize, j * m_tileSize), clan::Size(m_tileSize, m_tileSize)), draw_color);
		}
	}
}


void Tilemap::reset()
{
	for(int i = 0; i < m_size; i++)
		delete[](m_pTiles[i]);

	m_size = 0;
}


int Tilemap::get_id_by_array_pos(int x, int y)
{
	return x * m_size + y;
}


Tile* Tilemap::get_tile_by_id(int id)
{
	if(id == 0)
		return m_pTiles[0][0];

	int x = id % (m_size);
	int y = id / (m_size);

	return m_pTiles[x][y];
}


std::vector<Tile*> Tilemap::get_adjacent_tiles(int id, bool walkable)
{
	int x = id % (m_size);
	int y = id / (m_size);
	std::vector<Tile*> retval;


	if(x != 0)
	{
		if(walkable)
		{
			if(m_pTiles[x-1][y]->is_walkable())
				retval.push_back(m_pTiles[x-1][y]);
		}
		else
		{
			retval.push_back(m_pTiles[x-1][y]);
		}
	}

	if(x != m_size - 1)
	{
		if(walkable)
		{
			if(m_pTiles[x+1][y]->is_walkable())
				retval.push_back(m_pTiles[x+1][y]);
		}
		else
		{
			retval.push_back(m_pTiles[x+1][y]);
		}
	}

	if(y != 0)
	{
		if(walkable)
		{
			if(m_pTiles[x][y-1]->is_walkable())
				retval.push_back(m_pTiles[x][y-1]);
		}
		else
		{
			retval.push_back(m_pTiles[x][y-1]);
		}
	}

	if(y != m_size - 1)
	{
		if(walkable)
		{
			if(m_pTiles[x][y+1]->is_walkable())
				retval.push_back(m_pTiles[x][y+1]);
		}
		else
		{
			retval.push_back(m_pTiles[x][y+1]);
		}
	}


	return retval;
}


void Tilemap::make_rect_wall(int start_id, int end_id)
{
	if(start_id > m_size * m_size || end_id > m_size * m_size)
		return;

	int start_x = start_id % (m_size);
	int start_y = start_id / (m_size);
	int end_x = end_id % (m_size);
	int end_y = end_id / (m_size);

	
	for(int i = start_x; i <= end_x; i++)
		m_pTiles[i][start_y]->set_walkable(false);

	for(int i = start_x; i <= end_x; i++)
		m_pTiles[i][end_y]->set_walkable(false);

	for(int i = start_y; i <= end_y; i++)
		m_pTiles[start_x][i]->set_walkable(false);

	for(int i = start_y; i <= end_y; i++)
		m_pTiles[end_x][i]->set_walkable(false);
	
}


void Tilemap::make_rect_wall_by_arraypos(int start_x, int start_y, int end_x, int end_y)
{
	make_rect_wall(get_id_by_array_pos(start_x, start_y), get_id_by_array_pos(end_x, end_y));
}


void Tilemap::fill_rect_wall(int start_id, int end_id)
{
	if(start_id > m_size * m_size || end_id > m_size * m_size)
		return;

	int start_x = start_id % (m_size);
	int start_y = start_id / (m_size);
	int end_x = end_id % (m_size);
	int end_y = end_id / (m_size);

	
	for(int i = start_x; i <= end_x; i++)
	{
		for(int j = start_y; j <= end_y; j++)
		{
			m_pTiles[i][j]->set_walkable(false);
		}
	}
	
}


std::vector<Tile*> Tilemap::get_path_dijkstra(int start_id, int end_id)
{

}


void Tilemap::fill_rect_wall_by_arraypos(int start_x, int start_y, int end_x, int end_y)
{
	fill_rect_wall(get_id_by_array_pos(start_x, start_y), get_id_by_array_pos(end_x, end_y));
}


clan::Point Tilemap::world_to_tilemap_position(float x, float y)
{
	return clan::Point(x / m_size, y / m_size);
}


int Tilemap::worldpos_to_tilemap_id(float x, float y)
{
	clan::Point p = world_to_tilemap_position(x, y);
	return get_id_by_array_pos(p.x, p.y);
}


Tile* Tilemap::get_tile_from_worldpos(float x, float y)
{
	return get_tile_by_id(worldpos_to_tilemap_id(x, y));
}