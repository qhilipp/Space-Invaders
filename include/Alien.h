#pragma once

#include <vector>
#include <string>
#include "Powerup.h"
#include "BattleEntity.h"

using namespace std;

class Alien: public BattleEntity {
public:
    vector<Powerup> powerups;

    Alien(string identifier);
};