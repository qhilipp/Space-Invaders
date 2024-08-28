#pragma once

#include <ncurses.h>

class Controller {	
private:
	View* view = View();
	Game* game = Game("defaultGame");
};