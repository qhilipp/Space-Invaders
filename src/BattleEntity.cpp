#include "BattleEntity.h"
#include "util.h"

using namespace std;

BattleEntity::BattleEntity(string name, int bursts, int healthPoints, Bullet bullet):
    GameEntity(name),
    bursts(bursts),
    healthPoints(healthPoints),
    bullet(bullet)
{}

BattleEntity::BattleEntity(string identifier): GameEntity(identifier), bullet(Bullet("laser", 0, 0)) {
    string json = getJSON(identifier, "battleEntities");
    bursts = jsonIntValue(json, "bursts");
    healthPoints = jsonIntValue(json, "healthPoints");
    bullet = Bullet(jsonStringValue(json, "bullet"));
    aliensKilled=0;
}