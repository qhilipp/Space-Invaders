#include "Alien.h"
#include "util.h"
#include "BattleEntity.h"

Alien::Alien(string identifier): BattleEntity(identifier) {
    string json = getJSON(identifier, "aliens");
    powerups = {};
    powerups.push_back(Powerup("healthBoost"));
    for(string powerupIdentifier : jsonStringArrayValue(json, "powerups")) {
        //powerups.push_back(Powerup(powerupIdentifier));
    }
}