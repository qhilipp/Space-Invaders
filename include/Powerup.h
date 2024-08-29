#pragma once

#include <string>
#include "GameEntity.h"
#include "util.h"

class Powerup: public GameEntity {
public:
    string effect;
    double probability;
    int value;

    Powerup(string identifier);
};