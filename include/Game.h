#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "Bounds.h"
#include "BattleEntity.h"
#include "Bullet.h"
#include "Powerup.h"
#include "Alien.h"

using namespace std;

enum class Input {
    LEFT, RIGHT, SHOOT, NONE, QUIT
};

class Game {
public:
    BattleEntity player;
    vector<Alien> aliens;
    vector<Bullet> bullets;
    vector<Powerup> powerups;
    Bounds bounds;
    unordered_map<int, Input> keyMap;

    Game(string identifier);

    void update(Input input);
    void spawnPlayerShoot();
    void spawnPowerup(int alienIndex);
    void applyPowerup(int powerupIndex);
};