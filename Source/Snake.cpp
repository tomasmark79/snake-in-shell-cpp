// MIT License
//
// Copyright (c) 2024 Tomáš Mark
//

#include <memory>
#include "Snake.hpp"

Snake::Snake(int id, int stepDivider, int totalW, int totalH, int spawnX, int spawnY)
    : totalW(totalW), totalH(totalH),
      snakeCoosX(std::make_unique<int[]>(totalW * totalH)),
      snakeCoosY(std::make_unique<int[]>(totalW * totalH)),
      backupCoosX(std::make_unique<int[]>(totalW * totalH)),
      backupCoosY(std::make_unique<int[]>(totalW * totalH)),
      id(id), stepDivider(stepDivider), length(0), direction((rand() % 4) /*3*/), isDead(false), deadCode(0)
{
    // snake baby will born here
    snakeCoosX[0] = spawnX;
    snakeCoosY[0] = spawnY;

    // Now we send no direction at the start of game
    direction = -1;
}

Snake::~Snake()
{
    snakeCoosX.reset();
    snakeCoosY.reset();
    backupCoosX.reset();
    backupCoosY.reset();
}

void Snake::backupCoordinates()
{
    for (int bodyElement = 0; bodyElement < this->length; bodyElement++)
    {
        backupCoosX[bodyElement] = snakeCoosX[bodyElement];
        backupCoosY[bodyElement] = snakeCoosY[bodyElement];
    }
}

void Snake::restoreCoordinatesShifted()
{
    for (int bodyElement = 0; bodyElement < this->length; bodyElement++)
    {
        snakeCoosX[bodyElement + 1] = backupCoosX[bodyElement];
        snakeCoosY[bodyElement + 1] = backupCoosY[bodyElement];
    }
}

// step to back? - doesn't allow for Snake ;-)
bool Snake::isStepBack(int directionPassed) const
{
    if (this->isDead)
        return false;

    if (this->length > 0)
    {
        if ((this->direction == 0 && directionPassed == 1) || // up can't down
            (this->direction == 1 && directionPassed == 0) || // down con't up
            (this->direction == 2 && directionPassed == 3) || // left can't right
            (this->direction == 3 && directionPassed == 2))   // and vice versa
        {
            return true;
        }
    }
    return false;
}

void Snake::setMyDirectionAndShift(int directionPassed, bool stopSnake)
{
    if (stopSnake)
        return;

    if (this->isDead)
        return;

    if (this->isStepBack(directionPassed))
        return;

    if (directionPassed >= 0 && directionPassed <= 3) // allowed directions
        this->direction = directionPassed;

    this->backupCoordinates();

    if (this->direction == 0)
        this->snakeCoosY[0] -= this->stepDivider; // up
    else if (this->direction == 1)
        this->snakeCoosY[0] += this->stepDivider; // down
    else if (this->direction == 2)
        this->snakeCoosX[0] -= this->stepDivider; // left
    else if (this->direction == 3)
        this->snakeCoosX[0] += this->stepDivider; // right

    this->restoreCoordinatesShifted();
}

int Snake::getElementOfEattenFruit(const int *FruitX, const int *FruitY, int fruitCount) const
{
    if (this->isDead)
        return 0;

    for (int fruitIndex = 0; fruitIndex < fruitCount; fruitIndex++)
    {
        if (this->snakeCoosX[0] == FruitX[fruitIndex] && this->snakeCoosY[0] == FruitY[fruitIndex])
            return fruitIndex + 1;
    }
    return 0;
}
