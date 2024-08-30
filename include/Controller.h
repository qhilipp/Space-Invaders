#pragma once

class Controller {	
public:
	Controller();
private:
	View* view;
	Game* game;

	Input getInput(int input);
};