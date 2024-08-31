#include <optional>
#include "Alien.h"
#include "util.h"
#include "BattleEntity.h"

using namespace std;

Alien::Alien(string identifier): BattleEntity(identifier) {
    string json = getJSON(identifier, "aliens");
    powerups = {};
    minShootDelay = jsonIntValue(json, "minShootDelay");
    maxShootDelay = jsonIntValue(json, "maxShootDelay");
    this->bullet.delay = getRandom(minShootDelay, maxShootDelay);
    points = jsonIntValue(json, "points");
    for(string powerupIdentifier : jsonStringArrayValue(json, "powerups")) {
        powerups.push_back(Powerup(powerupIdentifier));
    }
}

optional<Bullet> Alien::autoShoot() {
    optional<Bullet> bullet = shoot(false);
    if(bullet.has_value()) {
        this->bullet.delay = getRandom(minShootDelay, maxShootDelay);
    }
    return bullet;
}