#pragma once

#include <string>
#include <vector>
#include "Bounds.h"
#include "BattleEntity.h"
#include "Bullet.h"
#include "Powerup.h"

using namespace std;

class Game {
public:
    BattleEntity player;
    vector<BattleEntity> aliens;
    vector<Bullet> bullets;
    Bounds bounds;

    Game(string identifier);

    void update(char input);
    void spawnPlayerShoot();
    void applyPowerup(Powerup powerup);
};