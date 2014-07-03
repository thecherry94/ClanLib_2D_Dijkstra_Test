#pragma once

#include "precomp.hpp"
#include "Tile.hpp"

class Tile;
class Tilemap
{
	private:
		int m_size;
		int m_tileSize;
		Tile*** m_pTiles;

		Tile* get_tile_by_id(int id);
		int get_id_by_array_pos(int x, int y);

		clan::Point world_to_tilemap_position(float x, float y);
		int worldpos_to_tilemap_id(float x, float y);

	public:
		Tilemap();

		void draw(clan::Canvas c);
		void build_map(int size);
		void reset();

		Tile* get_tile_from_worldpos(float x, float y);
		std::vector<Tile*> get_adjacent_tiles(int id, bool walkable);
		void make_rect_wall(int start_id, int end_id);
		void fill_rect_wall(int start_id, int end_id);
};