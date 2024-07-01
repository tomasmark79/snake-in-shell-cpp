# Snake Forever

## Description

Snake Forever was my C++ language excercise and it is a game inspired by the classic Snake game. This game is happening in a text console and is drawing with ansi chars. The game supports singleplayer, multiplayer like a hotseat up to four players or network multiplayer via UDP up to two players.

## Dependencies

This project depends on the following libraries:

- header-only eNet Library
  downloadable from repo https://github.com/zpl-c/enet

- audio playback and capture library - miniaudio - v0.11.21 - 2023-11-15
  downloadable from repo https://github.com/mackron/miniaudio

## Installation

Assuming you have CMake and a C++ compiler supporting C++17 installed, follow these steps for installation:

```bash
git clone https://github.com/yourusername/snake-forever.git
cd snake-forever
mkdir build
cd build
cmake ..
make
```

### Hot-Seat four players screenshot

![Screenshot_20240620_101022](https://github.com/tomasmark79/snake-forever/assets/44719504/035afc46-08f0-4b6e-8d87-91f33fd622b5)

### Two players internet multiplayer screenshot

![Screenshot_20240620_102013](https://github.com/tomasmark79/snake-forever/assets/44719504/14824aec-fdcf-4b5f-a1a6-53f747d8d1d4)


## License
This project is licensed under the Unlicense.
Initial author 2024 Tomáš Mark.
https://github.com/tomasmark79/snake-forever



