#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "BMP.h"
#include "Point.h"
#include "Bounds.h"
#include "util.h"
#include "GameEntity.h"
#include "Game.h"

using namespace std;

GameEntity::GameEntity(string identifier)
    : baseImg((identifier + string("/") + identifier + string("_base.bmp")).c_str()),
      textureImg((identifier + string("/") + identifier + string("_texture.bmp")).c_str())
{
    ifstream file((string("resources/") + identifier + string("/") + identifier + string("_texture.txt")).c_str());
    string line;
    if(file.is_open()) {
        while (getline(file, line)) {
            texture.push_back(line);
        }
        file.close();
    } else {
        cerr << "Unable to open texture file.\n";
    }

    string json = getJSON(identifier, "gameEntities");
    string positionJSON = jsonObjectValue(json, "position");
    position.x = jsonDoubleValue(positionJSON, "x");
    position.y = jsonDoubleValue(positionJSON, "y");

    string accelerationJSON = jsonObjectValue(json, "acceleration");
    acceleration.x = jsonDoubleValue(accelerationJSON, "x");
    acceleration.y = jsonDoubleValue(accelerationJSON, "y");
    terminalVelocity = jsonDoubleValue(json, "terminalVelocity");
    drag = jsonDoubleValue(json, "drag");
    boundsBehaviour = jsonStringValue(json, "boundsBehaviour");
}

void GameEntity::update(Game& game) {
    Point movingAcceleration = acceleration * movingDirection;
    Point updatedVelocity = velocity + movingAcceleration;
    if(movingDirection.length() == 0) updatedVelocity = updatedVelocity * (1 / drag);

    velocity = updatedVelocity.unit() * min(updatedVelocity.length(), terminalVelocity); // Make sure, that it's not faster than the terminal velocity

    Point updatedPosition = position + velocity;
    position = updatedPosition;

    if(boundsBehaviour == "stop") {
        if(position.x < game.bounds.position.x) {
            position.x = game.bounds.position.x;
            velocity.x = 0;
        }
        if(position.x + getBounds().size.x > game.bounds.position.x + game.bounds.size.x) {
            position.x = game.bounds.position.x + game.bounds.size.x - getBounds().size.x;
            velocity.x = 0;
        }
        if(position.y < game.bounds.position.y) {
            position.y = game.bounds.position.y;
            velocity.y = 0;
        }
        if(position.y + getBounds().size.y > game.bounds.position.y + game.bounds.size.y) {
            position.y = game.bounds.position.y + game.bounds.size.y - getBounds().size.y;
            velocity.y = 0;
        }
    }
    if(boundsBehaviour == "wrap") {
        if(position.x < game.bounds.position.x) {
            position.x = game.bounds.position.x + game.bounds.size.x - getBounds().size.x;
        }
        if(position.x + getBounds().size.x > game.bounds.position.x + game.bounds.size.x) {
            position.x = game.bounds.position.x;
        }
        if(position.y < game.bounds.position.y) {
            position.y = game.bounds.position.y + game.bounds.size.y - getBounds().size.y;
        }
        if(position.y + getBounds().size.y > game.bounds.position.y + game.bounds.size.y) {
            position.y = game.bounds.position.y;
        }
    }
    if(boundsBehaviour == "bounce") {
        if(position.x < game.bounds.position.x) {
            position.x = game.bounds.position.x;
            velocity.x *= -1;
            movingDirection.x *= -1;
        }
        if(position.x + getBounds().size.x > game.bounds.position.x + game.bounds.size.x) {
            position.x = game.bounds.position.x + game.bounds.size.x - getBounds().size.x;
            velocity.x *= -1;
            movingDirection.x *= -1;
        }
        if(position.y < game.bounds.position.y) {
            position.y = game.bounds.position.y;
            velocity.y *= -1;
            movingDirection.y *= -1;
        }
        if(position.y + getBounds().size.y > game.bounds.position.y + game.bounds.size.y) {
            position.y = game.bounds.position.y + game.bounds.size.y - getBounds().size.y;
            velocity.y *= -1;
            movingDirection.y *= -1;
        }
    }
    if(boundsBehaviour == "delete") {
        if(
            position.x + getBounds().size.x < game.bounds.position.x ||
            position.x > game.bounds.position.x + game.bounds.size.x ||
            position.y + getBounds().size.y < game.bounds.position.y ||
            position.y > game.bounds.position.y + game.bounds.size.y
        ) {
            position.y = -100000;
        }
    }
}

bool GameEntity::operator==(const GameEntity& other) const {
        return this->position.x == other.position.x && this->position.y == other.position.y;
    }

Bounds GameEntity::getBounds() {
    return Bounds(position.x, position.y, baseImg.bmp_info_header.width, baseImg.bmp_info_header.height);
}