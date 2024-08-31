#include <ncurses.h>
#include <unistd.h>
#include "View.h"
#include "Game.h"
#include "Controller.h"
#include "util.h"

using namespace std;

Controller::Controller() {
	game = new Game("defaultGame");
	view = new View(game);
	int input = -1;
	bool freeze = false;
	while(true) {
		input = getch();
		if(!freeze) {
		game->update(getInput(input));
		view->render();
	}
		if(input == 't') break;
		if(input == 'f') freeze = !freeze;
		usleep(30000);
	}
	endwin();
}

Input Controller::getInput(int input) {
	if(game->keyMap.find(input) != game->keyMap.end()) {
		return game->keyMap[input];
	}
	return Input::NONE;
}

int main() {
	std::srand(static_cast<unsigned int>(std::time(0)));
	Controller();
	return 0;
}