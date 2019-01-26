#pragma once

#include <feign/common.h>

template <class T>
class Primitive
{
public:
    Primitive(std::string str, T val)
    {
        name = str;
        value = val;
    }

    // getter methods
    T getValue() const
    {
        return value;
    }

    std::string getName() const
    {
        return name;
    }

    // setter methods
    void setValue(T param)
    {
        value = param;
    }

    void setName(std::string param)
    {
        name = param;
    }
protected:
    std::string name;
    T value;
};
