#include <optional>
#include "BattleEntity.h"
#include "util.h"

using namespace std;

BattleEntity::BattleEntity(string identifier): GameEntity(identifier), bullet(Bullet("laser", 0, 0)) {
    string json = getJSON(identifier, "battleEntities");
    bursts = jsonIntValue(json, "bursts");
    maxHealthPoints = jsonIntValue(json, "healthPoints");
    healthPoints = maxHealthPoints;
    bullet = Bullet(jsonStringValue(json, "bullet"));
    lastShot = getTime();
}

optional<Bullet> BattleEntity::burstShoot(){
if(burstsFired< bursts && burstsFired >0){
    return shoot(true);
}
else{
    burstsFired = 0;
    return nullopt;
}

}

optional<Bullet> BattleEntity::shoot(bool isBurst) {
    // Check if we are within the time to fire the next bullet in the burst
    if (getTime() - lastShot < bullet.delay) {
        if(!isBurst){
        return nullopt; // Not yet time to fire the next bullet in the burst
    }
    }
    lastShot = getTime();
    
    // Create and position the bullet
    Bullet b = bullet;
    b.position.x = position.x + getBounds().size.x / 2;
    if(shootingDirection == -1) {
        b.position.y = position.y;
    } else {
        b.position.y = position.y + getBounds().size.y;
    }
    b.movingDirection = Point(0, shootingDirection);
    burstsFired++;
    return b;
}