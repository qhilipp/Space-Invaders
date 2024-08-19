#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <math.h>
#include <optional>
#include <util.h>

using namespace std;

struct Point {
public:
	double x, y;
	Point(double x, double y): x(x), y(y) {}
	Point(): x(0), y(0) {}

    double length() {
        return sqrt(x * x + y * y);
    }

    static Point add(Point a, Point b) {
        return Point(a.x + b.x, a.y + b.y);
    }
};

struct Bounds {
public:
    Point position, size;

    Bounds(Point position, Point size): position(position), size(size) {}
    Bounds(double x, double y, double width, double height): position(Point(x, y)), size(Point(width, height)) {}
};

class GameEntity {
public:
    static Bounds worldBounds;

    BMP baseImg;
    BMP textureImg;
    vector<string> texture;
    Point position;
    Point velocity;
    Point acceleration;
    double terminalVelocity;

    GameEntity(string name)
        : baseImg((name + string("/") + name + string("_base.bmp")).c_str()),
          textureImg((name + string("/") + name + string("_texture.bmp")).c_str())
    {
        ifstream file((string("resources/") + name + string("/") + name + string("_texture.txt")).c_str());
        string line;
        if(file.is_open()) {
            while (getline(file, line)) {
                texture.push_back(line);
            }
            file.close();
        } else {
            cerr << "Unable to open texture file.\n";
        }
    }

    void update() {
        Point updatedVelocity = Point::add(velocity, acceleration);
        if(updatedVelocity.length() <= terminalVelocity) velocity = updatedVelocity;

        Point updatedPosition = Point::add(position, velocity);
        if(updatedPosition.length() <= terminalVelocity) position = updatedPosition;

    }

    Bounds getBounds() {
        return Bounds(position.x, position.y, baseImg.bmp_info_header.width, baseImg.bmp_info_header.height);
    }
};

class Bullet: public GameEntity {
public:
    int damage;
    int delay;
    double direction;

    Bullet(string name, int damage, int delay, double direction): 
        GameEntity(name), 
        damage(damage), 
        delay(delay), 
        direction(direction)
    {}

    Bullet(string identifier): GameEntity(identifier) {
        string json = getJSON(identifier, "bullets");
        damage = jsonIntValue(json, "damage");
        delay = jsonIntValue(json, "delay");
        direction = jsonDoubleValue(json, "direction");
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

    BattleEntity(string identifier): GameEntity(identifier), bullet(Bullet(jsonStringValue(getJSON(identifier, "battleEntities"), "bullet"))) {
        string json = getJSON(identifier, "battleEntities");
        bursts = jsonIntValue(json, "bursts");
        healthPoints = jsonIntValue(json, "healthPoints");
    }
};

class Game {
public:
    BattleEntity player;
    vector<BattleEntity> aliens;

    Game(string identifier) {
        string json = getJSON(identifier, "games");
        player = BattleEntity()
    }

    void update() {
        player.update();
        for(int i = 0; i < aliens.size(); i++) aliens[i].update();
    }
};