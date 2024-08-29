#pragma once

#include <string>
#include "Bullet.h"
#include "GameEntity.h"
#include "Bullet.h"

using namespace std;

class BattleEntity: public GameEntity {
public:
    int bursts;
    int healthPoints;
    Bullet bullet;

    BattleEntity(string name, int bursts, int healthPoints, Bullet bullet);
    BattleEntity(string identifier);
};