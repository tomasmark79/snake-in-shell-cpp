#include <SnakeLibrary/SnakeLibrary.hpp>
#include <snakelibrary/version.h>
#include <Logger.hpp>
#include <iostream>
#include <memory>

int main() {

  // c++11
  // std::unique_ptr<library::SnakeLibrary> lib(new library::SnakeLibrary());

  // c++14+
  std::unique_ptr<library::SnakeLibrary> lib = std::make_unique<library::SnakeLibrary>();

  return 0;
}