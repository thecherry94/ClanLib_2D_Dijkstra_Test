#include "App.hpp"
#include "precomp.hpp"
#include "Tilemap.hpp"

int App::main(const std::vector<std::string>& args)
{
	clan::DisplayWindowDescription desc;
	desc.set_position(clan::Rect(-800, 100, clan::Size(800, 600)), false);
	desc.set_title("ClanLib 2D Dijkstra Test");
	desc.set_allow_resize(false);

	clan::DisplayWindow win(desc);
	clan::Canvas canvas(win);
	
	m_canvas = canvas; 

	Tilemap map;
	map.build_map(31);

	clan::GameTime game_time;
	while(!win.get_ic().get_keyboard().get_keycode(clan::keycode_escape))
	{
		game_time.update();
		canvas.clear();

		map.draw(canvas);
		map.update(win.get_ic(), game_time.get_time_elapsed());

		win.flip();
		clan::KeepAlive::process();
	}

	map.reset();

	return 0;
}

clan::Canvas App::m_canvas;