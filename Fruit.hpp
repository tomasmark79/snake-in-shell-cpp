// MIT License
//
// Copyright (c) 2024 Tomáš Mark
//

#ifndef __FRUIT_H__
#define __FRUIT_H__

#include "Field.hpp"
#include <memory>

class Fruit
{
    int totalW;
    int totalH;
    std::shared_ptr<int[]> fruitXArr;
    std::shared_ptr<int[]> fruitYArr;
    int totalFruits;

public:
    Fruit(double fruitEmptiness, int totalW, int totalH);
    Fruit(const Fruit &fruit);
    Fruit &operator=(const Fruit &fruit);
    ~Fruit();
    const int *getFruitX() const;
    const int *getFruitY() const;
    int getRandomFruitX() const;
    int getRandomFruitY() const;
    int getTotalFruit() const;

    void addFruitItem();
    void refreshFruit(int refreshIndex);
};

#endif // __FRUIT_H__