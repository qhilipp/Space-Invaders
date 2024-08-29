#include <ncurses.h>
#include <unistd.h>
#include "View.h"
#include "Game.h"
#include "Controller.h"

using namespace std;

Controller::Controller() {
	game = new Game("defaultGame");
	view = new View(game);
	char input = 0;
	while(true) {
		input = getch();
		game->update(input);
		view->render();
		if(input == 't') break;
		usleep(30000);
	}
	endwin();
}

int main() {
	Controller();
	return 0;
}