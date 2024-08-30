#include <string>
#include <iostream>
#include <algorithm>
#include <sys/time.h>
#include "Game.h"
#include "util.h"
#include <unistd.h>


using namespace std;

string originalIdentifier;

Game::Game(string identifier): player(BattleEntity("player")), bounds(Bounds(0, 0, 210, 55)) {
    aliens = {};
    originalIdentifier = identifier;
    string json = getJSON(identifier, "games");
    player = BattleEntity(jsonStringValue(json, "player"));
    loadAliens();
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

    GameState gameState = getGameState();
    if(gameState == GameState::NEXT_LEVEL) {
        loadAliens();
    }
}

GameState Game::getGameState() {
    for(int i = 0; i < aliens.size(); i++) {
        if(aliens[i].position.y >= bounds.size.y - player.getBounds().size.y) {
            return GameState::GAME_OVER;
        }
    }
    if(aliens.size() == 0) {
        loadAliens();
    }
    return GameState::PLAYING;
}

void Game::loadAliens() {
    string json = getJSON(originalIdentifier, "games");
    vector<string> alienRows = jsonStringArrayValue(json, "aliens");
    for(int i = 0; i < alienRows.size(); i++) {
        Alien alien = Alien(alienRows[i]);
        alien.position = Point(i * (alien.getBounds().size.x + 4), 5);
        alien.movingDirection = Point(1, 1);
        aliens.push_back(alien);
    }
    initialAlienCount = aliens.size();
}

long int getTime() {
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return tp.tv_sec * 1000 + tp.tv_usec / 1000;
}

void Game::spawnPlayerShoot() { 
    // Check if the delay between bursts has passed and if all bursts have been fired
    if (getTime() - player.lastShotTime < player.bullet.delay) {
        if (player.burstsFired >= player.bursts) {
            return; // If we already fired all bursts, return
        }
    } else {
        // If the full delay has passed, reset for a new burst sequence
        player.burstsFired = 0;
        player.lastShotTime = getTime();
    }

    // Check if we are within the time to fire the next bullet in the burst
    if (getTime() - player.lastShotTime < player.bullet.delay / 5) {
        return; // Not yet time to fire the next bullet in the burst
    }
    player.lastShotTime = getTime();
    
    // Create and position the bullet
    Bullet b = player.bullet;
    b.position.x = player.position.x + player.getBounds().size.x / 2;
    b.position.y = player.position.y;
    b.movingDirection = Point(0, -1);
    bullets.push_back(b);
    player.burstsFired++;
}

void Game::spawnPowerup(int alienIndex) {
    Powerup p = Powerup("healthBoost");
    p.position = aliens[alienIndex].position;
    p.movingDirection = Point(1, 1);
    powerups.push_back(p);
    aliens.erase(aliens.begin() + alienIndex);
    player.aliensKilled++;
}

void Game::applyPowerup(int powerupIndex) {
    if(powerups[powerupIndex].effect == "health") player.healthPoints += powerups[powerupIndex].value;
    if(powerups[powerupIndex].effect == "bursts") player.bursts += powerups[powerupIndex].value;
    if(powerups[powerupIndex].effect == "damage") player.bullet.damage += powerups[powerupIndex].value;
    powerups.erase(powerups.begin() + powerupIndex);
}