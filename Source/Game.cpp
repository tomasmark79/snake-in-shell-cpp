// MIT License
//
// Copyright (c) 2024 Tomáš Mark
//

#include "Game.hpp"
#include <thread>  // std::this_thread::sleep_for
#include <iomanip> // std::setprecision
#include <sstream> // sstreamstring
#include <exception>
#include "Sounds.hpp"

Game::Game(int width, int height, double fruitEmptiness,
           int totalPlayers, std::string *playerNames, CppEnetWrapper::MyEnetNetwork &net, bool isMultiplayerActive)
    : totalPlayers(totalPlayers),
      totalDeadPlayers(0),
      field(std::make_shared<Field>(width, height)),
      fruit(std::make_unique<Fruit>(fruitEmptiness, width, height)),
      graphic(std::make_unique<Graphic>(width, height)),
      players(std::make_unique<std::unique_ptr<Player>[]>(totalPlayers + 1)),
      snakes(std::make_unique<std::shared_ptr<Snake>[]>(totalPlayers + 1)),
      isGameGoingOn(true), net(net), isMultiplayerActive(isMultiplayerActive)
{
    // spawn player's id and names
    for (int playerId = 0; playerId < totalPlayers; playerId++)
    {
        players[playerId] = std::make_unique<Player>(playerId, playerNames[playerId]);
        snakes[playerId] = std::make_unique<Snake>(playerId, SNAKE_SPEED, width, height, width / 2, height / 2);
    }

    this->mainLoop();
}

Game::~Game()
{
    snakes.reset();
    players.reset();
    graphic.reset();
    fruit.reset();
    field.reset();
}

#define DISCONNECTED (int)2
#define DATA_RECEIVED (int)3
// Network protocol for Snake
// [0] = keyboard command
// [1] = reserved for future
// [2] = reserved for future of future

vector<int> bufToSendtoServer = {1, 0};
vector<int> bufToSendtoClient = {1, 0};
vector<int> eraryVector;

