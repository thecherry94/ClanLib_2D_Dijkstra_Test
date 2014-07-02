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
	
	Tilemap map;
	map.build_map(30);

	while(!win.get_ic().get_keyboard().get_keycode(clan::keycode_escape))
	{
		canvas.clear();

		map.draw(canvas);

		win.flip();
		clan::KeepAlive::process();
	}

	map.reset();

	return 0;
}