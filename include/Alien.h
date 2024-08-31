#pragma once

#include <vector>
#include <optional>
#include <string>
#include "Powerup.h"
#include "BattleEntity.h"

using namespace std;

class Alien: public BattleEntity {
public:
    vector<Powerup> powerups;
    int minShootDelay, maxShootDelay, currentDelay;

    Alien(string identifier);

    optional<Bullet> autoShoot();
};