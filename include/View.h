#pragma once

#include <ncurses.h>
#include <vector>
#include <string>
#include <algorithm>
#include <math.h>
#include "BattleEntity.h"
#include "Game.h"
#include "Bullet.h"
#include "GameEntity.h"
#include "Point.h"

using namespace std;

int statSize = 32;

class View {

private:
	Game* game;

public:
	View(Game* game) {
		setup();
		this->game = game;
		int width, height;
		getmaxyx(stdscr, height, width);
		game->updateBounds(Bounds(statSize, 0, width - statSize, height));
	}

private:
	void initColors() {
		init_color(0, 0, 0, 100); // Background color
	    init_color(1, 0, 1000, 0); // Text color
	    init_color(2, 0, 500, 0); // Secondary color
	    init_pair(1, 1, 0); // Text color pair
	    init_pair(2, 2, 0); // Secondary color pair
	}

	void setup() {
		initscr();
	    start_color();
	    initColors();
	    curs_set(0);
	    noecho();
	    keypad(stdscr, true);
	    nodelay(stdscr, TRUE);
	}

	bool isTransparent(BMP bmp, int i, int j) {
		int channels = bmp.bmp_info_header.bit_count / 8;
		if(channels != 4) return false;
		return bmp.data[channels * (j * bmp.bmp_info_header.width + i) + 3] == 0;
	}

	int getColor(BMP bmp, int i, int j) {
	    int channels = bmp.bmp_info_header.bit_count / 8;

	    static vector<int> colorMap;
	    int b = bmp.data[channels * (j * bmp.bmp_info_header.width + i) + 0];
	    int g = bmp.data[channels * (j * bmp.bmp_info_header.width + i) + 1];
	    int r = bmp.data[channels * (j * bmp.bmp_info_header.width + i) + 2];
	    int key = r * 65536 + g * 256 + b;

	    auto it = std::find(colorMap.begin(), colorMap.end(), key);

	    if (it != colorMap.end()) {
	        return std::distance(colorMap.begin(), it) + 1 + 5;
	    } else {
	        colorMap.push_back(key);
	        init_color(colorMap.size() + 5, (r * 1000) / 255, (g * 1000) / 255, (b * 1000) / 255);
	        return colorMap.size();
	    }
	}

	int getColorPair(int a, int b) {
	    static vector<int> colorPairMap;
	    int key = a * 256 + b;

	    auto it = std::find(colorPairMap.begin(), colorPairMap.end(), key);

	    if (it != colorPairMap.end()) {
	        return std::distance(colorPairMap.begin(), it) + 1 + 5;
	    } else {
	        colorPairMap.push_back(key);
	        init_pair(colorPairMap.size() + 5, a, b);
	        return colorPairMap.size();
	    }
	}

	void renderEntity(GameEntity entity) {
	    int width = entity.baseImg.bmp_info_header.width;
	    int height = entity.baseImg.bmp_info_header.height;
	    static vector<int> colorMap, colorPairMap;

	    for(int j = 0; j < height; j++) {
	        for(int i = 0; i < width; i++) {
	            int baseColor = getColor(entity.baseImg, i, j);
	            int textureColor = getColor(entity.textureImg, i, j);
	            int colorPair = getColorPair(textureColor, baseColor);

	            if(isTransparent(entity.baseImg, i, j) && (isTransparent(entity.textureImg, i, j) || entity.texture[j][i] == ' ')) continue;

	            attron(COLOR_PAIR(colorPair));
	            mvaddch(j + entity.position.y, i + entity.position.x, entity.texture[j][i]);
	            attroff(COLOR_PAIR(colorPair));
	        }
	    }
	}

	void renderText(int row, const string& name) {
		attron(COLOR_PAIR(1));
		mvprintw(row, 0, "%s", name.c_str());
	}

	void renderStatBegining(int row, const string& name) {
		renderText(row, name);
		printw(": ");
		move(row, 10);
	}

	void renderStatNum(int row, const string& name, int value) {
		renderStatBegining(row, name);
		printw("%s", to_string(value).c_str());
	}

	void renderStat(int row, const string& name, double value) {
		value = max(0.0, min(1.0, value));
		int maxLength = 20;
		int length = (int) (value * maxLength);
		int remaining = maxLength - length;
		renderStatBegining(row, name);
		printw("%s", string(length, '#').c_str());
		attroff(COLOR_PAIR(1));
		attron(COLOR_PAIR(2));
		printw("%s", string(remaining, '#').c_str());
		attroff(COLOR_PAIR(2));
	}

	void renderStat(int row, const string& name, string value) {
		renderStatBegining(row, name);
		printw("%s", value.c_str());
		attroff(COLOR_PAIR(1));
	}

	void renderStats() {
		renderStatNum(0, "Level", game->level);
		renderStatNum(1, "Score", game->score);
		renderStat(2, "Health", game->player.healthPoints / (double) game->player.maxHealthPoints);
		renderStat(3, "Speed", game->player.velocity.length() / game->player.terminalVelocity);
		renderStatNum(4, "Kills", game->player.kills);
		renderStatNum(5, "Damage", game->player.bullet.damage);
		renderStatNum(6, "Shots", game->player.burstsFired);
	}

public:
	void render() {
		clear();
		for(Bullet bullet : game->bullets) {
			renderEntity(bullet);
		}
		for(Bullet bullet : game->alienBullets) {
			renderEntity(bullet);
		}
		for(BattleEntity alien : game->aliens) {
			renderEntity(alien);
		}
		for(Powerup powerup : game->powerups) {
			renderEntity(powerup);
		}
		renderEntity(game->player);
		renderStats();
		if(game->state == GameState::GAME_OVER) renderEntity(game->gameOverText);
	}
};