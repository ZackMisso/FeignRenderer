#pragma once

#include <feign/common.h>

template <class T>
class Primitive
{
public:
    Primitive(string str, T val)
    {
        name = str;
        value = val;
    }

    // getter methods
    T getValue() const
    {
        return value;
    }

    string getName() const
    {
        return name;
    }

    // setter methods
    void setValue(T param)
    {
        value = param;
    }

    void setName(string param)
    {
        name = param;
    }
protected:
    string name;
    T value;
};
