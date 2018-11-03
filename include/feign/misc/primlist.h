#pragma once

#include <feign/common.h>
#include <feign/misc/prim.h>
#include <feign/math/transform.h>
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
    void addTransformPrimitive(Primitive<Transform>* prim);

    void findInt(string name, int& val, int base) const;
    void findFloat(string name, Float& val, Float base) const;
    void findString(string name, string& val, string base) const;
    void findTransform(string name, Transform& val, Transform base) const;

    void clearPrimitiveList();

    int size() const;

protected:
    bool findInt(string name, int& val) const;
    bool findFloat(string name, Float& val) const;
    bool findString(string name, string& val) const;
    bool findTransform(string name, Transform& val) const;

    vector<Primitive<int>*> intPrimitives;
    vector<Primitive<Float>*> floatPrimitives;
    vector<Primitive<string>*> stringPrimitives;
    vector<Primitive<Transform>*> transformPrimitives;
};
