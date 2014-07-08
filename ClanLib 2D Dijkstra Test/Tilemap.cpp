#include "Tilemap.hpp"
#include <algorithm>
#include <time.h>
#include "App.hpp"

Tilemap::Tilemap()
{
	m_tileSize = 16;
	space = false;
}

void Tilemap::build_map(int size)
{
	reset();
	m_size = size;

	m_mapWalker = new TilemapWalker(this);

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

	std::vector<Tile*> adj = get_adjacent_tiles2(70);
	for(int i = 0; i < adj.size(); i++)
		adj[i]->set_walkable(false);
}


void Tilemap::update(clan::InputContext ic, float delta)
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

	if(kb.get_keycode(clan::keycode_r))
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
	}

	if(!space)
	{
		if(kb.get_keycode(clan::keycode_space))
		{
			std::vector<Tile*> path = get_path_astar(0, 899, true);
			for(int i = 0; i < path.size(); i++)
				path[i]->get_astar_info().mark_path = true;

			space = true;

			m_mapWalker->start_walk(get_worldpos_by_id(960));
		}
	}
	else
	{
		if(!kb.get_keycode(clan::keycode_space))
			space = false;
	}
	
	m_mapWalker->update(delta);
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

	m_mapWalker->draw(c);
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

	if(diagonal)
	{
		if(y != 0 && x != 0)
		{
			if(walkable)
			{
				if(m_pTiles[y-1][x-1]->is_walkable())
					retval.push_back(m_pTiles[y-1][x-1]);
			}
			else
			{
				retval.push_back(m_pTiles[y-1][x-1]);
			}
		}

		if(y != 0 && x != m_size - 1)
		{
			if(walkable)
			{
				if(m_pTiles[y-1][x+1]->is_walkable())
					retval.push_back(m_pTiles[y-1][x+1]);
			}
			else
			{
				retval.push_back(m_pTiles[y-1][x+1]);
			}
		}

		if(y != m_size - 1 && x != 0)
		{
			if(walkable)
			{
				if(m_pTiles[y+1][x-1]->is_walkable())
					retval.push_back(m_pTiles[y+1][x-1]);
			}
			else
			{
				retval.push_back(m_pTiles[y+1][x-1]);
			}
		}

		if(y != m_size - 1 && x != m_size - 1)
		{
			if(walkable)
			{
				if(m_pTiles[y+1][x+1]->is_walkable())
					retval.push_back(m_pTiles[y+1][x+1]);
			}
			else
			{
				retval.push_back(m_pTiles[y+1][x+1]);
			}
		}
	}


	return retval;
}


