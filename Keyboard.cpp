// MIT License
//
// Copyright (c) 2024 Tomáš Mark
//

#include "Keyboard.hpp"

#ifdef _WIN32
#include <conio.h> _kbhit && _getch
#else
#include <unistd.h>
#include <termios.h>
#include <sys/time.h>
#endif

void enableRawMode() {
#ifndef _WIN32
    termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
#endif
}

void disableRawMode() {
#ifndef _WIN32
    termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= (ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
#endif
}

int kbhit() {
#ifdef _WIN32
    return _kbhit();
#else
    timeval tv = {0, 0};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
#endif
}

int getch() {
#ifdef _WIN32
    return _getch();
#else
    unsigned char ch;
    if (read(STDIN_FILENO, &ch, 1) < 0) {
        return -1;
    } else {
        return ch;
    }
#endif
}

int Keyboard::getMyKeyboardCode() const
{
    enableRawMode();
    if (kbhit())
    {
        switch (getch())
        {
        case UP1:
            return 10;
        case DOWN1:
            return 11;
        case LEFT1:
            return 12;
        case RIGHT1:
            return 13;

        case UP2:
            return 20;
        case DOWN2:
            return 21;
        case LEFT2:
            return 22;
        case RIGHT2:
            return 23;

        case UP3:
            return 30;
        case DOWN3:
            return 31;
        case LEFT3:
            return 32;
        case RIGHT3:
            return 33;

        case UP4:
            return 40;
        case DOWN4:
            return 41;
        case LEFT4:
            return 42;
        case RIGHT4:
            return 43;

        case 'x':
            // exit
            return 4;
        case 'X':
            // exit
            return 4;
        case 'h':
            // help form
            return 5;
        case 'H':
            // help form
            return 5;
        case 'r':
            return 6;
        case 'R':
            return 6;
        }
    }
    disableRawMode();
    return -1;
}

