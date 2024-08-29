#pragma once

#include <string>
#include "GameEntity.h"

using namespace std;

class Bullet: public GameEntity {
public:
    int damage;
    int delay;

    Bullet(string name, int damage, int delay);
    Bullet(string identifier);
};