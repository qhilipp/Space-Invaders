#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <math.h>
#include <optional>
#include <util.h>
#include <BMP.h>
#include <algorithm>
#include <Models.h>

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
        player.update(this);
        for(BattleEntity alien : aliens) alien.update(this);
        for(int i = 0; i < bullets.size(); i++) {
            bullets[i].update(this);
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

    void update(Game& game) {
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
                erase(game.bullets, this);
            }
        }
    }

    Bounds getBounds() {
        return Bounds(position.x, position.y, baseImg.bmp_info_header.width, baseImg.bmp_info_header.height);
    }
};