std::vector<Tile*> Tilemap::get_adjacent_tiles2(int id)
{
	int x = id % (m_size);
	int y = id / (m_size);
	std::vector<Tile*> retval;


	if(y - 1 != 0)
	{
		retval.push_back(m_pTiles[y-2][x]);
	}

	if(y != m_size - 2)
	{
		retval.push_back(m_pTiles[y+2][x]);
	}

	if(x - 1 != 0)
	{
		retval.push_back(m_pTiles[y][x-2]);	
	}

	if(x != m_size - 2)
	{
		retval.push_back(m_pTiles[y][x+2]);	
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


std::vector<Tile*> Tilemap::get_path_astar(int start_id, int end_id, bool allow_diagonal_movement)
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
	std::vector<Tile*> open;
	std::vector<Tile*> closed;
	open.push_back(start_tile);


	// Repeat while the open list has elements and the goal tile isn't in the closed list
	while(!open.empty() && std::find(closed.begin(), closed.end(), end_tile) == closed.end())
	{
		// Get tile with the lowest F score from the open list
		int lowest_fscore = (*open.begin())->get_astar_info().F();
		Tile* lowest_fscore_tile = (*open.begin());
		std::vector<Tile*>::iterator it;
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
		open.erase(std::remove(open.begin(), open.end(), current_tile), open.end());
		closed.push_back(current_tile);
		current_tile->get_astar_info().visited = true;

		// For each adjacent tile of the current tile that is also not a wall
		std::vector<Tile*> adj_tiles = current_tile->get_adjacent_tiles(true, allow_diagonal_movement);
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
				int t_x = t->get_id() % (m_size);
				int t_y = t->get_id() / (m_size);
				int diff_x = goal_x - t_x;
				int diff_y = goal_y - t_y;
				int dist = sqrt(diff_x * diff_x + diff_y * diff_y);
				t->get_astar_info().H = dist;
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


	// All tiles need to be reset in order for the astar algorithm to work again
	//
	for(int i = 0; i < m_size; i++)
	{
		for(int j = 0; j < m_size; j++)
		{
			Tile* current_tile = m_pTiles[i][j];					
			current_tile->get_astar_info().mark_path = false;
			current_tile->get_astar_info().reset();
		}
	}

	return tail;
}


// This algorithm fucking sucks and can't do anything useful
//
void Tilemap::make_maze_custom()
{
	Tile* start_tile = m_pTiles[0][0];
	std::vector<Tile*> open;
	std::vector<Tile*> closed;

	open.push_back(start_tile);

	// Make all tiles walls
	for(int i = 0; i < m_size; i++)
	{
		for(int j = 0; j < m_size; j++)
		{
			Tile* current_tile = m_pTiles[i][j];
			current_tile->set_walkable(false);
		}
	}

	start_tile->set_walkable(true);

	srand(time(NULL));

	// While the open set has elements
	while(!open.empty())
	{
		// Return a random tile from the open set
		//Tile* current_tile = open[rand() % open.size()];
		Tile* current_tile = open.back();

		// Set the current tile as visited and move it into the closed list
		current_tile->get_cst_maze_info().visited = true;
		open.erase(std::find(open.begin(), open.end(), current_tile), open.end());
		closed.push_back(current_tile);

		// Iterate through all adjacent tiles of the current tile; whether they be walls or not
		std::vector<Tile*> adj_tiles = current_tile->get_adjacent_tiles(false, false);
		for(int i = 0; i < adj_tiles.size(); i++)
		{
			Tile* curr_adj_tile = adj_tiles[i];

			// If the adjacent tile isn't neither on the open nor on the closed list
			if (std::find(open.begin(), open.end(), curr_adj_tile) == open.end() &&
			    std::find(closed.begin(), closed.end(), curr_adj_tile) == closed.end())
			{
				/*
				// Add it to the open set and mark it as visited
				open.push_back(curr_adj_tile);
				curr_adj_tile->get_cst_maze_info().visited = true;
				curr_adj_tile->get_cst_maze_info().previous_tile = current_tile;
				*/

				// Get the relation between the current tile and the adjecent current tile
				TileRelation rel = get_relation(current_tile, curr_adj_tile);

				// Get the tile array position
				clan::Point adj_array_pos = curr_adj_tile->get_array_pos();

				// Whether or not the wall will be removed
				bool remove_wall = true;


				/* The following if clause will check whether there are walls on the side of the tile
				 * depending on the relation of the current tile and the current adjacent tile
				 * if there are walls on both side, the wall of the current adjecent tile will be removed
				 * and it will be added to the open set
				 */

				// If they are vertically aligned
				if(rel == TileRelation::DOWN || rel == TileRelation::UP)
				{
					// If there is an element on the left side
					if(adj_array_pos.x - 1 > 0)
					{
						// If there is no wall on the side, don't remove the wall on this tile
						if(m_pTiles[adj_array_pos.x-1][adj_array_pos.y]->is_walkable())
							remove_wall = false;
					}

					// If there is an element on the right side
					if(adj_array_pos.x + 1 != m_size)
					{
						// If there is no wall on the side, don't remove the wall on this tile
						if(m_pTiles[adj_array_pos.x+1][adj_array_pos.y]->is_walkable())
							remove_wall = false;
					}

				}
				// If they are horizontally aligned
				else if(rel == TileRelation::LEFT || rel == TileRelation::RIGHT)
				{
					// If there is an element above
					if(adj_array_pos.y - 1 > 0)
					{
						// If there is no wall on the side, don't remove the wall on this tile
						if(m_pTiles[adj_array_pos.x][adj_array_pos.y-1]->is_walkable())
							remove_wall = false;
					}

					// If there is an element below
					if(adj_array_pos.y + 1 != m_size)
					{
						// If there is no wall on the side, don't remove the wall on this tile
						if(m_pTiles[adj_array_pos.x][adj_array_pos.y+1]->is_walkable())
							remove_wall = false;
					}
				}

				// If the wall should be removed
				if(remove_wall)
				{
					// Add the current adjacent tile to the open set
					open.push_back(curr_adj_tile);
					curr_adj_tile->get_cst_maze_info().previous_tile = current_tile;

					// Finally remove the wall
					curr_adj_tile->set_walkable(true);
				}
				// If the wall should not be removed
				else
				{
					// Add the current adjacent tile to the closed set
					closed.push_back(curr_adj_tile);
					curr_adj_tile->get_cst_maze_info().previous_tile = current_tile;
				}
			}
		}
	}
}


// This method uses the depth first search algorithm to create a maze
//
void Tilemap::make_maze_depthfirst()
{
	// Make all tiles walls
	for(int i = 0; i < m_size; i++)
	{
		for(int j = 0; j < m_size; j++)
		{
			Tile* current_tile = m_pTiles[i][j];
			current_tile->set_walkable(false);
		}
	}

	// Create a queue of tiles to go trough
	std::vector<Tile*> open;

	// Select a random tile as starting point
	srand(time(NULL));
	Tile* start_tile = get_tile_by_id((2*rand()) % (m_size * m_size));

	Tile* current_tile = start_tile;

	// While there are tiles in the queue
	do 
	{
		open.push_back(current_tile);
		current_tile->get_cst_maze_info().visited = true;

		fuck:
		// Get all adjacent tiles of the current tile
		std::vector<Tile*> adj_tiles = get_adjacent_tiles2(current_tile->get_id());

		// Sort out all tiles that have already been visited
		std::vector<Tile*>::iterator it = adj_tiles.begin();
		while(it != adj_tiles.end())
		{
			Tile* t = *it;

			if(t->get_cst_maze_info().visited)
				it = adj_tiles.erase(it);
			else
				it++;
		}

		// if there are none adjacent tile that haven't been visited
		if(adj_tiles.empty())
		{
			if(open.empty())
				return;

			srand(time(NULL));
			current_tile = open[rand() % open.size()];
			open.erase(std::remove(open.begin(), open.end(), current_tile), open.end());
			goto fuck;
		}

		// Select a random adjacent tile
		srand(time(NULL));
		Tile* adj_tile = adj_tiles[rand() % adj_tiles.size()];

		// Get relation between current tile and adjacent tile
		TileRelation rel = get_relation(current_tile, adj_tile);

		switch (rel)
		{
			case UP:
				get_tile_by_id(current_tile->get_id() - m_size)->set_walkable(true);
			break;

			case RIGHT:
				get_tile_by_id(current_tile->get_id() + 1)->set_walkable(true);
			break;

			case LEFT:
				get_tile_by_id(current_tile->get_id() - 1)->set_walkable(true);
			break;

			case DOWN:
				get_tile_by_id(current_tile->get_id() + m_size)->set_walkable(true);
			break;
		}

		current_tile = adj_tile;
	} while(!open.empty());
}


TileRelation Tilemap::get_relation(Tile* a, Tile* b)
{
	clan::Point p_a = a->get_array_pos();
	clan::Point p_b = b->get_array_pos();

	if(p_a.y < p_b.y)
		return TileRelation::DOWN;
	if(p_a.y > p_b.y)
		return TileRelation::UP;
	if(p_a.x < p_b.x)
		return TileRelation::RIGHT;
	if(p_a.x > p_b.x)
		return TileRelation::LEFT;
}


void Tilemap::fill_rect_wall_by_arraypos(int start_x, int start_y, int end_x, int end_y)
{
	fill_rect_wall(get_id_by_array_pos(start_x, start_y), get_id_by_array_pos(end_x, end_y));
}


clan::Point Tilemap::world_to_tilemap_position(int x, int y)
{
	return clan::Point(x / m_tileSize , y / m_tileSize);
}


int Tilemap::worldpos_to_tilemap_id(int x, int y)
{
	clan::Point p = world_to_tilemap_position(x, y);
	return get_id_by_array_pos(p.x, p.y);
}


Tile* Tilemap::get_tile_from_worldpos(float x, float y)
{
	clan::Point p = world_to_tilemap_position(x, y);
	return m_pTiles[p.x][p.y];
}


clan::Point Tilemap::get_worldpos_by_id(int id)
{
	clan::Point retval;
	retval.x = (id % m_size) * m_tileSize;
	retval.y = (id / m_size) * m_tileSize;

	return retval;
}