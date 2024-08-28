#pragma once

#include <ncurses.h>
#include <vector>
#include <Objects.h>
#include <string>
#include <algorithm>

using namespace std;

class View {

private:
	Game* game;

public:
	View(Game* game) {
		setup();
		this->game = game;
	}

private:
	void setup() {
		initscr();
	    start_color();
	    init_color(0, 1, 1, 1);
	    curs_set(0);
	    noecho();
	    keypad(stdscr, true);
	    nodelay(stdscr, TRUE);
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
	        return std::distance(colorMap.begin(), it) + 1;
	    } else {
	        colorMap.push_back(key);
	        init_color(colorMap.size(), (r * 1000) / 255, (g * 1000) / 255, (b * 1000) / 255);
	        return colorMap.size();
	    }
	}

	int getColorPair(int a, int b) {
	    static vector<int> colorPairMap;
	    int key = a * 256 + b;

	    auto it = std::find(colorPairMap.begin(), colorPairMap.end(), key);

	    if (it != colorPairMap.end()) {
	        return std::distance(colorPairMap.begin(), it) + 1;
	    } else {
	        colorPairMap.push_back(key);
	        init_pair(colorPairMap.size(), a, b);
	        return colorPairMap.size();
	    }
	}

	void renderEntity(GameEntity entity) {
	    int width = entity.baseImg.bmp_info_header.width;
	    int height = entity.baseImg.bmp_info_header.height;
	    int channels = entity.baseImg.bmp_info_header.bit_count / 8;
	    static vector<int> colorMap, colorPairMap;

	    for(int j = 0; j < height; j++) {
	        for(int i = 0; i < width; i++) {
	            int baseColor = getColor(entity.baseImg, i, j);
	            int textureColor = getColor(entity.textureImg, i, j);
	            int colorPair = getColorPair(textureColor, baseColor);

	            attron(COLOR_PAIR(colorPair));
	            mvaddch(j + entity.position.y, i + entity.position.x, entity.texture[j][i]);
	            attroff(COLOR_PAIR(colorPair));
	        }
	    }
	}

public:
	void render() {
		clear();
		for(BattleEntity alien : game->aliens) {
			renderEntity(alien);
		}
		for(Bullet bullet : game->bullets) {
			renderEntity(bullet);
		}
		renderEntity(game->player);
	}
};