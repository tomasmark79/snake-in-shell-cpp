// MIT License
//
// Copyright (c) 2024 Tomáš Mark
//

#ifndef __SNAKE_H__
#define __SNAKE_H__

#include <map>
#include <string>
#include <memory>

static const short MAX_SNAKE_LENGTH = 256 + 1;

class Snake
{
    int totalW;
    int totalH;
    std::shared_ptr<int[]> snakeCoosX;
    std::shared_ptr<int[]> snakeCoosY;
    std::shared_ptr<int[]> backupCoosX;
    std::shared_ptr<int[]> backupCoosY;

    int id;
    int stepDivider;
    int length;
    int direction;
    bool isDead;
    int deadCode;

    const std::map<int, std::string> deadDescription{
        {0, "is living"}, {1, "wall"}, {2, "self"}, {3, "another Snake"}};

    void inhumeSnake();
    void backupCoordinates();
    void restoreCoordinatesShifted();
    bool isStepBack(int directionPassed) const;

public:
    Snake(int id, int stepDivider, int totalW, int totalH, int spawnX, int spawnY);
    ~Snake();
    void setMyDirectionAndShift(int directionPassed, bool stopSnake = false);

    int getDirection() const
    {
        return this->direction;
    };

    void setDeadAndCode(int deadCode)
    {
        this->deadCode = deadCode;
        this->isDead = true;
    };

    int getDeadCode() const
    {
        return this->deadCode;
    };

    bool getIsDead() const
    {
        return this->isDead;
    };

    std::string getDeadDescripion() const
    {
        return this->deadDescription.at(deadCode);
    };

    // eat fruit and grow up
    int getElementOfEattenFruit(
        const int *FruitX,
        const int *FruitY,
        int fruitCount) const;

    void growUp()
    {
        this->length += 1;
    };

    int getLength() const
    {
        return this->length;
    };

    int getXHead() const
    {
        return this->snakeCoosX[0];
    };

    int getYHead() const
    {
        return this->snakeCoosY[0];
    }

    const int *getX() const
    {
        return this->snakeCoosX.get();
    };

    const int *getY() const
    {
        return this->snakeCoosY.get();
    };

    void setX(int index, int value)
    {
        this->snakeCoosX[index] = value;
    }

    const int *getBackupX() const
    {
        return this->backupCoosX.get();
    };

    const int *getBackupY() const
    {
        return this->backupCoosY.get();
    };
};

#endif // __SNAKE_H__