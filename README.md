# Snake Forever

This snake game example of OOP is full of incorrect implementations that demonstrate how OOP should not be designed.

## Description

Snake Forever was my C++ language excercise and it is a game inspired by the classic Snake game. This game is happening in a text console and is drawing with ansi chars. The game supports singleplayer, multiplayer like a hotseat up to four players or network multiplayer via UDP up to two players.

## Dependencies

This project depends on the following libraries:
notice: both already included

Latest version can be downloaded from
- header-only eNet Library
  downloadable from repo https://github.com/zpl-c/enet

- audio playback and capture library - miniaudio - v0.11.21 - 2023-11-15
  downloadable from repo https://github.com/mackron/miniaudio

## Installation

Assuming you have CMake and a C++ compiler supporting C++17 installed, follow these steps for installation:

```bash
git clone https://github.com/tomasmark79/snake-in-shell-cpp.git
cd snake-in-shell-cpp
mkdir build
cd build
cmake ..
make
```

### Single Player

<img width="366" alt="image" src="https://github.com/user-attachments/assets/34a80fc8-60af-45ab-aed4-7f7dadfe4edb">

### Hot-Seat four players screenshot

<img width="366" alt="image" src="https://github.com/user-attachments/assets/330b87ac-57bc-456d-a74f-0b2e5c6d7673">

### Internet multiplayer screenshot

<img width="972" alt="image" src="https://github.com/user-attachments/assets/1bcb0d2f-f442-4a26-a35f-19c53476674a">
<img width="887" alt="image" src="https://github.com/user-attachments/assets/715ce396-6a9a-4409-bdbb-8adb074e95b7">

## License
Snake Forever is open-source software, released under the unlicense.