void Game::mainLoop()
{
    // TODO Play Background Music

    int eattenFruitElement = 0;
    int playerPoints[4] = {0, 0, 0, 0};
    std::string playerStats[4] = {"", "", "", ""};

    std::chrono::time_point<std::chrono::system_clock> start_time = std::chrono::high_resolution_clock::now();

    Keyboard keyboard;

    while (true)
    {
        std::chrono::time_point<std::chrono::system_clock> end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed_time = (end_time - start_time) / 1000;

        int playerInput[4] = {-1, -1, -1, -1};
        int remoteKeyboardCode = -1;
        int keyboardCode = keyboard.getMyKeyboardCode();

        if (isMultiplayerActive)
        {

            isRemoteSnakeSync = false;
            isSomeoneDisconnected = false;

            // Store pressed keyboard to colleague session
            bufToSendtoClient[0] = keyboardCode;
            bufToSendtoServer[0] = keyboardCode;

            if (net.getIsServer())
            {
                // wait until data received
                time_t startTime = time(nullptr);
                // 5 seconds timout if some noise on the way
                while (time(nullptr) - startTime < 5)
                {
                    // 50 ms for get the answer for the other side
                    int status = net.serverHostService(eraryVector, 50);
                    if (eraryVector.size() > 0 && status == DATA_RECEIVED)
                    {
                        // Snake 2 took remote direction
                        remoteKeyboardCode = (eraryVector[0]);
                        std::cout << std::to_string(remoteKeyboardCode);
                        isRemoteSnakeSync = true; /* we get an answer - snake can move */
                        break;
                    }
                    else if (status == DISCONNECTED)
                    {
                        isSomeoneDisconnected = true;
                        break;
                    }
                }

                // Local Snake Directions
                if (keyboardCode >= 10 && keyboardCode <= 13)
                    playerInput[0] = keyboardCode - 10;

                // Send local Direction to other side
                net.sendVectorToClient(bufToSendtoClient, bufToSendtoClient.size());

                // Get remote Snake Directions
                if (remoteKeyboardCode >= 10 && remoteKeyboardCode <= 13)
                    playerInput[1] = remoteKeyboardCode - 10;
            }
            else
            {
                // receiving data from other side
                time_t startTime = time(nullptr);
                // 5 seconds timout if some noise on the way
                while (time(nullptr) - startTime < 5)
                {
                    // 50 ms for get the answer for the other side
                    int status = net.clientHostService(eraryVector, 50);
                    if (eraryVector.size() > 0 && status == DATA_RECEIVED)
                    {
                        // Snake 2 took remote direction
                        remoteKeyboardCode = (eraryVector[0]);
                        std::cout << std::to_string(remoteKeyboardCode);
                        isRemoteSnakeSync = true; /* we get an answer - snake can move */
                        break;
                    }
                    else if (status == DISCONNECTED)
                    {
                        isSomeoneDisconnected = true;
                        break;
                    }
                }

                // Local Snake Directions
                if (keyboardCode >= 10 && keyboardCode <= 13)
                    playerInput[0] = keyboardCode - 10;

                // Send local Direction to other side
                net.sendVectorToServer(bufToSendtoServer, bufToSendtoServer.size());

                // Get remote Snake Directions
                if (remoteKeyboardCode >= 10 && remoteKeyboardCode <= 13)
                    playerInput[1] = remoteKeyboardCode - 10;
            }
        }
        else
        {
            isRemoteSnakeSync = true; // always true for local game
            // split keyboard input to players if no network multiplayer
            if (keyboardCode >= 10 && keyboardCode <= 13)
                playerInput[0] = keyboardCode - 10;
            else if (keyboardCode >= 20 && keyboardCode <= 23)
                playerInput[1] = keyboardCode - 20;
            else if (keyboardCode >= 30 && keyboardCode <= 33)
                playerInput[2] = keyboardCode - 30;
            else if (keyboardCode >= 40 && keyboardCode <= 43)
                playerInput[3] = keyboardCode - 40;
        }

        // clear
        graphic->clearVideoBuffer();

        // wall
        graphic->addWallsToVideoBuffer();

        // fruit
        graphic->addFruitToVideoBuffer(
            fruit->getFruitX(),
            fruit->getFruitY(),
            fruit->getTotalFruit());

        // go through snakes
        for (int currSnake = 0; currSnake < this->totalPlayers; currSnake++)
        {
            // Snake direction and movement and buffer filling
            snakes[currSnake]->setMyDirectionAndShift(playerInput[currSnake], !isRemoteSnakeSync);

            // Order is important - We want also to draw died snake
            this->checkSnakeConflicts(currSnake);

            graphic->addSnakeToVideoBuffer(currSnake,
                                           snakes[currSnake]->getX(),
                                           snakes[currSnake]->getY(),
                                           snakes[currSnake]->getLength(),
                                           snakes[currSnake]->getIsDead());

            if ((eattenFruitElement = snakes[currSnake]->getElementOfEattenFruit(
                     fruit->getFruitX(),
                     fruit->getFruitY(),
                     fruit->getTotalFruit())) > 0)
            {
                snakes[currSnake]->growUp();
                fruit->refreshFruit(eattenFruitElement - 1);

                std::shared_ptr<Sounds> sounds;
                sounds = std::make_shared<Sounds>();
                sounds.get()->PlaySoundSync("tick.wav", 0);
            }

            playerPoints[currSnake] = snakes[currSnake]->getLength() * SCORE_MULTIPLIER;
            playerStats[currSnake] = "Player with Snake " + players[currSnake]->getPlayerName() + " Points: " + std::to_string(playerPoints[currSnake]) + (snakes[currSnake]->getIsDead() ? " was killed by " + snakes[currSnake]->getDeadDescripion() : " lives");

        } // go through snakes end
        graphic->redrawVideoBuffer();

        // print stats
        for (int i = 0; i < totalPlayers; i++)
            graphic->coutVCentered(playerStats[i], field->getFieldHeight() + i + 1);
        graphic->coutVCentered("Duration: " +
                               std::to_string((int)elapsed_time.count()) + "s", field->getFieldHeight() + totalPlayers + 1);

        // Additive multiplayer conditions
        if (isMultiplayerActive)
        {
            if (net.getIsServer())
                graphic->coutVCentered("Server Sesion", field->getFieldHeight() + totalPlayers + 2);
            else
                graphic->coutVCentered("Client Sesion", field->getFieldHeight() + totalPlayers + 2);
            
            graphic->coutVCentered("(H)elp | (R)estart | e(X)it", field->getFieldHeight() + totalPlayers + 3);

            if (isSomeoneDisconnected)
                break;
        }

        // all players dead
        if (totalDeadPlayers == totalPlayers)
        {
            graphic->coutGameOver();
            break;
        }

        // restart game
        if (keyboardCode == 6 || remoteKeyboardCode == 6)
            break;

        if (keyboardCode == 4 || remoteKeyboardCode == 4)
        {
            this->isGameGoingOn = false;
            break;
        }

        // help game
        if (keyboardCode == 5 || remoteKeyboardCode == 5)
            graphic->coutHelp();

        // sleep interval in ms
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }
}

