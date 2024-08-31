#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "Bounds.h"
#include "BattleEntity.h"
#include "Bullet.h"
#include "Powerup.h"
#include "Alien.h"

using namespace std;

enum class Input {
    LEFT, RIGHT, SHOOT, NONE, QUIT
};

enum class GameState {
    PLAYING, GAME_OVER, NEXT_LEVEL
};

class Game {
public:
    GameEntity gameOverText;
    BattleEntity player;
    vector<Alien> aliens;
    vector<Bullet> bullets;
    vector<Bullet> alienBullets;
    vector<Powerup> powerups;
    Bounds bounds;
    unordered_map<int, Input> keyMap;
    double alienSpeedGain;
    double alienHealthGainFactor;
    int initialAlienCount;
    int level;
    bool frozen = false;
    GameState state = GameState::PLAYING;

    Game(string identifier);

    void update(Input input);
    void updateBounds(Bounds bounds);
    GameState getGameState();
    void loadAliens();
    void spawnPlayerShoot();
    void hitAlien(int alienIndex);
    void applyPowerup(int powerupIndex);
};