#include <feign/misc/primlist.h>

PrimitiveList::PrimitiveList()
{
    intPrimitives = std::vector<Primitive<int>*>();
    floatPrimitives = std::vector<Primitive<Float>*>();
    stringPrimitives = std::vector<Primitive<std::string>*>();
    transformPrimitives = std::vector<Primitive<Transform>*>();
    colorPrimitives = std::vector<Primitive<Color3f>*>();
}

PrimitiveList::~PrimitiveList()
{
    clearPrimitiveList();
}

void PrimitiveList::addIntPrimitive(Primitive<int>* prim)
{
    intPrimitives.push_back(prim);
}

void PrimitiveList::addFloatPrimitive(Primitive<Float>* prim)
{
    floatPrimitives.push_back(prim);
}

void PrimitiveList::addStringPrimitive(Primitive<std::string>* prim)
{
    stringPrimitives.push_back(prim);
}

void PrimitiveList::addTransformPrimitive(Primitive<Transform>* prim)
{
    transformPrimitives.push_back(prim);
}

void PrimitiveList::addColorPrimitive(Primitive<Color3f>* prim)
{
    colorPrimitives.push_back(prim);
}

bool PrimitiveList::findInt(std::string name, int& val) const
{
    for (int i = 0; i < intPrimitives.size(); ++i)
    {
        if (intPrimitives[i]->getName() == name)
        {
            val = intPrimitives[i]->getValue();
            return true;
        }
    }

    return false;
}

bool PrimitiveList::findFloat(std::string name, Float& val) const
{
    for (int i = 0; i < floatPrimitives.size(); ++i)
    {
        if (floatPrimitives[i]->getName() == name)
        {
            val = floatPrimitives[i]->getValue();
            return true;
        }
    }

    return false;
}

bool PrimitiveList::findString(std::string name, std::string& val) const
{
    for (int i = 0; i < stringPrimitives.size(); ++i)
    {
        if (stringPrimitives[i]->getName() == name)
        {
            val = stringPrimitives[i]->getValue();
            return true;
        }
    }

    return false;
}

bool PrimitiveList::findTransform(std::string name, Transform& val) const
{
    for (int i = 0; i < transformPrimitives.size(); ++i)
    {
        if (transformPrimitives[i]->getName() == name)
        {
            val = transformPrimitives[i]->getValue();
            return true;
        }
    }

    return false;
}

bool PrimitiveList::findColor(std::string name, Color3f& val) const
{
    for (int i = 0; i < colorPrimitives.size(); ++i)
    {
        if (colorPrimitives[i]->getName() == name)
        {
            val = colorPrimitives[i]->getValue();
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
