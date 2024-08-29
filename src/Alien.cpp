#include "Alien.h"
#include "util.h"

Alien::Alien(string identifier): GameEntity(identifier) {
    string json = getJSON(identifier, "aliens");
    for(string powerupIdentifier : jsonStringArrayValue(json, "powerups")) {
        powerups.push_back(Powerup(powerupIdentifier));
    }
}