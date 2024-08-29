#include "Bullet.h"
#include "util.h"

using namespace std;

Bullet::Bullet(string name, int damage, int delay): GameEntity(name),
      damage(damage), 
      delay(delay) 
{}

Bullet::Bullet(string identifier) 
    : GameEntity(identifier)
{
    string json = getJSON(identifier, "bullets");
    damage = jsonIntValue(json, "damage");
    delay = jsonIntValue(json, "delay");
}
