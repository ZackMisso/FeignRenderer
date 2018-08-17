#pragma once

#include <feign/misc/prim.h>
#include <vector>
#include <string>

class PrimitiveList
{
public:
    PrimitiveList();
    ~PrimitiveList();

    void addIntPrimitive(Primitive<int>* prim);
    void addFloatPrimitive(Primitive<Float>* prim);
    void addStringPrimitive(Primitive<string>* prim);

    bool findInt(string name, int& val) const;
    bool findFloat(string name, Float& val) const;
    bool findString(string name, string& val) const;

    void clearPrimitiveList();

protected:
    vector<Primitive<int>*> intPrimitives;
    vector<Primitive<Float>*> floatPrimitives;
    vector<Primitive<string>*> stringPrimitives;
};
