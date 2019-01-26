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
    void addStringPrimitive(Primitive<std::string>* prim);
    void addTransformPrimitive(Primitive<Transform>* prim);
    void addColorPrimitive(Primitive<Color3f>* prim);

    void findInt(std::string name, int& val, int base) const;
    void findFloat(std::string name, Float& val, Float base) const;
    void findString(std::string name, std::string& val, std::string base) const;
    void findTransform(std::string name, Transform& val, Transform base) const;
    void findColor(std::string name, Color3f& val, Color3f base) const;

    void clearPrimitiveList();

    int size() const;

protected:
    bool findInt(std::string name, int& val) const;
    bool findFloat(std::string name, Float& val) const;
    bool findString(std::string name, std::string& val) const;
    bool findTransform(std::string name, Transform& val) const;
    bool findColor(std::string name, Color3f& val) const;

    std::vector<Primitive<int>*> intPrimitives;
    std::vector<Primitive<Float>*> floatPrimitives;
    std::vector<Primitive<std::string>*> stringPrimitives;
    std::vector<Primitive<Transform>*> transformPrimitives;
    std::vector<Primitive<Color3f>*> colorPrimitives;
};
