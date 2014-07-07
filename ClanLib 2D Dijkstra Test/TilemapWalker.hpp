#pragma once

#include "precomp.hpp"
#include "Tilemap.hpp"


class Tile;
class Tilemap;
class TilemapWalker
{
	private:
		Tilemap* m_map;
		clan::Pointf m_pos;
		clan::Point m_goal;
		bool m_going;
		std::vector<Tile*> m_path;
		int m_index;

		void walk(float delta);

	public:
		TilemapWalker(Tilemap* map);
		void update(float delta);
		void draw(clan::Canvas c);
		void start_walk(clan::Point goal);
};