//! \brief  Check Snake conflict with environment
//!
//! \param  Current element number of Snake instance
//! \return 1 wall, 2 itself, 3 foreign snake conflict
//!
//!
// TODO (tomas#1#): Required remove returns, but mixed condition can occur
void Game::checkSnakeConflicts(int currSnake)
{
    if (snakes[currSnake]->getIsDead())
        return;

    // hitting wall check
    if (snakes[currSnake]->getXHead() == 0 ||
        snakes[currSnake]->getXHead() == field->getFieldWidth() - 1 ||
        snakes[currSnake]->getYHead() == 0 ||
        snakes[currSnake]->getYHead() == field->getFieldHeight() - 1)
    {
        snakes[currSnake]->setDeadAndCode(1);
        totalDeadPlayers++;
        // TODO
        std::shared_ptr<Sounds> sounds;
        sounds = std::make_shared<Sounds>();
        sounds.get()->PlaySoundSync("wall.wav", 800);
        return;
    }

    // hitting itsefl check
    for (int tail = 1; tail < snakes[currSnake]->getLength(); tail++)
    {
        if (snakes[currSnake]->getXHead() == snakes[currSnake]->getBackupX()[tail] &&
            snakes[currSnake]->getYHead() == snakes[currSnake]->getBackupY()[tail])
        {
            snakes[currSnake]->setDeadAndCode(2);
            totalDeadPlayers++;
            // TODO
            std::shared_ptr<Sounds> sounds;
            sounds = std::make_shared<Sounds>();
            sounds.get()->PlaySoundSync("self.wav", 800);

            return;
        }
    }

    // looking for foreign snake instance
    for (int foreignSnake = 0; foreignSnake < totalPlayers; foreignSnake++)
    {
        // looking for foreign snake tail
        for (int tail = 0; tail < snakes[foreignSnake]->getLength(); tail++)
        {
            // check conflict only for snake with longer tail than 2
            // check conflict only with stranger snake
            // ignore here self conflict
            if (currSnake != foreignSnake               /* ignore self instance conflict */
                && !snakes[foreignSnake]->getIsDead()   /* if stranger is not dead */
                && snakes[currSnake]->getLength() >= 2) /* if current snake is longer than 2*/
            {
                if (snakes[currSnake]->getXHead() == snakes[foreignSnake]->getBackupX()[tail] &&
                    snakes[currSnake]->getYHead() == snakes[foreignSnake]->getBackupY()[tail])
                {
                    // eats each other
                    snakes[currSnake]->setDeadAndCode(3);
                    totalDeadPlayers++;
                    return;
                }
            }
        }
    }
}

bool Game::getIsGameGoingOn() const
{
    return this->isGameGoingOn;
}
