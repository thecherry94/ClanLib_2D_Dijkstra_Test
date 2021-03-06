#pragma once

#include "precomp.hpp"
#include "Tile.hpp"
#include "TilemapWalker.hpp"

enum TileRelation
{
	UP,
	RIGHT,
	LEFT,
	DOWN
};

class Tile;
class TilemapWalker;
class Tilemap
{
	private:
		int m_size;
		int m_tileSize;
		Tile*** m_pTiles;
		bool space;

		TilemapWalker* m_mapWalker;
		

		std::vector<Tile*> make_path_recursively(Tile* tile, std::vector<Tile*> tail);

	public:
		Tilemap();

		void draw(clan::Canvas c);
		void update(clan::InputContext ic, float delta);
		void build_map(int size);
		void reset();

		Tile* get_tile_from_worldpos(float x, float y);
		std::vector<Tile*> get_adjacent_tiles(int id, bool walkable, bool diagonal);
		std::vector<Tile*> get_adjacent_tiles2(int id);
		void make_rect_wall(int start_id, int end_id);
		void make_rect_wall_by_arraypos(int start_x, int start_y, int end_x, int end_y);
		void fill_rect_wall(int start_id, int end_id);
		void fill_rect_wall_by_arraypos(int start_x, int start_y, int end_x, int end_y);
		Tile* get_tile_by_id(int id);
		int get_id_by_array_pos(int x, int y);
		clan::Point get_worldpos_by_id(int id);

		clan::Point world_to_tilemap_position(int x, int y);
		int worldpos_to_tilemap_id(int x, int y);

		TileRelation get_relation(Tile* a, Tile* b);

		std::vector<Tile*> get_path_astar(int start_id, int end_id, bool allow_diagonal_movement);
		void make_maze_custom();
		void make_maze_depthfirst();

		int get_size() { return m_size; }

};