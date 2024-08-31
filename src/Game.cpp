#include <string>
#include <iostream>
#include <algorithm>
#include <ctime>
#include <unistd.h>
#include <math.h>
#include "Game.h"
#include "util.h"

using namespace std;

string originalIdentifier;

Game::Game(string identifier): player(BattleEntity("player")), bounds(Bounds(0, 0, 0, 0)), gameOverText(GameEntity("gameOver")) {
    originalIdentifier = identifier;
    string json = getJSON(identifier, "games");
    player = BattleEntity(jsonStringValue(json, "player"));
    player.shootingDirection = -1;
    alienSpeedGain = jsonDoubleValue(json, "alienSpeedGain");
    alienHealthGainFactor = jsonDoubleValue(json, "alienHealthGainFactor");
    
    keyMap[getKeyCode(jsonStringValue(json, "left"))] = Input::LEFT;
    keyMap[getKeyCode(jsonStringValue(json, "right"))] = Input::RIGHT;
    keyMap[getKeyCode(jsonStringValue(json, "shoot"))] = Input::SHOOT;
}

void Game::updateBounds(Bounds bounds) {
    this->bounds = bounds;

    player.position.x = (bounds.size.x - player.getBounds().size.x) / 2 + bounds.position.x;
    player.position.y = bounds.size.y - player.getBounds().size.y - 1;

    gameOverText.position.x = ((bounds.size.x + bounds.position.x) - gameOverText.getBounds().size.x) / 2;
    gameOverText.position.y = (bounds.size.y - gameOverText.getBounds().size.y) / 2;

    loadAliens();
}

void Game::update(Input input) {
    if(frozen) return;
    player.movingDirection = Point();
    if(input == Input::LEFT) {
        player.movingDirection.x = -1;
    }
    if(input == Input::RIGHT) {
        player.movingDirection.x = 1;
    }
    if(input == Input::SHOOT) {
        optional<Bullet> bullet = player.shoot(false);
        if(bullet.has_value()) {
            bullets.push_back(bullet.value());
        }
    }
    
    player.update(*this);
    optional<Bullet> b = player.burstShoot();

    if(b.has_value()) {
        bullets.push_back(b.value());
    }

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
                hitAlien(j);
                bullets.erase(bullets.begin() + i);
                break;
            }
        }
    }

    for(int i = alienBullets.size() -1 ; i >= 0; i--) {
        alienBullets[i].update(*this);
        if(alienBullets[i].position.y < -100) alienBullets.erase(alienBullets.begin() + i);
        if(alienBullets[i].getBounds().collides(player.getBounds())) {
            player.healthPoints = max(0, player.healthPoints - alienBullets[i].damage);
            alienBullets.erase(alienBullets.begin() + i);
        }
    }

    for(int i = 0; i < aliens.size(); i++) {
        aliens[i].update(*this);
        optional<Bullet> bullet = aliens[i].autoShoot();
        if(bullet.has_value()) {
            alienBullets.push_back(bullet.value());
        }
        if(aliens[i].position.y < -100) aliens.erase(aliens.begin() + i);
    }

    state = getGameState();
    if(state == GameState::NEXT_LEVEL) {
        loadAliens();
    }
    if(state == GameState::GAME_OVER) {
        frozen = true;
    }
}

GameState Game::getGameState() {
    for(int i = 0; i < aliens.size(); i++) {
        if(aliens[i].position.y >= bounds.size.y - player.getBounds().size.y) {
            return GameState::GAME_OVER;
        }
    }
    if(aliens.size() == 0) {
        return GameState::NEXT_LEVEL;
    }
    if(player.healthPoints <= 0) return GameState::GAME_OVER;
    return GameState::PLAYING;
}

void Game::loadAliens() {
    string json = getJSON(originalIdentifier, "games");
    vector<string> alienIdentifiers = jsonStringArrayValue(json, "aliens");
    aliens = {};
    for(int i = 0; i < alienIdentifiers.size(); i++) {
        Alien alien = Alien(alienIdentifiers[i]);
        alien.position = Point(i * (alien.getBounds().size.x + 4) + bounds.position.x, 0);
        alien.movingDirection = Point(1, 1);
        alien.terminalVelocity += alienSpeedGain * level;
        alien.maxHealthPoints *= pow(alienHealthGainFactor, level);
        alien.healthPoints = alien.maxHealthPoints;
        aliens.push_back(alien);
    }
    initialAlienCount = aliens.size();
    level++;
}

void Game::hitAlien(int alienIndex) {
    aliens[alienIndex].healthPoints -= player.bullet.damage;

    if(aliens[alienIndex].healthPoints > 0) return;

    vector<double> probabilities;
    for(Powerup p : aliens[alienIndex].powerups) {
        probabilities.push_back(p.probability);
    }

    int powerupIndex = getRandomIndex(probabilities);
    cout << probabilities.size() << " ";

    if(powerupIndex != -1) {
        Powerup p = aliens[alienIndex].powerups[powerupIndex];
        p.position = aliens[alienIndex].position;
        p.movingDirection = Point(1, 1);
        powerups.push_back(p);
    }

    aliens.erase(aliens.begin() + alienIndex);
    player.kills++;
}

void Game::applyPowerup(int powerupIndex) {
    if(powerups[powerupIndex].effect == "health") player.healthPoints = min(player.maxHealthPoints, player.healthPoints + powerups[powerupIndex].value);
    if(powerups[powerupIndex].effect == "bursts") player.bursts += powerups[powerupIndex].value;
    if(powerups[powerupIndex].effect == "damage") player.bullet.damage += powerups[powerupIndex].value;
    powerups.erase(powerups.begin() + powerupIndex);
}