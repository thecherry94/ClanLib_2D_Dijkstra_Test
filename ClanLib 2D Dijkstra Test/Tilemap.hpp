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
		bool space;

		Tile* get_tile_by_id(int id);
		int get_id_by_array_pos(int x, int y);

		clan::Point world_to_tilemap_position(float x, float y);
		int worldpos_to_tilemap_id(float x, float y);

		std::vector<Tile*> make_path_recursively(Tile* tile, std::vector<Tile*> tail);

	public:
		Tilemap();

		void draw(clan::Canvas c);
		void update(clan::InputContext ic);
		void build_map(int size);
		void reset();

		Tile* get_tile_from_worldpos(float x, float y);
		std::vector<Tile*> get_adjacent_tiles(int id, bool walkable, bool diagonal);
		void make_rect_wall(int start_id, int end_id);
		void make_rect_wall_by_arraypos(int start_x, int start_y, int end_x, int end_y);
		void fill_rect_wall(int start_id, int end_id);
		void fill_rect_wall_by_arraypos(int start_x, int start_y, int end_x, int end_y);


		std::vector<Tile*> get_path_astar(int start_id, int end_id);

};