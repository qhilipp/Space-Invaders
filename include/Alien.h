#pragma once

#include <vector>
#include <string>
#include "Powerup.h"

using namespace std;

class Alien: public GameEntity {
public:
    vector<Powerup> powerups;

    Alien(string identifier);
};