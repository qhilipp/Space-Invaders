#include <ncurses.h>
#include <View.h>
#include <Models.h>
#include <ncurses.h>
#include <string>
#include <vector>
#include <BMP.h>
#include <algorithm>
#include <iostream>
#include <unistd.h>

using namespace std;

class Controller {

	Game* game = new Game("defaultGame");
	View* view = new View(game);

public:
	Controller() {
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
};

int main() {
	Controller();
	return 0;
}