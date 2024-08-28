#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <math.h>
#include <optional>
#include <util.h>
#include <BMP.h>

using namespace std;

struct Point {
public:
	double x, y;
	Point(double x, double y): x(x), y(y) {}
	Point(): x(0), y(0) {}

    double length() {
        return sqrt(x * x + y * y);
    }

    Point operator+(const Point& other) const {
        return Point(x + other.x, y + other.y);
    }

    Point operator*(const Point& other) const {
        return Point(x * other.x, y * other.y);
    }

    Point operator*(double f) const {
        return Point(x * f, y * f);
    }

    friend Point operator*(double f, const Point& p) {
        return Point(p.x * f, p.y * f);
    }

    Point unit() {
        double l = length();
        if(l == 0) return Point(0, 0);
        return Point(x / l, y / l);
    }
};

struct Bounds {
public:
    Point position, size;

    Bounds(Point position, Point size): position(position), size(size) {}
    Bounds(double x, double y, double width, double height): position(Point(x, y)), size(Point(width, height)) {}

    bool collides(Bounds other) {
        return (position.x >= other.position.x && position.x <= other.position.x + other.size.x) || (other.position.x >= position.x && other.position.x <= position.x + other.size.x) && (position.y >= other.position.y && position.y <= other.position.y + other.size.y);
    }

    bool contains(Bounds other) {
        return other.position.x >= position.x && other.position.x + other.size.x <= position.x + size.x && other.position.y >= position.y && other.position.y + other.size.y <= position.y + size.y;
    }
};

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
    string boundsBehaviour;

    GameEntity(string identifier)
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

    void update(Bounds bounds) {
        Point movingAcceleration = acceleration * movingDirection;
        Point updatedVelocity = velocity + movingAcceleration;
        if(movingDirection.length() == 0) updatedVelocity = updatedVelocity * (1 / drag);

        velocity = updatedVelocity.unit() * min(updatedVelocity.length(), terminalVelocity); // Make sure, that it's not faster than the terminal velocity

        Point updatedPosition = position + velocity;
        position = updatedPosition;

        if(boundsBehaviour == "stop") {
            if(position.x < bounds.position.x) {
                position.x = bounds.position.x;
                velocity.x = 0;
            }
            if(position.x + getBounds().size.x > bounds.position.x + bounds.size.x) {
                position.x = bounds.position.x + bounds.size.x - getBounds().size.x;
                velocity.x = 0;
            }
            if(position.y < bounds.position.y) {
                position.y = bounds.position.y;
                velocity.y = 0;
            }
            if(position.y + getBounds().size.y > bounds.position.y + bounds.size.y) {
                position.y = bounds.position.y + bounds.size.y - getBounds().size.y;
                velocity.y = 0;
            }
        }
        if(boundsBehaviour == "wrap") {
            if(position.x < bounds.position.x) {
                position.x = bounds.position.x + bounds.size.x - getBounds().size.x;
            }
            if(position.x + getBounds().size.x > bounds.position.x + bounds.size.x) {
                position.x = bounds.position.x;
            }
            if(position.y < bounds.position.y) {
                position.y = bounds.position.y + bounds.size.y - getBounds().size.y;
            }
            if(position.y + getBounds().size.y > bounds.position.y + bounds.size.y) {
                position.y = bounds.position.y;
            }
        }
        if(boundsBehaviour == "bounce") {
            if(position.x < bounds.position.x) {
                position.x = bounds.position.x;
                velocity.x *= -1;
            }
            if(position.x + getBounds().size.x > bounds.position.x + bounds.size.x) {
                position.x = bounds.position.x + bounds.size.x - getBounds().size.x;
                velocity.x *= -1;
            }
            if(position.y < bounds.position.y) {
                position.y = bounds.position.y;
                velocity.y *= -1;
            }
            if(position.y + getBounds().size.y > bounds.position.y + bounds.size.y) {
                position.y = bounds.position.y + bounds.size.y - getBounds().size.y;
                velocity.y *= -1;
            }
        }
    }

    Bounds getBounds() {
        return Bounds(position.x, position.y, baseImg.bmp_info_header.width, baseImg.bmp_info_header.height);
    }
};

class Bullet: public GameEntity {
public:
    int damage;
    int delay;

    Bullet(string name, int damage, int delay): 
        GameEntity(name),
        damage(damage),
        delay(delay)
    {}

    Bullet(string identifier): GameEntity(identifier) {
        string json = getJSON(identifier, "bullets");
        damage = jsonIntValue(json, "damage");
        delay = jsonIntValue(json, "delay");
    }
};

class BattleEntity: public GameEntity {
public:
    int bursts;
    int healthPoints;
    Bullet bullet;

    BattleEntity(string name, int bursts, int healthPoints, Bullet bullet):
        GameEntity(name),
        bursts(bursts),
        healthPoints(healthPoints),
        bullet(bullet)
    {}

    BattleEntity(string identifier): GameEntity(identifier), bullet(Bullet("laser", 0, 0)) {
        string json = getJSON(identifier, "battleEntities");
        bursts = jsonIntValue(json, "bursts");
        healthPoints = jsonIntValue(json, "healthPoints");
        bullet = Bullet(jsonStringValue(json, "bullet"));
    }
};

class Powerup: public GameEntity {
public:
    string effect;
    double probability;
    int value;

    Powerup(string identifier): GameEntity(identifier) {
        string json = getJSON(identifier, "powerups");
        effect = jsonStringValue(json, "effect");
        probability = jsonDoubleValue(json, "probability");
        value = jsonIntValue(json, "value");
    }
};

class Alien: public GameEntity {
public:
    vector<Powerup> powerups;

    Alien(string identifier): GameEntity(identifier) {
        string json = getJSON(identifier, "aliens");
        for(string powerupIdentifier : jsonStringArrayValue(json, "powerups")) {
            powerups.push_back(Powerup(powerupIdentifier));
        }
    }
};

class Game {
public:
    BattleEntity player;
    vector<BattleEntity> aliens;
    vector<Bullet> bullets;
    Bounds bounds;

    Game(string identifier): player(BattleEntity("player")), bounds(Bounds(0, 0, 210, 55)) {
        aliens = {};
        string json = getJSON(identifier, "games");
        player = BattleEntity(jsonStringValue(json, "player"));
    }

    void update(char input) {
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
        player.update(bounds);
        for(BattleEntity alien : aliens) alien.update(bounds);
        for(int i = 0; i < bullets.size(); i++) {
            bullets[i].update(bounds);
            //cout << bullets[i].velocity.y << " " << bullets[i].acceleration.y;
        }
    }

    void spawnPlayerShoot() {
        for(int i = 0; i < player.bursts; i++) {
            Bullet b = player.bullet;
            b.position.x = player.position.x + player.getBounds().size.x / 2;
            b.position.y = player.position.y;
            b.movingDirection = Point(0, -1);
            bullets.push_back(b);
        }
    }

    void applyPowerup(Powerup powerup) {
        if(powerup.effect == "health") player.healthPoints += powerup.value;
        if(powerup.effect == "health") player.bursts += powerup.value;
        if(powerup.effect == "health") player.bullet.damage += powerup.value;
    }
};