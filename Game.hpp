#include "CppEnetWrapper.hpp"
#include "Field.hpp"
#include "Player.hpp"
#include "Snake.hpp"
#include "Fruit.hpp"
#include "Graphic.hpp"
#include "Keyboard.hpp"
#include <chrono>
#include <string>
#include <vector>
#include <map>

static const int SCORE_MULTIPLIER = 11;
static const int SNAKE_SPEED = 1;

class Game
{
    int totalPlayers;
    int totalDeadPlayers;
    std::shared_ptr<Field> field;
    std::unique_ptr<Fruit> fruit;
    std::unique_ptr<Graphic> graphic; // Graphic* Graphic
    std::unique_ptr<std::unique_ptr<Player>[]> players;
    std::unique_ptr<std::shared_ptr<Snake>[]> snakes;
    
    


    CppEnetWrapper::MyEnetNetwork &net;
    bool isMultiplayerActive;
    bool isRemoteSnakeSync = false;
    bool isSomeoneDisconnected = false;

public:
    Game(int width,
         int height,
         double fruitEmptiness,
         int totalPlayers,
         std::string *playerNames,
         CppEnetWrapper::MyEnetNetwork &net,
         bool isMultiplayerActive);

    ~Game();

    void mainLoop();
    void checkSnakeConflicts(int currSnake);

    bool isGameGoingOn;
    bool getIsGameGoingOn() const;
};
