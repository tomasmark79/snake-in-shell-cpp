#pragma once
#include <string>
#include <sstream>

/// @brief Return version of used C++
class CppVersion
{
    std::string cppVStr;

public:
    CppVersion()
        : cppVStr("")
    {
        if (__cplusplus == 202101L)
            this->cppVStr = "C++23";
        else if (__cplusplus == 202002L)
            this->cppVStr = "C++20";
        else if (__cplusplus == 201703L)
            this->cppVStr = "C++17";
        else if (__cplusplus == 201402L)
            this->cppVStr = "C++14";
        else if (__cplusplus == 201103L)
            this->cppVStr = "C++11";
        else if (__cplusplus == 199711L)
            this->cppVStr = "C++98";
        else
            this->cppVStr = "pre-standard C++." + std::to_string(__cplusplus);
    }

    std::string getCppVersion() const
    {
        return this->cppVStr;
    };

    friend std::ostream &operator<<(std::ostream &str, const CppVersion &cppVersion)
    {
        return str << cppVersion.cppVStr;
    };
};
