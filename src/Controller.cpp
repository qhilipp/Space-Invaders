#include <ncurses.h>
#include <unistd.h>
#include <ctime>
#include <cstdlib>
#include "View.h"
#include "Game.h"
#include "Controller.h"
#include "util.h"

using namespace std;

Controller::Controller() {
	game = new Game("selected");
	view = new View(game);
	int input = -1;
	while(true) {
		input = getch();
		game->update(getInput(input));
		view->render();
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