# Space Invaders

## Abstract
This is an implementation of the classic "Space Invaders" 80's game with great customization options. Even though this game runs in the terminal, it still uses images to display game entities. That's why we recommend to scale down the terminal font, in order to have the optimal gaming experience.

## Prerequisites
The **c++ compiler** and **nurses** library are mandatory. The **boost** library is optional for testing.

## Installation
Clone this repository cd into the repo and execute `make` and then `make run` to build and run the game.

## Testing
To build the tests execute `make tests` and to run the tests execute `make run_tests`. The test execution might produce some error looking output. E.g. that some key in a json object cannot be found, but that is actually expected behavior, since we also test error cases.

## Controls
As you will find out, all controls are fully customizable with the games.json file. To use left, right, up or down arrow key, enter "left", etc. in lower case. Depending on your terminal, the behavior of the key input might be different. You can hold each key to repeat the action of that key in the next frame, but some terminals realize that you hold the key very late and therefore there will be a short amount of time after you first press down the key, in which nothing happens. In those cases you can increase the acceleration and terminalVelocity of the player and move by repeatedly pressing that key.

## Customization
As mentioned before, this game is highly customizable. Everything can be found in the resources directory. There you will find different json files and other directories. You can play around with the values, but when adding new objects to the json file, you will find that the game no longer works. This is because each json object must correspond to one image directory. An image directory contains three files: `[name]_base.bmp`, `[name]_texture.bmp` and `[name]_texture.txt`. These three files make up the texture for a game entity. The base file is the "background" for the entity, which is overlayed by the `[name]_texture.txt` with the colors of the `[name]_texture.bmp` file. The texture gives the final image more detail. So if you add an object in a json file, you must also add an image directory with that name and the given files. See "Creating textures" to find out more about how you can easily add textures and see "Structure of the models" to find out, how to properly add more objects with the json files.

## Creating textures
In the repository you will also find a `converter.py` file, which can help you creating textures. Once you have created the image directory with a `[name]_texture.bmp` you can execute this python script and input the name of the image directory. This script then creates the .txt file filled with '#' for each non-transparent pixel of the `[name]_texture.bmp` file, giving you an outline, of how the texture will look. Please be aware, that all three files in each image directory must have the same dimensions.

## Structure of the models
At the basis is the `GameEntity`. GameEntities are objects, that can live within the game. They have a texture, position, velocity, acceleration, etc. New game entities can be added in the gameEntities.json file. Be aware, that not all properties can be set within the json file, since e.g. the position will be determined by the game.
On the next level are `Powerup`, `BattleEntity` and `Bullet`. `Powerup`s have a probability, with which they spawn, after an alien was killed, an effect, which can be one of the following values: "health", "bursts", "damage" and a value, representing by how much the property specified by effect will be enhanced. `Bullet` is self-explanatory. The player is an instance of`BattleEntity`, which is a `GameEntity` that has healthPoints, the ability to shoot and so on. If you want to add a `BattleEntity`, `Powerup` or `Bullet`, you must also add a `GameEntity`instance with the same name. The same is also true for all other subclasses. The next and last level is the `Alien` class, which is a subclass of `BattleEntity`. An alien has an array of powerups it can drop, the minimum and maximum time interval, that this alien will fire a bullet and the amount of points, the player is rewarded by killing this alien.
Then, completely separate from all other models is the `Game`, which holds information on which player, aliens, key mappings to use and how the aliens advance with higher levels. The program will load the game called "selected". So to select another game preset, just rename that json object to "selected" and the previous selected one to another arbitrary name.

## Side notes
Since most terminals are not made to be a graphical game environment, the graphics might not look as they do in the .bmp files. The most accurate terminal we could find was the default macOS terminal. Other more modern terminals like Warp try to make text more readable by manipulating the colors, which results in awkward looking game entities.