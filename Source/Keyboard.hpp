// MIT License
//
// Copyright (c) 2024 Tomáš Mark
//

#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

class Keyboard
{
    enum playerControlLayout1
   
    {
        UP1 = 'w',
        DOWN1 = 's',
        LEFT1 = 'a',
        RIGHT1 = 'd'
    };

    enum playerControlLayout2
    {
        UP2 = 'i',
        DOWN2 = 'k',
        LEFT2 = 'j',
        RIGHT2 = 'l'
    };

    enum playerControlLayout3
    {
        UP3 = '8',
        DOWN3 = '5',
        LEFT3 = '4',
        RIGHT3 = '6'
    };

    enum playerControlLayout4
    {
        UP4 = '[',
        DOWN4 = '\'',
        LEFT4 = ';',
        RIGHT4 = '\\'
    };

public:
    Keyboard() = default;
    int getMyKeyboardCode() const;
};

#endif // __KEYBOARD_H__