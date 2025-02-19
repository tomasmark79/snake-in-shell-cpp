// MIT License
//
// Copyright (c) 2024 Tomáš Mark
//

#ifndef __FIELD_H__
#define __FIELD_H__

class Field
{
    int width;
    int height;

public:
    Field(short width, short height)
        : width(width), height(height){};

    int getFieldWidth() const
    {
        return this->width;
    };

    int getFieldHeight() const
    {
        return this->height;
    };

    int getAreaElements() const
    {
        return this->width * this->height;
    };
};

#endif // __FIELD_H__