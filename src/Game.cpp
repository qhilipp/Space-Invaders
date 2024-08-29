#include <string>
#include "Game.h"

using namespace std;

Game::Game(string identifier): player(BattleEntity("player")), bounds(Bounds(0, 0, 210, 55)) {
    aliens = {};
    string json = getJSON(identifier, "games");
    player = BattleEntity(jsonStringValue(json, "player"));
}

void Game::update(char input) {
    player.movingDirection = Point();
    if(input == 'a') {
        player.movingDirection.x = -1;
    }
    if(input == 'd') {
        player.movingDirection.x = 1;
    }
    if(input == ' ') {
        spawnPlayerShoot();
    }
    player.update(*this);
    for(BattleEntity alien : aliens) alien.update(*this);
    for(int i = 0; i < bullets.size(); i++) {
        bullets[i].update(*this);
    }
}

void Game::spawnPlayerShoot() {
    for(int i = 0; i < player.bursts; i++) {
        Bullet b = player.bullet;
        b.position.x = player.position.x + player.getBounds().size.x / 2;
        b.position.y = player.position.y;
        b.movingDirection = Point(0, -1);
        bullets.push_back(b);
    }
}

void Game::applyPowerup(Powerup powerup) {
    if(powerup.effect == "health") player.healthPoints += powerup.value;
    if(powerup.effect == "health") player.bursts += powerup.value;
    if(powerup.effect == "health") player.bullet.damage += powerup.value;
}