#include "Tilemap.hpp"
#include <fstream>


Tilemap::Tilemap()
{
	m_tileSize = 16;
	space = false;
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
}


void Tilemap::update(clan::InputContext ic)
{
	clan::InputDevice mouse = ic.get_mouse();
	clan::InputDevice kb = ic.get_keyboard();

	if(mouse.get_keycode(clan::mouse_left))
	{
		get_tile_from_worldpos(mouse.get_position().x, mouse.get_position().y)->set_walkable(false);
	}
	else if(mouse.get_keycode(clan::mouse_right))
	{
		get_tile_from_worldpos(mouse.get_position().x, mouse.get_position().y)->set_walkable(true);
	}

	if(!space)
	{
		if(kb.get_keycode(clan::keycode_space))
		{
			for(int i = 0; i < m_size; i++)
			{
				for(int j = 0; j < m_size; j++)
				{
					Tile* current_tile = m_pTiles[i][j];					
					current_tile->get_astar_info().mark_path = false;
					current_tile->get_astar_info().reset();
				}
			}

			std::vector<Tile*> path = get_path_astar(0, 899);
			for(int i = 0; i < path.size(); i++)
				path[i]->get_astar_info().mark_path = true;

			space = true;
		}
	}
	else
	{
		if(!kb.get_keycode(clan::keycode_space))
			space = false;
	}
}


void Tilemap::draw(clan::Canvas c)
{
	for(int i = 0; i < m_size; i++)
	{
		for(int j = 0; j < m_size; j++)
		{
			Tile* current_tile = m_pTiles[i][j];
			clan::Colorf draw_color; 
			if(current_tile->get_astar_info().mark_path)
				draw_color = clan::Colorf::yellow;
			else if(current_tile->is_walkable()) 
				draw_color = clan::Colorf::green;
			else
				draw_color = clan::Colorf::red;

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

	int x = abs(id % (m_size));
	int y = abs(id / (m_size));

	if(x >= m_size || y >= m_size)
		return NULL;

	return m_pTiles[y][x];
}


std::vector<Tile*> Tilemap::get_adjacent_tiles(int id, bool walkable, bool diagonal)
{
	int x = id % (m_size);
	int y = id / (m_size);
	std::vector<Tile*> retval;


	if(y != 0)
	{
		if(walkable)
		{
			if(m_pTiles[y-1][x]->is_walkable())
				retval.push_back(m_pTiles[y-1][x]);

		}
		else
		{
			retval.push_back(m_pTiles[y-1][x]);
		}
	}

	if(y != m_size - 1)
	{
		if(walkable)
		{
			if(m_pTiles[y+1][x]->is_walkable())
				retval.push_back(m_pTiles[y+1][x]);
		}
		else
		{
			retval.push_back(m_pTiles[y+1][x]);
		}
	}

	if(x != 0)
	{
		if(walkable)
		{
			if(m_pTiles[y][x-1]->is_walkable())
				retval.push_back(m_pTiles[y][x-1]);

		}
		else
		{
			retval.push_back(m_pTiles[y][x-1]);
		}
	}

	if(x != m_size - 1)
	{
		if(walkable)
		{
			if(m_pTiles[y][x+1]->is_walkable())
				retval.push_back(m_pTiles[y][x+1]);
		}
		else
		{
			retval.push_back(m_pTiles[y][x+1]);
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


std::vector<Tile*> Tilemap::get_path_astar(int start_id, int end_id)
{
	// determines whether the search was successfull
	bool success = false;

	// get the start and goal tile
	Tile* start_tile = get_tile_by_id(start_id);
	Tile* end_tile = get_tile_by_id(end_id);

	// get their position
	int goal_x = end_id % (m_size);
	int goal_y = end_id / (m_size);

	// Create two lists; open and closed; add the start tile to the open list
	std::list<Tile*> open;
	std::list<Tile*> closed;
	open.push_back(start_tile);


	// Repeat while the open list has elements and the goal tile isn't in the closed list
	while(!open.empty() && std::find(closed.begin(), closed.end(), end_tile) == closed.end())
	{
		// Get tile with the lowest F score from the open list
		int lowest_fscore = (*open.begin())->get_astar_info().F();
		Tile* lowest_fscore_tile = (*open.begin());
		std::list<Tile*>::iterator it;
		for(it = open.begin(); it != open.end(); it++)
		{
			Tile* t = *it;
			if(t->get_astar_info().F() < lowest_fscore)
			{
				lowest_fscore = t->get_astar_info().F();
				lowest_fscore_tile = t;
			}
		}

		// Move the current tile to the closed list and mark it as visited
		// already visited tiles will never be visited again
		Tile* current_tile = lowest_fscore_tile;	
		open.remove(current_tile);
		closed.push_back(current_tile);
		current_tile->get_astar_info().visited = true;

		// For each adjacent tile of the current tile that is also not a wall
		std::vector<Tile*> adj_tiles = current_tile->get_adjacent_tiles(true, true);
		for(int i = 0; i < adj_tiles.size(); i++)
		{
			Tile* t = adj_tiles[i];

			// Ignore the following steps if tile was already visited or is in the closed list
			if(t->get_astar_info().visited || std::find(closed.begin(), closed.end(), t) != closed.end())
				continue;
			
			// if the adjacent tile isn't on the open nor on the closed list yet
			if(std::find(open.begin(), open.end(), t) == open.end())
			{
				// Add this adjacent tile to the open list
				open.push_back(t);

				// Mark the adjacent tile as child of the current tile
				t->get_astar_info().previous_tile = current_tile;

				// Calculate the G score
				t->get_astar_info().G = current_tile->get_astar_info().G + 1;

				// Calculate the H score
				int t_x = start_id % (m_size);
				int t_y = start_id / (m_size);
				int diff_x = goal_x - t_x;
				int diff_y = goal_y - t_y;
				t->get_astar_info().H = sqrt(diff_x * diff_x - diff_y * diff_y);
			}
			// if the adjacent tile is in the open list already
			else
			{
				// if the G score of the current tile is lower than that of the adjacent tile
				if(current_tile->get_astar_info().G < t->get_astar_info().G)
				{
					// Mark the adjacent tile as child of the current tile
					t->get_astar_info().previous_tile = current_tile;
				}
			}

		}
	}


	// is the goal tile in the closed list? if yes, a path has been found
	if(std::find(closed.begin(), closed.end(), end_tile) != closed.end())
		success = true;

	// if path has been found
	if(success)
	{
		// return the path as a vector using a recursive function
		std::vector<Tile*> path;
		return make_path_recursively(end_tile, path);
	}

	// if no path was found, return an empty vector
	return std::vector<Tile*>();
}


std::vector<Tile*> Tilemap::make_path_recursively(Tile* tile, std::vector<Tile*> tail)
{
	tail.push_back(tile);
	if(tile->get_astar_info().previous_tile != NULL)
		return make_path_recursively(tile->get_astar_info().previous_tile, tail);

	std::reverse(tail.begin(), tail.end());
	return tail;
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