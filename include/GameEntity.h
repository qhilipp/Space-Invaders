#pragma once

#include <string>
#include <vector>
#include "BMP.h"
#include "Point.h"
#include "Bounds.h"

using namespace std;

class Game;

class GameEntity {
public:
    BMP baseImg;
    BMP textureImg;
    vector<string> texture;
    Point position;
    Point velocity;
    Point acceleration;
    double terminalVelocity;
    Point movingDirection;
    double drag;
    string boundsBehavior;
    int burstsFired;
    long int lastShotTime;

    GameEntity(string identifier);

    void update(Game& game);
    bool operator==(const GameEntity& other) const;
    Bounds getBounds();
};