cmake_minimum_required(VERSION 3.5.0)
project(snake-forever VERSION 0.1.0 LANGUAGES C CXX)

add_executable(snake-forever
    Fruit.cpp
    Game.cpp
    Graphic.cpp
    Keyboard.cpp
    Snake-forever.cpp
    Snake.cpp
)

target_include_directories(snake-forever PUBLIC
    ${PROJECT_SOURCE_DIR}/
)

# Přidání dalšího výstupu
add_executable(CppEnetWrapperMinimal
    CppEnetWrapperMinimal.cpp
)

target_include_directories(CppEnetWrapperMinimal PUBLIC
    ${PROJECT_SOURCE_DIR}/
)