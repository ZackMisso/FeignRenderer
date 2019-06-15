#include <feign/misc/primlist.h>

// TODO: replace all of these with macros

PrimitiveList::PrimitiveList()
{
    int_primitives = std::vector<Primitive<int>*>();
    float_primitives = std::vector<Primitive<Float>*>();
    vec3_primitives = std::vector<Primitive<Vec3>*>();
    string_primitives = std::vector<Primitive<std::string>*>();
    transform_primitives = std::vector<Primitive<Transform>*>();
    color_primitives = std::vector<Primitive<Color3f>*>();
}

PrimitiveList::~PrimitiveList()
{
    clearPrimitiveList();
}

void PrimitiveList::addIntPrimitive(Primitive<int>* prim)
{
    int_primitives.push_back(prim);
}

void PrimitiveList::addFloatPrimitive(Primitive<Float>* prim)
{
    float_primitives.push_back(prim);
}

void PrimitiveList::addVec3Primitive(Primitive<Vec3>* prim)
{
    vec3_primitives.push_back(prim);
}

void PrimitiveList::addStringPrimitive(Primitive<std::string>* prim)
{
    string_primitives.push_back(prim);
}

void PrimitiveList::addTransformPrimitive(Primitive<Transform>* prim)
{
    transform_primitives.push_back(prim);
}

void PrimitiveList::addColorPrimitive(Primitive<Color3f>* prim)
{
    color_primitives.push_back(prim);
}

bool PrimitiveList::findInt(std::string name, int& val) const
{
    for (int i = 0; i < int_primitives.size(); ++i)
    {
        if (int_primitives[i]->getName() == name)
        {
            val = int_primitives[i]->getValue();
            return true;
        }
    }

    return false;
}

bool PrimitiveList::findFloat(std::string name, Float& val) const
{
    for (int i = 0; i < float_primitives.size(); ++i)
    {
        if (float_primitives[i]->getName() == name)
        {
            val = float_primitives[i]->getValue();
            return true;
        }
    }

    return false;
}

bool PrimitiveList::findVec3(std::string name, Vec3& val) const
{
    for (int i = 0; i < vec3_primitives.size(); ++i)
    {
        if (vec3_primitives[i]->getName() == name)
        {
            val = vec3_primitives[i]->getValue();
            return true;
        }
    }

    return false;
}

bool PrimitiveList::findString(std::string name, std::string& val) const
{
    for (int i = 0; i < string_primitives.size(); ++i)
    {
        if (string_primitives[i]->getName() == name)
        {
            val = string_primitives[i]->getValue();
            return true;
        }
    }

    return false;
}

bool PrimitiveList::findTransform(std::string name, Transform& val) const
{
    for (int i = 0; i < transform_primitives.size(); ++i)
    {
        if (transform_primitives[i]->getName() == name)
        {
            val = transform_primitives[i]->getValue();
            return true;
        }
    }

    return false;
}

bool PrimitiveList::findColor(std::string name, Color3f& val) const
{
    for (int i = 0; i < color_primitives.size(); ++i)
    {
        if (color_primitives[i]->getName() == name)
        {
            val = color_primitives[i]->getValue();
            return true;
        }
    }

    return false;
}

void PrimitiveList::findInt(std::string name, int& val, int base) const
{
    bool found = findInt(name, val);
    if (!found)
    {
        val = base;
    }
}

void PrimitiveList::findFloat(std::string name, Float& val, Float base) const
{
    bool found = findFloat(name, val);
    if (!found)
    {
        val = base;
    }
}

void PrimitiveList::findString(std::string name, std::string& val, std::string base) const
{
    bool found = findString(name, val);
    if (!found)
    {
        val = base;
    }
}

void PrimitiveList::findTransform(std::string name, Transform& val, Transform base) const
{
    bool found = findTransform(name, val);
    if (!found)
    {
        val = base;
    }
}

void PrimitiveList::findColor(std::string name, Color3f& val, Color3f base) const
{
    bool found = findColor(name, val);
    if (!found)
    {
        val = base;
    }
}

void PrimitiveList::clearPrimitiveList()
{
    for (int i = 0; i < intPrimitives.size(); ++i)
    {
        delete intPrimitives[i];
    }

    for (int i = 0; i < floatPrimitives.size(); ++i)
    {
        delete floatPrimitives[i];
    }

    for (int i = 0; i < stringPrimitives.size(); ++i)
    {
        delete stringPrimitives[i];
    }

    for (int i = 0; i < transformPrimitives.size(); ++i)
    {
        delete transformPrimitives[i];
    }

    for (int i = 0; i < colorPrimitives.size(); ++i)
    {
        delete colorPrimitives[i];
    }

    intPrimitives.clear();
    floatPrimitives.clear();
    stringPrimitives.clear();
    transformPrimitives.clear();
    colorPrimitives.clear();
}

int PrimitiveList::size() const
{
    return intPrimitives.size() +
           floatPrimitives.size() +
           stringPrimitives.size() +
           transformPrimitives.size() +
           colorPrimitives.size();
}
