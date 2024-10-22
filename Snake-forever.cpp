// MIT License
//
// Copyright (c) 2024 Tomáš Mark
//

#define ENET_IMPLEMENTATION

#include "CppVersion.hpp"
#include "Game.hpp"
#include "Graphic.hpp"
#include "UserUtil.hpp"

#include <iostream>
#include <random>
#include <ctime>
#include <vector>
#include <exception>

#define MAX_PLAYERS (int)4
#define PLAYER_ANSWER_TRESHHOLD (int)5
#define DEF_ARENA_WIDTH (int)60
#define DEF_ARENA_HEIGHT (int)25
#define DEF_FRUIT_EMPTYNESS (double)3.5

std::string playerNames[MAX_PLAYERS];

class arenaProperties
{
public:
    int fieldWidth = DEF_ARENA_WIDTH;
    int fieldHeight = DEF_ARENA_HEIGHT;
    int fruitEmptiness = DEF_FRUIT_EMPTYNESS;
} arenaProps;

UserUtil uu;

// Arena size and fruite modification
void setArenaProperties()
{
    arenaProps.fieldWidth = uu.getNumericAnswerFromPlayer<int>(
        "Enter Arena width, [ENTER for 40] ?: ", DEF_ARENA_WIDTH, 40, 360, PLAYER_ANSWER_TRESHHOLD);
    arenaProps.fieldHeight = uu.getNumericAnswerFromPlayer<int>(
        "Enter Arena height, [ENTER for 15] ?: ", DEF_ARENA_HEIGHT, 10, 360, PLAYER_ANSWER_TRESHHOLD);
    arenaProps.fruitEmptiness = uu.getNumericAnswerFromPlayer<double>(
        "Enter fruit emptiness, [ENTER for 3.5] ?: ", DEF_FRUIT_EMPTYNESS, 0.1, 10, PLAYER_ANSWER_TRESHHOLD);
}

int main( /*int argc, char *argv[]*/ )
{
    CppEnetWrapper::MyEnetNetwork net;
    vector<int> eraryVector;
    bool isMultiplayerActive = false;

    std::random_device rd;
    std::mt19937 gen(rd());

    std::cout << getAppBanner();

    int totalPlayers = MAX_PLAYERS;
    playerNames[0] = "Tomas";
    playerNames[1] = "Lucia";
    playerNames[2] = "Inge";
    playerNames[3] = "Milan";

    std::string isUDPMultiplayer =
        uu.getStringAnswerFromPlayer("Play Network Multiplayer? y/n [ENTER for no]: ", "no", 1, 3, PLAYER_ANSWER_TRESHHOLD);

    if (uu.isAnswerYes(isUDPMultiplayer))
    {
        isMultiplayerActive = true;
        totalPlayers = 2;

        // Network Multiplayer
        std::string isThisGameSessionServer =
            uu.getStringAnswerFromPlayer("HOST Network Game? y/n [ENTER for no]: ", "no", 1, 3, PLAYER_ANSWER_TRESHHOLD);
        if (uu.isAnswerYes(isThisGameSessionServer))
        {
            std::string bindIp =
                uu.getStringAnswerFromPlayer("Enter FQDN, IP address or [ENTER for localhost]: ",
                                             "localhost", 7, 15, PLAYER_ANSWER_TRESHHOLD);

            int bindPort = uu.getNumericAnswerFromPlayer<int>(
                "Enter port? [ENTER for 7996]: ", 7996, 1, 65535, PLAYER_ANSWER_TRESHHOLD);

            // Server Session Game
            net
                .setIsServer(true)
                //.setBindHostName("192.168.79.101")
                //.setBindHostName("localhost")
                .setBindHostName(bindIp)
                .setBindPort(bindPort)
                .setMaxClients(2)
                .setChannels(2)
                .setAmountIn(0)
                .setAmountOut(0);

            if (net.initENet() != 0)
                return 1;
            if (net.registerClients(1000, 10) != 0)
                return 1;
        }
        else
        {
            // Client Session Game
            std::string remoteIp =
                uu.getStringAnswerFromPlayer("Enter !!REMOTE!! FQDN, IP address or [ENTER for localhost]: ",
                                             "localhost", 7, 15, PLAYER_ANSWER_TRESHHOLD);

            int remotePort = uu.getNumericAnswerFromPlayer<int>(
                "Enter !!REMOTE!! port? [ENTER for 7996]: ", 7996, 1, 65535, PLAYER_ANSWER_TRESHHOLD);

            net
                .setIsServer(false)
                // .setRemoteHostName("192.168.79.101")
                // .setRemoteHostName("localhost")
                .setRemoteHostName(remoteIp)
                .setRemotePort(remotePort)
                .setOutConnections(1);

            if (net.initENet() != 0)
                return 1;
            if (net.registerServer(1000, 10) != 0)
                return 1;
        }
    }
    else
    {
        if (uu.isAnswerYes(uu.getStringAnswerFromPlayer(
                "Play HOT-SEAT Game? (up to 4 players) y/n [ENTER for no]: ", "no", 1, 3, PLAYER_ANSWER_TRESHHOLD)))
        {
            // Hot-SEAT
            if (uu.isAnswerYes(uu.getStringAnswerFromPlayer(
                    "Modify Arena size? y/n [ENTER for no]: ", "no", 1, 3, PLAYER_ANSWER_TRESHHOLD)))
            {
                setArenaProperties();
            }
            totalPlayers = uu.getNumericAnswerFromPlayer<int>(
                "How many players will play? [ENTER for 1] 1-4: ", 1, 1, 4, PLAYER_ANSWER_TRESHHOLD);

            if (uu.isAnswerYes(uu.getStringAnswerFromPlayer(
                    "Modify player names? y/n [ENTER for no]: ", "no", 1, 3, PLAYER_ANSWER_TRESHHOLD)))
            {
                for (int playerId = 0; playerId < totalPlayers; playerId++)
                    playerNames[playerId] = uu.getStringAnswerFromPlayer("Enter name of " + playerNames[playerId] + " ?: ", "Snake", 2, 20, PLAYER_ANSWER_TRESHHOLD);
            }
        }
        else
        {
            // Single Player Session
            totalPlayers = 1;
            if (uu.isAnswerYes(uu.getStringAnswerFromPlayer(
                    "Modify Arena size? y/n [ENTER for no]: ", "no", 1, 3, PLAYER_ANSWER_TRESHHOLD)))
            {
                setArenaProperties();
            }
        }
    }

    // Start the game
    Game gameSnake(arenaProps.fieldWidth,
                   arenaProps.fieldHeight,
                   arenaProps.fruitEmptiness,
                   totalPlayers,
                   playerNames,
                   net,
                   isMultiplayerActive);
    return 0;
}
