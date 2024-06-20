#ifndef __PLAYER_H__
#define __PLAYER_H__


#include <string>

class Player
{
    int playerId;
    std::string playerName;

public:
    Player(int playerId, std::string playerName)
        : playerId(playerId), playerName(playerName){};

    void setPlayerName(std::string playerName)
    {
        this->playerName = playerName;
    };

    std::string getPlayerName() const
    {
        return this->playerName;
    };
};

#endif // __PLAYER_H__