// MIT License
//
// Copyright (c) 2024 Tomáš Mark
//

#ifndef __USERUTIL_H__
#define __USERUTIL_H__

#include <iostream>
#include <string>
#include <limits>
#include <string_view>

#define STR_INPUT_FAIL "\n\nGame needs your correct inputs.\n\n"

class UserUtil
{
public:
    UserUtil(){};
    ~UserUtil(){};

    template <class T1>
    T1 getNumericAnswerFromPlayer(std::string_view question, T1 def, T1 min, T1 max, int attemptTreshold)
    {
        T1 value = 0;
        std::string enteredString;
        int totalAnswers = 0;
        do
        {
            if (totalAnswers == attemptTreshold)
            {
                std::cout << STR_INPUT_FAIL;
                throw std::runtime_error("failed");
            }

            std::cout << question;
            std::getline(std::cin, enteredString);
            std::cin.clear();

            if (enteredString.length() == 0) // no input
                return def;

            value = std::stoi(enteredString);
            totalAnswers++;
        } while (value < min || value > max);
        return value;
    }

    std::string getStringAnswerFromPlayer(std::string_view question, std::string_view def, int minStrSize, int maxStrSize, int attemptTreshold)
    {
        std::string answer = "";

        int totalAnswers = 0;
        do
        {
            if (totalAnswers == attemptTreshold)
            {
                std::cout << STR_INPUT_FAIL;
                throw std::runtime_error("failed");
            }
            std::cout << question;
            std::getline(std::cin, answer);
            std::cin.clear();

            if (answer.length() == 0) // no input
                return static_cast<std::string>(def);

            totalAnswers++;
        } while (static_cast<int>(answer.length()) < minStrSize || static_cast<int>(answer.length()) > maxStrSize);
        return answer;
    }

    bool isAnswerYes(std::string question)
    {
        if (std::tolower((question.substr(0, 3) == "yes")) ||
            std::tolower((question.substr(0, 2) == "ye")) ||
            std::tolower((question.substr(0, 1) == "y")))
            return true;
        return false;
    }
};

#endif // __USERUTIL_H__