#pragma once

#include <optional>
#include <string>
#include "Bullet.h"
#include "GameEntity.h"
#include "Bullet.h"

using namespace std;

class BattleEntity: public GameEntity {
public:
    int bursts;
    int healthPoints, maxHealthPoints;
    Bullet bullet;
    int shootingDirection = 1, burstsFired = 0, kills = 0;
    long int lastShot;

    BattleEntity(string name, int bursts, int healthPoints, Bullet bullet);
    BattleEntity(string identifier);

    optional <Bullet> burstShoot();
    optional<Bullet> shoot();
};