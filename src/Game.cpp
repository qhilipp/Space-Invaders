#include <string>
#include <iostream>
#include <algorithm>
#include "Game.h"
#include "util.h"

using namespace std;

Game::Game(string identifier): player(BattleEntity("player")), bounds(Bounds(0, 0, 210, 55)) {
    aliens = {};
    string json = getJSON(identifier, "games");
    player = BattleEntity(jsonStringValue(json, "player"));
    vector<string> alienRows = jsonStringArrayValue(json, "aliens");
    for(int i = 0; i < alienRows.size(); i++) {
        Alien alien = Alien(alienRows[i]);
        alien.position = Point(i * (alien.getBounds().size.x + 4), 5);
        aliens.push_back(alien);
    }
    keyMap[getKeyCode(jsonStringValue(json, "left"))] = Input::LEFT;
    keyMap[getKeyCode(jsonStringValue(json, "right"))] = Input::RIGHT;
    keyMap[getKeyCode(jsonStringValue(json, "shoot"))] = Input::SHOOT;
}

void Game::update(Input input) {
    player.movingDirection = Point();
    if(input == Input::LEFT) {
        player.movingDirection.x = -1;
    }
    if(input == Input::RIGHT) {
        player.movingDirection.x = 1;
    }
    if(input == Input::SHOOT) {
        spawnPlayerShoot();
    }
    player.update(*this);
    for(int i = 0; i < powerups.size(); i++) {
        powerups[i].update(*this);
        if(powerups[i].position.y < -100) powerups.erase(powerups.begin() + i);
        if(powerups[i].getBounds().collides(player.getBounds())) {
            applyPowerup(i);
        }
    }

    for(int i = bullets.size() - 1; i >= 0; i--) {
        bullets[i].update(*this);
        if(bullets[i].position.y < -100) bullets.erase(bullets.begin() + i);
        for(int j = 0; j < aliens.size(); j++) {
            if(bullets[i].getBounds().collides(aliens[j].getBounds())) {
                spawnPowerup(j);
                bullets.erase(bullets.begin() + i);
                break;
            }
        }
    }

    for(int i = 0; i < aliens.size(); i++) {
        aliens[i].update(*this);
        if(aliens[i].position.y < -100) aliens.erase(aliens.begin() + i);
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

void Game::spawnPowerup(int alienIndex) {
    Powerup p = Powerup("healthBoost");
    p.position = aliens[alienIndex].position;
    p.movingDirection = Point(1, 1);
    powerups.push_back(p);
    aliens.erase(aliens.begin() + alienIndex);
}

void Game::applyPowerup(int powerupIndex) {
    if(powerups[powerupIndex].effect == "health") player.healthPoints += powerups[powerupIndex].value;
    if(powerups[powerupIndex].effect == "bursts") player.bursts += powerups[powerupIndex].value;
    if(powerups[powerupIndex].effect == "damage") player.bullet.damage += powerups[powerupIndex].value;
    powerups.erase(powerups.begin() + powerupIndex);
}