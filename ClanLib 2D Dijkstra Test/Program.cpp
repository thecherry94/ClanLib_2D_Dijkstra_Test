#include "Program.hpp"
#include "App.hpp"

using namespace clan;

int Program::main(const std::vector<std::string>& args)
{
	// Setup ClanLib
	SetupCore setup_core;
	SetupDisplay setup_display;
	SetupSound setup_sound;
	SetupNetwork setup_network;
	SetupGL setup_gl;
	SetupGUI setup_gui;

	App app;
	int retval = app.main(args);

	return retval;
}

// Program starts here
Application main_app(&Program::main);