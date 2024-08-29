#include "Powerup.h"
#include "util.h"

Powerup::Powerup(string identifier): GameEntity(identifier) {
    string json = getJSON(identifier, "powerups");
    effect = jsonStringValue(json, "effect");
    probability = jsonDoubleValue(json, "probability");
    value = jsonIntValue(json, "value");
}