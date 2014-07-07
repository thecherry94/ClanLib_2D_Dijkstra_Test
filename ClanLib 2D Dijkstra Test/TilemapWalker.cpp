#include "TilemapWalker.hpp"

TilemapWalker::TilemapWalker(Tilemap* map)
{
	m_map = map;
	m_pos = clan::Pointf(0, 0);
	m_goal = clan::Point(-1, -1);
	m_going = false;	
}


void TilemapWalker::start_walk(clan::Point goal)
{
	m_goal = goal;

	int id_start = m_map->worldpos_to_tilemap_id(m_pos.x, m_pos.y);
	int id_goal = m_map->worldpos_to_tilemap_id(m_goal.x, m_goal.y);

	m_path = m_map->get_path_astar(
		id_start, 
		id_goal,
		false);

	m_going = !m_path.empty();
	m_index = 0;
}


void TilemapWalker::draw(clan::Canvas c)
{
	c.fill_rect(clan::Rect(clan::Point(m_pos.x, m_pos.y), clan::Size(16, 16)), clan::Colorf::darkblue);
}


void TilemapWalker::update(float delta)
{
	if(m_going)
		walk(delta);
}


void TilemapWalker::walk(float delta)
{
	if(!m_going)
		return;

	float speed = 100.0f;

	Tile* t = m_path[m_index];
	clan::Point tile_pos = m_map->get_worldpos_by_id(t->get_id());

	if((int)m_pos.x < tile_pos.y)
		m_pos.x += speed * delta;
	else if((int)m_pos.x > tile_pos.y)
		m_pos.x += -speed * delta;
	
	if((int)m_pos.y < tile_pos.x)
		m_pos.y += speed * delta;
	else if((int)m_pos.y > tile_pos.x)
		m_pos.y += -speed * delta;

	if((int)m_pos.x == tile_pos.y && (int)m_pos.y == tile_pos.x)
		m_index++;


	if(m_index == m_path.size())
		m_going = false;
}